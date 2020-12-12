#pragma once

#include <functional>

#include <QObject>

#include "Bus/Message.hpp"
#include "ServiceInterface.hpp"

namespace QtPiDeck::Services {
class IMessageBus : public ServiceInterface {
public:
    virtual ~IMessageBus() {}

    virtual auto subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method) -> QMetaObject::Connection = 0;
    virtual auto subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method, uint64_t messageType) -> QMetaObject::Connection = 0;
    virtual void unsubscribe(QMetaObject::Connection) = 0;
    virtual void sendMessage(Bus::Message) = 0;
};
}

