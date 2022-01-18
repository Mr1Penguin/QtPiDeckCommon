#pragma once

#include <QString>

#include "Network/Messages/Message.hpp"
#include "Services/ServiceInterface.hpp"

namespace QtPiDeck::Services {
class IMessageSender : public ServiceInterface {
public:
  virtual void send(const Network::Messages::Message& message, const QString& id) = 0;
  virtual void send(const Network::MessageHeader& header)                         = 0;
};
}