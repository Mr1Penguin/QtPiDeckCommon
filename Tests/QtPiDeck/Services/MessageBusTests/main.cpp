#include <future>

#include <QDataStream>
#include <QtTest>

#include "Services/MessageBus.hpp"

namespace QtPiDeck::Services::Tests {
using namespace std::chrono_literals;
class MessageBusTests : public QObject {
  Q_OBJECT // NOLINT

private slots:
  void init();
  void subscribe();
  void unsubscribe();
  void unsubscribeWithRAII();
  void subscribeFiltered();
  void subscribeTwoSubscribers();
  void subscribeMember();
  void subscribeMemberFiltered();
  void subscribeMemberFilteredWithType();
  void sendMessageWithPayload();

private: // NOLINT(readability-redundant-access-specifiers)
  std::unique_ptr<MessageBus> m_messageBus;
};

namespace {
constexpr auto time = 10ms;
}

void MessageBusTests::init() { m_messageBus = std::make_unique<MessageBus>(nullptr); }

void MessageBusTests::subscribe() {
  std::promise<uint64_t> promise;
  constexpr uint64_t messageType = 1234;
  auto future = promise.get_future();
  auto subscription =
      m_messageBus->subscribe(this, [&promise](const Bus::Message& mess) { promise.set_value(mess.messageType); });
  m_messageBus->sendMessage({messageType});
  QCOMPARE(future.wait_for(time), std::future_status::ready);
  QCOMPARE(future.get(), messageType);
}

void MessageBusTests::unsubscribe() {
  constexpr uint64_t messageType = 1234;
  std::promise<uint64_t> promise;
  auto subscription =
      m_messageBus->subscribe(this, [&promise](const Bus::Message& mess) { promise.set_value(mess.messageType); });
  m_messageBus->unsubscribe(subscription);
  m_messageBus->sendMessage({messageType});
  auto future = promise.get_future();
  QCOMPARE(future.wait_for(time), std::future_status::timeout);
}

void MessageBusTests::unsubscribeWithRAII() {
  constexpr uint64_t messageType = 1234;
  std::promise<uint64_t> promise;
  {
    auto subscription =
        m_messageBus->subscribe(this, [&promise](const Bus::Message& mess) { promise.set_value(mess.messageType); });
  }
  m_messageBus->sendMessage({messageType});
  auto future = promise.get_future();
  QCOMPARE(future.wait_for(time), std::future_status::timeout);
}

void MessageBusTests::subscribeFiltered() {
  std::promise<uint64_t> promise;
  constexpr uint64_t messageType = 1234;
  constexpr uint64_t messageType2 = 1235;
  auto future = promise.get_future();
  auto sub = m_messageBus->subscribe(
      this, [&promise](const Bus::Message& mess) { promise.set_value(mess.messageType); }, messageType);
  m_messageBus->sendMessage({messageType2});
  QCOMPARE(future.wait_for(time), std::future_status::timeout);
  m_messageBus->sendMessage({messageType});
  QCOMPARE(future.wait_for(time), std::future_status::ready);
  QCOMPARE(future.get(), messageType);
}

void MessageBusTests::subscribeTwoSubscribers() {
  std::promise<uint64_t> promise;
  std::promise<uint64_t> promise2;
  constexpr uint64_t messageType = 1234;
  auto future = promise.get_future();
  auto future2 = promise2.get_future();
  auto subscription =
      m_messageBus->subscribe(this, [&promise](const Bus::Message& mess) { promise.set_value(mess.messageType); });
  auto subscription2 =
      m_messageBus->subscribe(this, [&promise2](const Bus::Message& mess) { promise2.set_value(mess.messageType); });
  m_messageBus->sendMessage({messageType});
  QCOMPARE(future.wait_for(time), std::future_status::ready);
  QCOMPARE(future.get(), messageType);
  QCOMPARE(future2.wait_for(time), std::future_status::ready);
  QCOMPARE(future2.get(), messageType);
}

class Listener : public QObject {
  Q_OBJECT // NOLINT
public:
  Listener(uint64_t expectedMessageType) : m_expectedMessageType(expectedMessageType) {}

  void callMe() noexcept { m_promise.set_value(m_expectedMessageType); }

  void callMe(const Bus::Message& message) noexcept { m_promise.set_value(message.messageType); }

  auto getFuture() noexcept -> std::future<uint64_t> { return m_promise.get_future(); }

private:
  std::promise<uint64_t> m_promise{};
  const uint64_t m_expectedMessageType;
};

void MessageBusTests::subscribeMember() {
  constexpr uint64_t messageType = 1234;
  Listener listener(messageType);
  auto subscription = Services::subscribe(*m_messageBus, &listener, &Listener::callMe);
  auto future = listener.getFuture();
  m_messageBus->sendMessage({messageType});
  QCOMPARE(future.wait_for(time), std::future_status::ready);
  QCOMPARE(future.get(), messageType);
}

void MessageBusTests::subscribeMemberFiltered() {
  constexpr uint64_t messageType = 1234;
  Listener listener(messageType);
  auto sub =
      Services::subscribe(*m_messageBus, &listener, static_cast<void (Listener::*)()>(&Listener::callMe), messageType);
  auto future = listener.getFuture();
  m_messageBus->sendMessage({messageType});
  QCOMPARE(future.wait_for(time), std::future_status::ready);
  QCOMPARE(future.get(), messageType);
}

void MessageBusTests::subscribeMemberFilteredWithType() {
  constexpr uint64_t messageType = 1234;
  Listener listener(messageType);
  auto sub = Services::subscribe(*m_messageBus, &listener,
                                 static_cast<void (Listener::*)(const Bus::Message&)>(&Listener::callMe), messageType);
  auto future = listener.getFuture();
  m_messageBus->sendMessage({messageType});
  QCOMPARE(future.wait_for(time), std::future_status::ready);
  QCOMPARE(future.get(), messageType);
}

void MessageBusTests::sendMessageWithPayload() {
  constexpr uint64_t messageType = 1234;
  const QString payloadData = "Some random data";
  std::promise<std::pair<uint64_t, QByteArray>> messagePromise;
  auto messageFuture = messagePromise.get_future();
  auto subscription = m_messageBus->subscribe(this, [&messagePromise](const Bus::Message& mess) {
    messagePromise.set_value({mess.messageType, mess.payload});
  });
  m_messageBus->sendMessage({messageType, [&payloadData]() {
                               QByteArray qba;
                               QDataStream qds{&qba, QIODevice::WriteOnly};
                               qds << payloadData;
                               return qba;
                             }()});
  QCOMPARE(messageFuture.wait_for(time), std::future_status::ready);
  const auto message = messageFuture.get();
  QCOMPARE(message.first, messageType);
  const QString receivedPayloadData = [&message] {
    QDataStream qds{message.second};
    QString data;
    qds >> data;
    return data;
  }();
  QCOMPARE(receivedPayloadData, payloadData);
}
}

QTEST_APPLESS_MAIN(QtPiDeck::Services::Tests::MessageBusTests) // NOLINT

#include "main.moc"
