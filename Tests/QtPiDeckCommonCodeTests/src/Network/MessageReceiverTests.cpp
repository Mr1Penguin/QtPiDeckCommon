#include "BoostUnitTest.hpp"

#include "Network/DeckDataStream.hpp"
#include "Network/MessageHeader.hpp"
#include "Network/MessageReceiver.hpp"
#include "Network/Messages/HelloMessage.hpp"
#include "QStringLiteral.hpp"
#include "Services/ISocketHolder.hpp"
#include "Utilities/QStringUtils.hpp"

#include <cstring>

BOOST_AUTO_TEST_SUITE(MessageReceiverTests) // NOLINT

using namespace QtPiDeck::Bus;
using namespace QtPiDeck::Network;
using namespace QtPiDeck::Services;
using namespace QtPiDeck::Utilities;

class Device : public QIODevice {
  Q_OBJECT // NOLINT
protected:
  // QIODevice
  auto readData(char* /*data*/, qint64 /*maxlen*/) -> qint64 override {
    throw
      std::logic_error("The method or operation is not implemented.");
  }

  auto writeData(const char* /*data*/, qint64 /*len*/) -> qint64 override {
    throw std::logic_error("The method or operation is not implemented.");
  }

public:
};

class SocketHolder : public ISocketHolder {
public:
  // ISocketHolder
  void requestWrite(const QByteArray& /*data*/) override {}
  void setSocket(QIODevice* /*socket*/) override {}
  auto socket() -> QIODevice* override { return &m_device; }

private:
  Device m_device;
};

class ReadableDevice : public Device {
  Q_OBJECT // NOLINT
public:
  ReadableDevice() { setOpenMode(QIODevice::OpenModeFlag::ReadOnly); }
  // QIODevice
  auto readData(char* /*data*/, qint64 /*maxlen*/) -> qint64 override { return 0; }

  void emitReadyRead() { emit readyRead(); }
};

template<class TDevice>
class ReadableSocketHolder final : public SocketHolder {
public:
  auto socket() -> QIODevice* final { return &m_device; }

private:
  TDevice m_device;
};

BOOST_AUTO_TEST_CASE(ShouldCreateObject) // NOLINT
{ const auto receiver = MessageReceiver{{std::make_shared<SocketHolder>(), nullptr, nullptr}}; }

class MessageBus final : public IMessageBus {
public:
  // IMessageBus
  [[nodiscard]] auto subscribe(QObject* /*context*/,
                               const std::function<void(const QtPiDeck::Bus::Message&)>& /*method*/) noexcept
      -> Connection final {
    return {};
  }
  [[nodiscard]] auto subscribe(QObject* /*context*/,
                               const std::function<void(const QtPiDeck::Bus::Message&)>& /*method*/,
                               uint64_t /*messageType*/) noexcept -> Connection final {
    return {};
  }
  void unsubscribe(Connection& /*connection*/) noexcept final {}
  void sendMessage(QtPiDeck::Bus::Message message) noexcept final { m_lastMessage = message; }

  [[nodiscard]] auto getLastMessage() const -> std::optional<QtPiDeck::Bus::Message> { return m_lastMessage; }

private:
  std::optional<QtPiDeck::Bus::Message> m_lastMessage;
};

BOOST_AUTO_TEST_CASE(shouldHandleEmptyData) // NOLINT
{
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDevice>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, nullptr}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();
  BOOST_TEST(!bus->getLastMessage().has_value()); // NOLINT
}

class ReadableDeviceHeaderPart : public ReadableDevice {
public:
  // QIODevice
  auto readData(char* data, qint64 /*maxlen*/) -> qint64 final {
    static auto state = State::DATA;
    if (state == State::DATA) {
      constexpr auto value = uint64_t{36};
      auto buffer          = QByteArray{};
      auto stream          = DeckDataStream{&buffer, QIODevice::WriteOnly};
      stream << static_cast<quint64>(value);
      auto* src            = buffer.data();
      constexpr auto bytes = sizeof(uint64_t);
      memcpy(data, src, bytes);
      state = State::END;
      return bytes;
    }

    return 0;
  }

private:
  enum class State { DATA, END };
};

BOOST_AUTO_TEST_CASE(shouldHandleHeaderPart) // NOLINT
{
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderPart>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, nullptr}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();
  BOOST_TEST(!bus->getLastMessage().has_value()); // NOLINT
}

class ReadableDeviceHeaderZeroPayload : public ReadableDevice {
public:
  // QIODevice
  auto readData(char* data, qint64 maxlen) -> qint64 final {
    if (m_state == State::DATA) {
      constexpr auto value = uint64_t{0};
      auto buffer          = QByteArray{};
      buffer.reserve(static_cast<int>(maxlen));
      auto stream   = DeckDataStream{&buffer, QIODevice::WriteOnly};
      const auto id = CT_QStringLiteral("ID");
      stream << static_cast<quint64>(value) << MessageType::Dummy << id;
      auto* src            = buffer.data();
      const auto rawStrLen = 4 + id.size() * 2;
      const auto bytes     = sizeof(uint64_t) + sizeof(MessageType) + rawStrLen;
      memcpy(data, src, bytes);
      m_state = State::END;
      return static_cast<qint64>(bytes);
    }

    return 0;
  }

private:
  enum class State { DATA, END };
  State m_state = State::DATA;
};

class NoMapMapper final : public IDeckMessageToBusMessageMapper {
public:
  // IDeckMessageToBusMessageMapper
  [[nodiscard]] auto getBusMessageType(MessageType /*messageType*/) const
      -> std::optional<decltype(QtPiDeck::Bus::Message::messageType)> final {
    return std::nullopt;
  }
};

BOOST_AUTO_TEST_CASE(shouldHandleHeaderZeroPayloadNoMapped) // NOLINT
{
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderZeroPayload>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, std::make_shared<NoMapMapper>()}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();
  BOOST_TEST(!bus->getLastMessage().has_value()); // NOLINT
}

class Mapper final : public IDeckMessageToBusMessageMapper {
public:
  // IDeckMessageToBusMessageMapper
  [[nodiscard]] auto getBusMessageType(MessageType /*messageType*/) const
      -> std::optional<decltype(QtPiDeck::Bus::Message::messageType)> final {
    return expectedType;
  }

  static constexpr decltype(QtPiDeck::Bus::Message::messageType) expectedType = 10;
};

BOOST_AUTO_TEST_CASE(shouldHandleHeaderZeroPayloadMapped) // NOLINT
{
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderZeroPayload>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, std::make_shared<Mapper>()}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();

  const auto lastMessage = bus->getLastMessage();

  BOOST_TEST(lastMessage.has_value());                                 // NOLINT
  BOOST_TEST(lastMessage.value().payload.isEmpty());                   // NOLINT
  BOOST_TEST(lastMessage.value().messageType == Mapper::expectedType); // NOLINT
}

class ReadableDeviceHeaderWithIncompletePayload final : public ReadableDevice {
public:
  // QIODevice
  auto readData(char* data, qint64 maxlen) -> qint64 final {
    if (m_state == State::DATA) {
      constexpr auto value = uint64_t{32};
      auto buffer          = QByteArray{};
      buffer.reserve(static_cast<int>(maxlen));
      auto stream   = DeckDataStream{&buffer, QIODevice::WriteOnly};
      const auto id = CT_QStringLiteral("ID");
      stream << static_cast<quint64>(value) << MessageType::Dummy << id;
      auto* src            = buffer.data();
      const auto rawStrLen = 4 + id.size() * 2;
      const auto bytes     = sizeof(uint64_t) + sizeof(MessageType) + rawStrLen;
      memcpy(data, src, bytes);
      m_state = State::END;
      return static_cast<qint64>(bytes);
    }

    return 0;
  }

private:
  enum class State { DATA, END };
  State m_state = State::DATA;
};

BOOST_AUTO_TEST_CASE(shouldHandleHeaderIncompletePayloadMapped) // NOLINT
{
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderWithIncompletePayload>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, std::make_shared<Mapper>()}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();

  const auto lastMessage = bus->getLastMessage();
  BOOST_TEST(!lastMessage.has_value()); // NOLINT
}

class ReadableDeviceHeaderWithFullPayload final : public ReadableDevice {
public:
  // QIODevice
  auto readData(char* data, qint64 /*maxlen*/) -> qint64 final {
    if (m_state == State::DATA) {
      auto arr                   = QByteArray{};
      auto stream                = DeckDataStream{&arr, QIODevice::WriteOnly};
      constexpr auto payloadSize = quint64{24};
      stream << payloadSize << MessageType::Dummy << QString("ID");
      memcpy(data, arr.data(), arr.size());
      m_state = State::DATAEND;
      return arr.size();
    }

    if (m_state == State::DATAEND) {
      m_state = State::PAYLOAD;
      return 0;
    }

    if (m_state == State::PAYLOAD) {
      auto arr    = QByteArray{};
      auto stream = DeckDataStream{&arr, QIODevice::WriteOnly};
      stream << QString(R"({"val":24})");
      memcpy(data, arr.data(), arr.size());
      m_state = State::PAYLOADEND;
      return arr.size();
    }

    return 0;
  }

private:
  enum class State { DATA, DATAEND, PAYLOAD, PAYLOADEND };
  State m_state{State::DATA};
};

BOOST_AUTO_TEST_CASE(shouldHandleHeaderFullPayloadMapped) // NOLINT
{
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderWithFullPayload>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, std::make_shared<Mapper>()}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();

  const auto lastMessage = bus->getLastMessage();
  BOOST_REQUIRE(lastMessage.has_value());                              // NOLINT
  BOOST_TEST(lastMessage.value().messageType == Mapper::expectedType); // NOLINT
  BOOST_TEST(!lastMessage.value().payload.isEmpty());                  // NOLINT
}

BOOST_AUTO_TEST_SUITE_END() // NOLINT

#include "MessageReceiverTests.moc"
