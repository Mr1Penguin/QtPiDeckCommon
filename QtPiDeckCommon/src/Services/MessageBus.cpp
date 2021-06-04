#include "Services/MessageBus.hpp"

#include <QMetaMethod>

namespace QtPiDeck::Services {
auto MessageBus::subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method) noexcept -> Subscription {
  return Subscription{connect(this, &MessageBus::newMessage, context, method)};
}

namespace {
auto filtered(const std::function<void(const Bus::Message&)>& method, const uint64_t messageType) noexcept {
  return [=](const Bus::Message& message) {
    if (message.messageType == messageType) {
      method(message);
    }
  };
}
}

auto MessageBus::subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method,
                           uint64_t messageType) noexcept -> Subscription {
  return Subscription{connect(this, &MessageBus::newMessage, context, filtered(method, messageType))};
}

void MessageBus::unsubscribe(Subscription& subscription) noexcept { subscription.reset(); }

void MessageBus::sendMessage(Bus::Message message) noexcept { emit newMessage(message); }
}
