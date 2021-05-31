#pragma once

#include <functional>
#include <optional>

#include <QObject>

#include "Bus/Message.hpp"
#include "ServiceInterface.hpp"

namespace QtPiDeck::Services {
class Subscription {
public:
  Subscription() = default;
  explicit Subscription(QMetaObject::Connection connection) : m_connection(connection) {}
  Subscription(const Subscription&) = delete;
  Subscription(Subscription&& other) {
    m_connection = other.m_connection;
    other.m_connection.reset();
  }
  auto operator=(const Subscription&) -> Subscription& = delete;
  auto operator                                        =(Subscription&& other) -> Subscription& {
    m_connection = other.m_connection;
    other.m_connection.reset();
    return *this;
  };
  ~Subscription() { reset(); }

  void reset() noexcept {
    if (m_connection) {
      QObject::disconnect(*m_connection);
      m_connection.reset();
    }
  }

private:
  std::optional<QMetaObject::Connection> m_connection;
};

class IMessageBus : public ServiceInterface {
public:
  [[nodiscard]] virtual auto subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method)
      -> Subscription                                                        = 0;
  [[nodiscard]] virtual auto subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method,
                                       uint64_t messageType) -> Subscription = 0;
  virtual void unsubscribe(Subscription&)                                    = 0;
  virtual void sendMessage(Bus::Message)                                     = 0;
};

template<class TSubscriber, typename = std::enable_if_t<std::is_base_of_v<QObject, TSubscriber>>>
[[nodiscard]] auto subscribe(IMessageBus& bus, TSubscriber* context,
                             void (TSubscriber::*response)(const Bus::Message&)) noexcept -> Subscription {
  return bus.subscribe(context, [context, response](const Bus::Message& message) { (context->*response)(message); });
}

template<class TSubscriber, typename = std::enable_if_t<std::is_base_of_v<QObject, TSubscriber>>>
[[nodiscard]] auto subscribe(IMessageBus& bus, TSubscriber* context, void (TSubscriber::*response)(),
                             uint64_t messageType) noexcept -> Subscription {
  return bus.subscribe(
      context, [context, response](const Bus::Message& /*message*/) { (context->*response)(); }, messageType);
}

template<class TSubscriber, typename = std::enable_if_t<std::is_base_of_v<QObject, TSubscriber>>>
[[nodiscard]] auto subscribe(IMessageBus& bus, TSubscriber* context, void (TSubscriber::*response)(const Bus::Message&),
                             uint64_t messageType) noexcept -> Subscription {
  return bus.subscribe(
      context, [context, response](const Bus::Message& message) { (context->*response)(message); }, messageType);
}
}
