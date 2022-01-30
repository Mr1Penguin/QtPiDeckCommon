#include "BoostUnitTest.hpp"

#include <QBuffer>

#include "Network/DeckDataStream.hpp"
#include "Network/MessageReceiver.hpp"
#include "QStringLiteral.hpp"
#include "Services/ISocketHolder.hpp"

CT_BOOST_AUTO_TEST_SUITE(MessageReceiverTests)

using namespace QtPiDeck::Bus;
using namespace QtPiDeck::Network;
using namespace QtPiDeck::Services;

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

CT_BOOST_AUTO_TEST_CASE(shouldHandleEmptyData) {
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDevice>>();
  const auto receiver = MessageReceiver{{holder, nullptr, nullptr}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();
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
  const auto receiver = MessageReceiver{{holder, nullptr, nullptr}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();
}

class ReadableDeviceHeaderZeroPayload : public ReadableDevice {
public:
  // QIODevice
  auto readData(char* data, qint64 maxlen) -> qint64 final {
    static auto state = State::DATA;
    if (state == State::DATA) {
      constexpr auto value = uint64_t{0};
      auto buffer          = QByteArray{};
      buffer.reserve(static_cast<int>(maxlen));
      auto stream = DeckDataStream{&buffer, QIODevice::WriteOnly};
      stream << value << MessageType::Dummy << CT_QStringLiteral("ID");
      auto* src            = buffer.data();
      constexpr auto bytes = sizeof(uint64_t) + sizeof(MessageType) + 4 + 4;
      memcpy(data, src, bytes);
      state = State::END;
      return bytes;
    }

    return 0;
  }

private:
  enum class State { DATA, END };
};

class NoMapMapper final : public IDeckMessageToBusMessageMapper {
public:
  // IDeckMessageToBusMessageMapper
  auto getBusMessageType(MessageType) const -> std::optional<decltype(Message::messageType)> final {
    return std::nullopt;
  }
};

CT_BOOST_AUTO_TEST_CASE(shouldHandleHeaderZeroPayloadNoMapped) {
  auto holder         = std::make_shared<ReadableSocketHolder<ReadableDeviceHeaderZeroPayload>>();
  const auto receiver = MessageReceiver{{holder, nullptr, std::make_shared<NoMapMapper>()}};
  auto* device        = qobject_cast<ReadableDevice*>(holder->socket());

  device->emitReadyRead();
}

CT_BOOST_AUTO_TEST_SUITE_END()

#include "MessageReceiverTests.moc"