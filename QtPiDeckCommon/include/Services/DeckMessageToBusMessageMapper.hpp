#pragma once

#include <optional>

#include "Bus/CommonMessageIds.hpp"
#include "Services/IDeckMessageToBusMessageMapper.hpp"

namespace QtPiDeck::Services {
/*
 * Deck message type to bus message type converter
 * Mapper with some default values used in common code
 * Derived classes should implement protected getType method
 * If getType returns nullopt default type will be return(if available)
 */
class DeckMessageToBusMessageMapper : public IDeckMessageToBusMessageMapper {
public:
  [[nodiscard]] auto getBusMessageType(Network::MessageType deckType) const
      -> std::optional<decltype(Bus::Message::messageType)> final {
    if (const auto typeFromDerived = getType(deckType); typeFromDerived.has_value()) {
      return typeFromDerived;
    }

    switch (deckType) {
    case Network::MessageType::Dummy:
      return Bus::Dummy::DummyId;
    default:
      return std::nullopt;
    }
  }

protected:
  [[nodiscard]] virtual auto getType(Network::MessageType /*deckType*/) const
      -> std::optional<decltype(Bus::Message::messageType)> {
    return std::nullopt;
  };
};
}
