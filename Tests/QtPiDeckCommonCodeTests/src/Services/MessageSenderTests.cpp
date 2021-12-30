#include "BoostUnitTest.hpp"

#include "Network/MessageHeader.hpp"
#include "Network/Messages/Message.hpp"
#include "Services/ISocketHolder.hpp"
#include "Services/Ioc.hpp"
#include "Services/MessageSender.hpp"
#include "Utilities/ISerializable.hpp"

using namespace QtPiDeck::Services;
using namespace QtPiDeck::Network;
using namespace QtPiDeck::Utilities;

namespace {
class Socket : public QIODevice {
  Q_OBJECT // NOLINT
  // QIODevice
  auto readData(char* /*data*/, qint64 /*maxlen*/) -> qint64 final { return qint64{}; }
  auto writeData(const char* /*data*/, qint64 /*len*/) -> qint64 final { return qint64{}; }
};

class SocketHolder final : public ISocketHolder {
public:
  auto requestedWrites() const -> const std::vector<decltype(std::declval<QByteArray>().size())>& {
    return m_requestedWrites;
  }

  // ISocketHolder
  void requestWrite(const QByteArray& data) final { m_requestedWrites.push_back(data.size()); }
  void setSocket(QIODevice* /*socket*/) final {}
  [[nodiscard]] auto socket() -> QIODevice* { throw std::logic_error("The method or operation is not implemented."); }

private:
  Socket m_socket;
  std::vector<decltype(std::declval<QByteArray>().size())> m_requestedWrites;
};

struct TestMessage final : public ISerializable, public Messages::Message {
  // Message
  auto messageSize() const -> uint64_t final { return uint64_t{1}; }
  auto messageHeader(const QString& requestId) const -> MessageHeader final {

    auto header        = MessageHeader{};
    header.dataSize    = 1;
    header.messageType = MessageType::Dummy;
    header.requestId   = requestId;
    return header;
  }
  // ISerializable
  void read(QDataStream& /*stream*/) final { throw std::logic_error("The method or operation is not implemented."); }
  void write(QDataStream& stream) const final { stream << ';'; }
};

struct Fixture {
public:
  Fixture() : m_messageSender(), m_ioc() {
    m_ioc.registerSingleton<ISocketHolder>(std::make_shared<SocketHolder>());
    m_messageSender = m_ioc.make<MessageSender, CreationType::UniquePointer>();
  }

protected:
  [[nodiscard]] auto messageSender() -> MessageSender& { return *m_messageSender; }
  [[nodiscard]] auto socketHolder() const -> const SocketHolder& {
    return *static_cast<SocketHolder*>(m_ioc.resolveService<ISocketHolder>().get());
  }

private:
  std::unique_ptr<MessageSender> m_messageSender;
  Ioc m_ioc;
};
}

CT_BOOST_FIXTURE_TEST_SUITE(IocTests, Fixture)
CT_BOOST_AUTO_TEST_CASE(shouldRequestWriteOfHeader) {
  const auto header = []() -> MessageHeader {
    auto res        = MessageHeader{};
    res.dataSize    = 0;
    res.messageType = MessageType::Dummy;
    res.requestId   = QStringLiteral(".");
    return res;
  }();
  constexpr auto headerSize = size_t{18};
  Fixture::messageSender().send(header);
  const auto& requestedWrites = Fixture::socketHolder().requestedWrites();
  CT_BOOST_TEST(requestedWrites.size() == 1);
  CT_BOOST_TEST(requestedWrites.front() == headerSize);
}

CT_BOOST_AUTO_TEST_CASE(shouldRequestWriteOfMessage) {
  const auto message = TestMessage{};
  constexpr auto headerSize = size_t{19};
  Fixture::messageSender().send(message, QStringLiteral("."));
  const auto& requestedWrites = Fixture::socketHolder().requestedWrites();
  CT_BOOST_TEST(requestedWrites.size() == 1);
  CT_BOOST_TEST(requestedWrites.front() == headerSize);
}

CT_BOOST_AUTO_TEST_SUITE_END()

#include "MessageSenderTests.moc"