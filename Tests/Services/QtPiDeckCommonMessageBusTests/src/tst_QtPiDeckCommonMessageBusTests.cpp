#include <future>

#include <QtTest>

#include "Services/MessageBus.hpp"

namespace QtPiDeck::Services::Tests {
using namespace std::chrono_literals;
class MessageBus : public QObject {
    Q_OBJECT // NOLINT

private slots:
    void init();
    void subscribe();
    void unsubscribe();
    void subscribeFiltered();
    void subscribeTwoSubscribers();

private:
    std::unique_ptr<Services::MessageBus> m_messageBus;
};

void MessageBus::init() {
    m_messageBus = std::make_unique<Services::MessageBus>(nullptr);
}

void MessageBus::subscribe() {
    std::promise<uint64_t> promise;
    constexpr uint64_t messageType = 1234;
    auto future = promise.get_future();
    m_messageBus->subscribe(this, [&promise](const Bus::Message& mess) { promise.set_value(mess.messageType); });
    m_messageBus->sendMessage({messageType});
    QCOMPARE(future.wait_for(100ms), std::future_status::ready);
    QCOMPARE(future.get(), messageType);
}

void MessageBus::unsubscribe() {
    constexpr uint64_t messageType = 1234;
    std::promise<uint64_t> promise;
    auto connection = m_messageBus->subscribe(this, [&promise](const Bus::Message& mess) { promise.set_value(mess.messageType); });
    m_messageBus->unsubscribe(connection);
    m_messageBus->newMessage({messageType});
    auto future = promise.get_future();
    QCOMPARE(future.wait_for(100ms), std::future_status::timeout);
}

void MessageBus::subscribeFiltered() {
    std::promise<uint64_t> promise;
    constexpr uint64_t messageType = 1234;
    constexpr uint64_t messageType2 = 1235;
    auto future = promise.get_future();
    m_messageBus->subscribe(this, [&promise](const Bus::Message& mess) { promise.set_value(mess.messageType); }, 1234);
    m_messageBus->sendMessage({messageType2});
    QCOMPARE(future.wait_for(100ms), std::future_status::timeout);
    m_messageBus->sendMessage({messageType});
    QCOMPARE(future.wait_for(100ms), std::future_status::ready);
    QCOMPARE(future.get(), messageType);
}

void MessageBus::subscribeTwoSubscribers() {
    std::promise<uint64_t> promise;
    std::promise<uint64_t> promise2;
    constexpr uint64_t messageType = 1234;
    auto future = promise.get_future();
    auto future2 = promise2.get_future();
    m_messageBus->subscribe(this, [&promise](const Bus::Message& mess) { promise.set_value(mess.messageType); });
    m_messageBus->subscribe(this, [&promise2](const Bus::Message& mess) { promise2.set_value(mess.messageType); });
    m_messageBus->sendMessage({messageType});
    QCOMPARE(future.wait_for(100ms), std::future_status::ready);
    QCOMPARE(future.get(), messageType);
    QCOMPARE(future2.wait_for(100ms), std::future_status::ready);
    QCOMPARE(future2.get(), messageType);
}
}

QTEST_APPLESS_MAIN(QtPiDeck::Services::Tests::MessageBus) // NOLINT

#include "tst_QtPiDeckCommonMessageBusTests.moc"
