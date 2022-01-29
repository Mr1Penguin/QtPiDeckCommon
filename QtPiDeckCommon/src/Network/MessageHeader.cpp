#include "Network/MessageHeader.hpp"

#include "Network/DeckDataStream.hpp"

namespace QtPiDeck::Network {
void MessageHeader::read(QDataStream& stream) {
  stream >> dataSize;
  stream >> messageType;
  stream >> requestId;
}

void MessageHeader::write(QDataStream& stream) const {
  stream << dataSize;
  stream << messageType;
  stream << requestId;
}

auto MessageHeader::make(uint64_t dataSize, MessageType messageType, QString requestId) -> MessageHeader {
  auto header        = MessageHeader{};
  header.dataSize    = dataSize;
  header.messageType = messageType;
  header.requestId   = std::move(requestId);
  return header;
}

#if (QT_VERSION == QTPI4_VERSION)
auto operator>>(QDataStream& str, MessageType& messageType) noexcept -> QDataStream& {
  auto v = uint32_t{};
  str >> v;
  messageType = static_cast<MessageType>(v);
  return str;
}

auto operator<<(QDataStream& str, const MessageType& messageType) noexcept -> QDataStream& {
  str << static_cast<uint32_t>(messageType);
  return str;
}
#endif
}
