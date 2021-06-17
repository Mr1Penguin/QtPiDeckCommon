#pragma once

#include <functional>
#include <optional>

#include <QObject>

#include "Bus/Message.hpp"
#include "ServiceInterface.hpp"
#include "Utilities/Connection.hpp"

namespace QtPiDeck::Services {
class IMessageBus : public ServiceInterface {
public:
  [[nodiscard]] virtual auto subscribe(QObject* context,
                                       const std::function<void(const Bus::Message&)>& method) noexcept
      -> Utilities::Connection                                                                 = 0;
  [[nodiscard]] virtual auto subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method,
                                       uint64_t messageType) noexcept -> Utilities::Connection = 0;
  virtual void unsubscribe(Utilities::Connection&) noexcept                                    = 0;
  virtual void sendMessage(Bus::Message) noexcept                                              = 0;
};

template<class TSubscriber, typename = std::enable_if_t<std::is_base_of_v<QObject, TSubscriber>>>
[[nodiscard]] auto subscribe(IMessageBus& bus, TSubscriber* context,
                             void (TSubscriber::*response)(const Bus::Message&)) noexcept -> Utilities::Connection {

#if defined(__cpp_lib_bind_front)
  return bus.subscribe(context, std::bind_front(response, context));
#else
  return bus.subscribe(context,
                       [context, response](const Bus::Message& message) { std::invoke(response, context, message); });
#endif
}

template<class TSubscriber, typename = std::enable_if_t<std::is_base_of_v<QObject, TSubscriber>>>
[[nodiscard]] auto subscribe(IMessageBus& bus, TSubscriber* context, void (TSubscriber::*response)(),
                             uint64_t messageType) noexcept -> Utilities::Connection {
  return bus.subscribe(
      context, [context, response](const Bus::Message& /*message*/) { std::invoke(response, context); }, messageType);
}

template<class TSubscriber, typename = std::enable_if_t<std::is_base_of_v<QObject, TSubscriber>>>
[[nodiscard]] auto subscribe(IMessageBus& bus, TSubscriber* context, void (TSubscriber::*response)(const Bus::Message&),
                             uint64_t messageType) noexcept -> Utilities::Connection {
#if defined(__cpp_lib_bind_front)
  return bus.subscribe(context, std::bind_front(response, context), messageType);
#else
  return bus.subscribe(
      context, [context, response](const Bus::Message& message) { std::invoke(response, context, message); },
      messageType);
#endif
}
}
