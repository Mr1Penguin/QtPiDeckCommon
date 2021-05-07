#if defined(_MSC_VER) && defined(__clang__) && defined(__INTELLISENSE__)
#pragma push_macro("__clang__")
#undef __clang__
#include <boost/test/unit_test.hpp>
#pragma pop_macro("__clang__")
#else
#include <boost/test/unit_test.hpp>
#endif
#include <QDataStream>

#include "Services/MessageBus.hpp"
#include "Network/DeckDataStream.hpp"

struct MessageBusFixture {
  MessageBusFixture() : messageBus(std::make_unique<QtPiDeck::Services::MessageBus>(nullptr)) {}

  std::unique_ptr<QtPiDeck::Services::MessageBus> messageBus;
};

BOOST_FIXTURE_TEST_SUITE(MessageBusTests, MessageBusFixture)
using namespace QtPiDeck::Services;

BOOST_AUTO_TEST_CASE(subscribe)
{
  constexpr uint64_t messageType = 1234;
  uint64_t setMessageType        = 0;
  auto subscription              = messageBus->subscribe(
      messageBus.get(), [&setMessageType](const QtPiDeck::Bus::Message& mess) { setMessageType = mess.messageType; });
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == messageType);
}

BOOST_AUTO_TEST_CASE(unsubscribe)
{
  constexpr uint64_t messageType = 1234;
  uint64_t setMessageType        = 0;
  auto subscription              = messageBus->subscribe(
      messageBus.get(), [&setMessageType](const QtPiDeck::Bus::Message& mess) { setMessageType = mess.messageType; });
  messageBus->unsubscribe(subscription);
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == 0);
}

BOOST_AUTO_TEST_CASE(unsubscribeWithRAII)
{
  constexpr uint64_t messageType = 1234;
  uint64_t setMessageType        = 0;
  {
    auto subscription = messageBus->subscribe(
        messageBus.get(), [&setMessageType](const QtPiDeck::Bus::Message& mess) { setMessageType = mess.messageType; });
  }
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == 0);
}

BOOST_AUTO_TEST_CASE(subscribeFiltered)
{
  constexpr uint64_t messageType  = 1234;
  constexpr uint64_t messageType2 = 1235;
  uint64_t setMessageType         = 0;
  auto subscription               = messageBus->subscribe(
      messageBus.get(), [&setMessageType](const QtPiDeck::Bus::Message& mess) { setMessageType = mess.messageType; },
      messageType);
  messageBus->sendMessage({messageType2});
  BOOST_TEST(setMessageType == 0);
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == messageType);
}

BOOST_AUTO_TEST_CASE(subscribeTwoSubscribers)
{
  constexpr uint64_t messageType = 1234;
  uint64_t setMessageType        = 0;
  uint64_t setMessageType2       = 0;

  auto subscription = messageBus->subscribe(
      messageBus.get(), [&setMessageType](const QtPiDeck::Bus::Message& mess) { setMessageType = mess.messageType; });
  auto subscription2 = messageBus->subscribe(
      messageBus.get(), [&setMessageType2](const QtPiDeck::Bus::Message& mess) { setMessageType2 = mess.messageType; });
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == messageType);
  BOOST_TEST(setMessageType2 == messageType);
}

class Listener : public QObject {
  Q_OBJECT // NOLINT
public:
  Listener(uint64_t expectedMessageType) : m_expectedMessageType(expectedMessageType), setMessageType() {}

  void callMe() noexcept { setMessageType = m_expectedMessageType; };

  void callMe(const QtPiDeck::Bus::Message& message) noexcept { setMessageType = message.messageType; }

  auto getMessageType() noexcept -> uint64_t { return setMessageType; }

private:
  const uint64_t m_expectedMessageType;
  uint64_t setMessageType;
};

BOOST_AUTO_TEST_CASE(subscribeMember)
{
  constexpr uint64_t messageType = 1234;
  Listener listener(messageType);
  auto subscription = QtPiDeck::Services::subscribe(*messageBus, &listener, &Listener::callMe);
  messageBus->sendMessage({messageType});
  auto setMessageType = listener.getMessageType();
  BOOST_TEST(setMessageType == messageType);
}

BOOST_AUTO_TEST_CASE(subscribeMemberFiltered)
{
  constexpr uint64_t messageType = 1234;
  Listener listener(messageType);
  auto sub = QtPiDeck::Services::subscribe(*messageBus, &listener, static_cast<void (Listener::*)()>(&Listener::callMe),
                                           messageType);
  messageBus->sendMessage({messageType});
  auto setMessageType = listener.getMessageType();
  BOOST_TEST(setMessageType == messageType);
}

BOOST_AUTO_TEST_CASE(subscribeMemberFilteredWithType)
{
  constexpr uint64_t messageType = 1234;
  Listener listener(messageType);
  auto sub = QtPiDeck::Services::subscribe(
      *messageBus, &listener, static_cast<void (Listener::*)(const QtPiDeck::Bus::Message&)>(&Listener::callMe),
      messageType);
  messageBus->sendMessage({messageType});
  auto setMessageType = listener.getMessageType();
  BOOST_TEST(setMessageType == messageType);
}

BOOST_AUTO_TEST_CASE(sendMessageWithPayload)
{
  constexpr uint64_t messageType = 1234;
  const QString payloadData      = "Some random data";
  uint64_t setMessageType        = 0;
  QByteArray payload;

  auto subscription =
      messageBus->subscribe(messageBus.get(), [&payload, &setMessageType](const QtPiDeck::Bus::Message& mess) {
        setMessageType = mess.messageType;
        payload        = mess.payload;
      });
  messageBus->sendMessage({messageType, [&payloadData]() {
                             QByteArray qba;
                             QDataStream qds{&qba, QtPiDeck::Network::DeckDataStream::OpenModeFlag::WriteOnly};
                             qds << payloadData;
                             return qba;
                           }()});

  BOOST_TEST(setMessageType == messageType);
  const QString receivedPayloadData = [&payload] {
    QDataStream qds{payload};
    QString data;
    qds >> data;
    return data;
  }();
  BOOST_TEST(receivedPayloadData == payloadData);
}

BOOST_AUTO_TEST_SUITE_END()

#ifndef __INTELLISENSE__
#include "MessageBusTests.moc"
#endif
