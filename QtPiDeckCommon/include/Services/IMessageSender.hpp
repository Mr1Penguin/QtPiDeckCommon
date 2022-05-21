#pragma once

#include <QString>

#include "Network/MessageHeader.hpp"
#include "Services/ServiceInterface.hpp"

namespace QtPiDeck::Services {
class IMessageSender : public ServiceInterface {
public:
  virtual void send(const Network::MessageHeader& header, const QString& payload) = 0;
  virtual void send(const Network::MessageHeader& header)                         = 0;
};
}
