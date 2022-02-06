#include "BoostUnitTest.hpp"

#include <QBuffer>

#include "Network/DeckDataStream.hpp"
#include "Network/MessageReceiver.hpp"
#include "Network/Messages/HelloMessage.hpp"
#include "QStringLiteral.hpp"
#include "Services/ISocketHolder.hpp"

CT_BOOST_AUTO_TEST_SUITE(MessageReceiverTests)

using namespace QtPiDeck::Bus;
using namespace QtPiDeck::Network;
using namespace QtPiDeck::Services;
using namespace QtPiDeck::Utilities;

class Device : public QIODevice {
  Q_OBJECT // NOLINT
protected:
  // QIODevice
  auto readData(char* /*data*/, qint64 /*maxlen*/) -> qint64 override {
    throw std::logic_error("The method or operation is not implemented.");
  }

  auto writeData(const char* /*data*/, qint64 /*len*/) -> qint64 override {
    throw std::logic_error("The method or operation is not implemented.");
  }
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

CT_BOOST_AUTO_TEST_CASE(shouldCreateObject) {
  const auto receiver = MessageReceiver{{std::make_shared<SocketHolder>(), nullptr, nullptr}};
}

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
  // ISocketHolder
  auto socket() -> QIODevice* final { return &m_device; }

private:
  TDevice m_device;
};

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

CT_BOOST_AUTO_TEST_CASE(shouldHandleEmptyData) {
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDevice>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, nullptr}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();
  CT_BOOST_TEST(!bus->getLastMessage().has_value());
}

class ReadableDeviceHeaderPart : public ReadableDevice {
public:
  // QIODevice
  auto readData(char* data, qint64 maxlen) -> qint64 final {
    static auto state = State::DATA;
    if (state == State::DATA) {
      constexpr auto value = uint64_t{36};
      auto buffer          = QByteArray{};
      buffer.reserve(static_cast<int>(maxlen));
      auto stream = DeckDataStream{&buffer, QIODevice::WriteOnly};
      stream << value;
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

CT_BOOST_AUTO_TEST_CASE(shouldHandleHeaderPart) {
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderPart>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, nullptr}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();
  CT_BOOST_TEST(!bus->getLastMessage().has_value());
}

class ReadableDeviceHeaderZeroPayload : public ReadableDevice {
public:
  // QIODevice
  auto readData(char* data, qint64 maxlen) -> qint64 final {
    if (m_state == State::DATA) {
      constexpr auto value = uint64_t{0};
      auto buffer          = QByteArray{};
      buffer.reserve(static_cast<int>(maxlen));
      auto stream = DeckDataStream{&buffer, QIODevice::WriteOnly};
      stream << value << MessageType::Dummy << CT_QStringLiteral("ID");
      auto* src            = buffer.data();
      constexpr auto bytes = sizeof(uint64_t) + sizeof(MessageType) + 4 + 4;
      memcpy(data, src, bytes);
      m_state = State::END;
      return bytes;
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

CT_BOOST_AUTO_TEST_CASE(shouldHandleHeaderZeroPayloadNoMapped) {
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderZeroPayload>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, std::make_shared<NoMapMapper>()}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();
  CT_BOOST_TEST(!bus->getLastMessage().has_value());
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

CT_BOOST_AUTO_TEST_CASE(shouldHandleHeaderZeroPayloadMapped) {
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderZeroPayload>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, std::make_shared<Mapper>()}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();

  const auto lastMessage = bus->getLastMessage();

  CT_BOOST_TEST(lastMessage.has_value());
  CT_BOOST_TEST(lastMessage.value().payload.isEmpty());
  CT_BOOST_TEST(lastMessage.value().messageType == Mapper::expectedType);
}

class ReadableDeviceHeaderWithIncompletePayload final : public ReadableDevice {
public:
  // QIODevice
  auto readData(char* data, qint64 maxlen) -> qint64 final {
    if (m_state == State::DATA) {
      constexpr auto value = uint64_t{32};
      auto buffer          = QByteArray{};
      buffer.reserve(static_cast<int>(maxlen));
      auto stream = DeckDataStream{&buffer, QIODevice::WriteOnly};
      stream << value << MessageType::Dummy << CT_QStringLiteral("ID");
      auto* src            = buffer.data();
      constexpr auto bytes = sizeof(uint64_t) + sizeof(MessageType) + 4 + 4;
      memcpy(data, src, bytes);
      m_state = State::END;
      return bytes;
    }

    return 0;
  }

  auto bytesAvailable() const -> qint64 final { return 10; }

private:
  enum class State { DATA, END };
  State m_state = State::DATA;
};

CT_BOOST_AUTO_TEST_CASE(shouldHandleHeaderIncompletePayloadMapped) {
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderWithIncompletePayload>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, std::make_shared<Mapper>()}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();

  const auto lastMessage = bus->getLastMessage();
  CT_BOOST_TEST(!lastMessage.has_value());
}

class ReadableDeviceHeaderWithFullPayload final : public ReadableDevice {
public:
  // QIODevice
  auto readData(char* data, qint64 maxlen) -> qint64 final {
    if (m_state == State::DATA) {
      auto buffer          = QByteArray{};
      buffer.reserve(static_cast<int>(maxlen));
      auto stream = DeckDataStream{&buffer, QIODevice::WriteOnly};
      m_message.messageHeader(CT_QStringLiteral("ID")).write(stream);
      auto* src = buffer.data();
      memcpy(data, src, buffer.size());
      m_state = State::PAYLOAD;
      return buffer.size();
    }

    if (m_state == State::PAYLOAD) {
      auto buffer = QByteArray{};
      buffer.reserve(static_cast<int>(maxlen));
      auto stream = DeckDataStream{&buffer, QIODevice::WriteOnly};
      m_message.write(stream);
      auto* src = buffer.data();
      memcpy(data, src, buffer.size());
      m_state = State::END;
      return buffer.size();
    }

    return 0;
  }

  auto bytesAvailable() const -> qint64 final { return m_message.messageSize(); }

private:
  enum class State { DATA, PAYLOAD, END };
  State m_state{State::DATA};
  Hello m_message{};
};

CT_BOOST_AUTO_TEST_CASE(shouldHandleHeaderFullPayloadMapped) {
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderWithFullPayload>>();
  auto bus            = std::make_shared<MessageBus>();
  const auto receiver = MessageReceiver{{holder, bus, std::make_shared<Mapper>()}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();

  const auto lastMessage = bus->getLastMessage();
  CT_BOOST_TEST(lastMessage.has_value());
  CT_BOOST_TEST(lastMessage.value().messageType == Mapper::expectedType);
  CT_BOOST_TEST(lastMessage.value().payload.size() = Hello{}.messageSize());
}

CT_BOOST_AUTO_TEST_SUITE_END()

#include "MessageReceiverTests.moc"