#pragma once

#include <optional>

#include "Bus/Message.hpp"
#include "Network/MessageHeader.hpp"
#include "ServiceInterface.hpp"

namespace QtPiDeck::Services {
class IDeckMessageToBusMessageMapper : public ServiceInterface {
public:
  [[nodiscard]] virtual auto getBusMessageType(Network::MessageType) const -> std::optional<decltype(Bus::Message::messageType)> = 0;
};
}