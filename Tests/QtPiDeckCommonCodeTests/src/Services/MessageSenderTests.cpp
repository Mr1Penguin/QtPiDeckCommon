#include "BoostUnitTest.hpp"

#include "Network/MessageHeader.hpp"
#include "QStringLiteral.hpp"
#include "Services/ISocketHolder.hpp"
#include "Services/Ioc.hpp"
#include "Services/MessageSender.hpp"
#include "Utilities/QStringUtils.hpp"

using namespace QtPiDeck::Services;
using namespace QtPiDeck::Network;
using namespace QtPiDeck::Utilities;

namespace {
class Socket : public QIODevice {
  Q_OBJECT // NOLINT
protected:
  // QIODevice
  auto readData(char* /*data*/, qint64 /*maxlen*/) -> qint64 final { return qint64{}; }
  auto writeData(const char* /*data*/, qint64 /*len*/) -> qint64 final { return qint64{}; }
};

class SocketHolder final : public ISocketHolder {
public:
  [[nodiscard]] auto requestedWrites() const -> const std::vector<decltype(std::declval<QByteArray>().size())>& {
    return m_requestedWrites;
  }

  // ISocketHolder
  void requestWrite(const QByteArray& data) final { m_requestedWrites.push_back(data.size()); }
  void setSocket(QIODevice* /*socket*/) final {}
  [[nodiscard]] auto socket() -> QIODevice* final {
    throw std::logic_error("The method or operation is not implemented.");
  }

private:
  Socket m_socket;
  std::vector<decltype(std::declval<QByteArray>().size())> m_requestedWrites;
};

struct Fixture {
public:
  Fixture() {
    m_ioc.registerSingleton<ISocketHolder>(std::make_shared<SocketHolder>());
    m_messageSender = m_ioc.make<MessageSender, CreationType::UniquePointer>();
  }

protected:
  [[nodiscard]] auto messageSender() -> MessageSender& { return *m_messageSender; }
  [[nodiscard]] auto socketHolder() const -> const SocketHolder& {
    return *dynamic_cast<SocketHolder*>(m_ioc.resolveService<ISocketHolder>().get());
  }

private:
  std::unique_ptr<MessageSender> m_messageSender;
  Ioc m_ioc;
};
}

BOOST_FIXTURE_TEST_SUITE(IocTests, Fixture)      // NOLINT
BOOST_AUTO_TEST_CASE(shouldRequestWriteOfHeader) // NOLINT
{
  const auto header         = MessageHeader::make(0, MessageType::Dummy, CT_QStringLiteral("."));
  constexpr auto headerSize = size_t{18};
  Fixture::messageSender().send(header);
  const auto& requestedWrites = Fixture::socketHolder().requestedWrites();
  BOOST_TEST(requestedWrites.size() == 1);           // NOLINT
  BOOST_TEST(requestedWrites.front() == headerSize); // NOLINT
}

BOOST_AUTO_TEST_CASE(shouldRequestWriteOfMessage) // NOLINT
{
  const auto json        = CT_QStringLiteral("{}");
  const auto jsonRawSize = qstringRawSize(json);
  const auto header      = MessageHeader::make(jsonRawSize, MessageType::Dummy, CT_QStringLiteral("."));
  const auto totalSize   = size_t{18} + jsonRawSize;
  Fixture::messageSender().send(header, json);
  const auto& requestedWrites = Fixture::socketHolder().requestedWrites();
  BOOST_TEST(requestedWrites.size() == 1);          // NOLINT
  BOOST_TEST(requestedWrites.front() == totalSize); // NOLINT
}

BOOST_AUTO_TEST_SUITE_END() // NOLINT

#include "MessageSenderTests.moc"
