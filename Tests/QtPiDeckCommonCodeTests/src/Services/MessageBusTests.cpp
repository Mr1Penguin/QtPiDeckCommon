#include <QDataStream>
#include <QJsonObject>
#include <qdebug.h>

#include "BoostUnitTest.hpp"

#include <QDataStream>

#include "Network/DeckDataStream.hpp"
#include "Services/MessageBus.hpp"

#include "Utilities/Logging.hpp"

struct MessageBusFixture {
  std::unique_ptr<QtPiDeck::Services::MessageBus> messageBus{std::make_unique<QtPiDeck::Services::MessageBus>(nullptr)};
};

BOOST_FIXTURE_TEST_SUITE(MessageBusTests, MessageBusFixture) // NOLINT
using namespace QtPiDeck::Services;

namespace {
auto makeSetValue(uint64_t& ref) {
  return [&ref](const QtPiDeck::Bus::Message& mess) { ref = mess.messageType; };
}
}

BOOST_AUTO_TEST_CASE(subscribe) // NOLINT
{
  constexpr uint64_t messageType = 1234;
  uint64_t setMessageType        = 0;
  auto subscription              = messageBus->subscribe(messageBus.get(), makeSetValue(setMessageType));
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == messageType); // NOLINT
}

BOOST_AUTO_TEST_CASE(unsubscribe) // NOLINT
{
  constexpr uint64_t messageType = 1234;
  uint64_t setMessageType        = 0;
  auto subscription              = messageBus->subscribe(messageBus.get(), makeSetValue(setMessageType));
  messageBus->unsubscribe(subscription);
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == uint64_t{0}); // NOLINT
}

BOOST_AUTO_TEST_CASE(unsubscribeWithRAII) // NOLINT
{
  constexpr uint64_t messageType = 1234;
  uint64_t setMessageType        = 0;
  { auto subscription = messageBus->subscribe(messageBus.get(), makeSetValue(setMessageType)); }
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == uint64_t{0}); // NOLINT
}

BOOST_AUTO_TEST_CASE(subscribeFiltered) // NOLINT
{
  constexpr uint64_t messageType  = 1234;
  constexpr uint64_t messageType2 = 1235;
  uint64_t setMessageType         = 0;
  auto subscription               = messageBus->subscribe(messageBus.get(), makeSetValue(setMessageType), messageType);
  messageBus->sendMessage({messageType2});
  BOOST_TEST(setMessageType == uint64_t{0}); // NOLINT
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == messageType); // NOLINT
}

BOOST_AUTO_TEST_CASE(subscribeTwoSubscribers) // NOLINT
{
  constexpr uint64_t messageType = 1234;
  uint64_t setMessageType        = 0;
  uint64_t setMessageType2       = 0;

  auto subscription  = messageBus->subscribe(messageBus.get(), makeSetValue(setMessageType));
  auto subscription2 = messageBus->subscribe(messageBus.get(), makeSetValue(setMessageType2));
  messageBus->sendMessage({messageType});
  BOOST_TEST(setMessageType == messageType);  // NOLINT
  BOOST_TEST(setMessageType2 == messageType); // NOLINT
}

class Listener : public QObject {
  Q_OBJECT // NOLINT
public:
  Listener(uint64_t expectedMessageType) : m_expectedMessageType(expectedMessageType), setMessageType() {}

  void callMe() noexcept { setMessageType = m_expectedMessageType; };

  void callMe(const QtPiDeck::Bus::Message& message) noexcept { setMessageType = message.messageType; }

  [[nodiscard]] auto getMessageType() const noexcept -> uint64_t { return setMessageType; }

private:
  const uint64_t m_expectedMessageType;
  uint64_t setMessageType;
};

BOOST_AUTO_TEST_CASE(subscribeMember) // NOLINT
{
  constexpr uint64_t messageType = 1234;
  Listener listener(messageType);
  auto subscription = QtPiDeck::Services::subscribe(*messageBus, &listener, &Listener::callMe);
  messageBus->sendMessage({messageType});
  auto setMessageType = listener.getMessageType();
  BOOST_TEST(setMessageType == messageType); // NOLINT
}

BOOST_AUTO_TEST_CASE(subscribeMemberFiltered) // NOLINT
{
  constexpr uint64_t messageType = 1234;
  Listener listener(messageType);
  auto sub = QtPiDeck::Services::subscribe(*messageBus, &listener, static_cast<void (Listener::*)()>(&Listener::callMe),
                                           messageType);
  messageBus->sendMessage({messageType});
  auto setMessageType = listener.getMessageType();
  BOOST_TEST(setMessageType == messageType); // NOLINT
}

BOOST_AUTO_TEST_CASE(subscribeMemberFilteredWithType) // NOLINT
{
  constexpr uint64_t messageType = 1234;
  Listener listener(messageType);
  auto sub = QtPiDeck::Services::subscribe(
      *messageBus, &listener, static_cast<void (Listener::*)(const QtPiDeck::Bus::Message&)>(&Listener::callMe),
      messageType);
  messageBus->sendMessage({messageType});
  auto setMessageType = listener.getMessageType();
  BOOST_TEST(setMessageType == messageType); // NOLINT
}

BOOST_AUTO_TEST_CASE(sendMessageWithPayload) // NOLINT
{
  constexpr uint64_t messageType = 1234;
  const auto payloadData      = QString{R"({"text":"Some random data"})"};
  uint64_t setMessageType        = 0;
  QJsonDocument payload;

  auto subscription =
      messageBus->subscribe(messageBus.get(), [&payload, &setMessageType](const QtPiDeck::Bus::Message& mess) {
        setMessageType = mess.messageType;
        payload        = mess.payload;
      });
  messageBus->sendMessage({messageType, [&payloadData]() { return QJsonDocument::fromJson(payloadData.toUtf8()); }()});

  BOOST_TEST(setMessageType == messageType); // NOLINT
  const auto receivedPayloadData = QString{payload.toJson(decltype(payload)::JsonFormat::Compact) };
  BOOST_TEST(receivedPayloadData == payloadData); // NOLINT
}

BOOST_AUTO_TEST_SUITE_END() // NOLINT

#include "MessageBusTests.moc"
