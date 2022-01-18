#include "BoostUnitTest.hpp"

#include "Network/MessageReceiver.hpp"
#include "Services/ISocketHolder.hpp"

CT_BOOST_AUTO_TEST_SUITE(MessageReceiverTests)

using namespace QtPiDeck::Network;
using namespace QtPiDeck::Services;

class Device final : public QIODevice {
  Q_OBJECT // NOLINT
protected:
  // QIODevice
  auto readData(char* /*data*/, qint64 /*maxlen*/) -> qint64 final {
    throw std::logic_error("The method or operation is not implemented.");
  }

  auto writeData(const char* /*data*/, qint64 /*len*/) -> qint64 final {
    throw std::logic_error("The method or operation is not implemented.");
  }

public:
};

class SocketHolder final : public ISocketHolder {
public:
  // ISocketHolder
  void requestWrite(const QByteArray& /*data*/) final {}
  void setSocket(QIODevice* /*socket*/) final {}
  auto socket() -> QIODevice* final { return &m_device; }

private:
  Device m_device;
};

CT_BOOST_AUTO_TEST_CASE(ShouldCreateObject) {
  const auto receiver = MessageReceiver{std::make_shared<SocketHolder>(), nullptr, nullptr};
}

CT_BOOST_AUTO_TEST_SUITE_END()

#include "MessageReceiverTests.moc"