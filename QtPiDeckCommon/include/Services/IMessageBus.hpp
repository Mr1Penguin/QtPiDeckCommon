#pragma once

#include <functional>

#include <QObject>

#include "Bus/Message.hpp"
#include "ServiceInterface.hpp"

namespace QtPiDeck::Services {
class IMessageBus : public ServiceInterface {
public:
    virtual auto subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method) -> QMetaObject::Connection = 0;
    virtual auto subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method, uint64_t messageType) -> QMetaObject::Connection = 0;
    virtual void unsubscribe(QMetaObject::Connection) = 0;
    virtual void sendMessage(Bus::Message) = 0;
};

template<class TSubscriber, typename = std::enable_if_t<std::is_base_of_v<QObject, TSubscriber>>>
void subscribe(IMessageBus& bus, TSubscriber* context, void(TSubscriber::* response)(const Bus::Message&)) noexcept {
    bus.subscribe(context, [context, response](const Bus::Message& message){ (context->*response)(message); });
}

template<class TSubscriber, typename = std::enable_if_t<std::is_base_of_v<QObject, TSubscriber>>>
void subscribe(IMessageBus& bus, TSubscriber* context, void(TSubscriber::* response)(), uint64_t messageType) noexcept {
    bus.subscribe(context, [context, response](const Bus::Message& /*message*/){ (context->*response)(); }, messageType);
}

template<class TSubscriber, typename = std::enable_if_t<std::is_base_of_v<QObject, TSubscriber>>>
void subscribe(IMessageBus& bus, TSubscriber* context, void(TSubscriber::* response)(const Bus::Message&), uint64_t messageType) noexcept {
    bus.subscribe(context, [context, response](const Bus::Message& message){ (context->*response)(message); }, messageType);
}
}

