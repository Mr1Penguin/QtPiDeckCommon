#include "Network/MessageHeader.hpp"

#include <iostream>
#include <qendian.h>

namespace QtPiDeck::Network {
template<class TNum>
using addOverload = std::enable_if_t<std::is_same_v<TNum, uint64_t> && !std::is_same_v<quint64, uint64_t>>;

template<class TNum, typename = addOverload<TNum>>
auto operator<<(QDataStream& str, const TNum& number) noexcept -> QDataStream& {
  str << static_cast<quint64>(number);
  return str;
}

template<class TNum, typename = addOverload<TNum>>
auto operator>>(QDataStream& str, TNum& number) noexcept -> QDataStream& {
  quint64 tmp{};
  str >> tmp;
  number = static_cast<uint64_t>(tmp);
  return str;
}

QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& stream, const MessageHeader& header) -> QDataStream& {
  stream << header.dataSize;
  stream << header.messageType;
  stream << header.requestId;
  return stream;
}

QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& stream, MessageHeader& header) -> QDataStream& {
  stream >> header.dataSize;
  stream >> header.messageType;
  stream >> header.requestId;
  return stream;
}

auto MessageHeader::make(uint64_t dataSize, MessageType messageType, QString requestId) -> MessageHeader {
  auto header        = MessageHeader{};
  header.dataSize    = dataSize;
  header.messageType = messageType;
  header.requestId   = std::move(requestId);
  return header;
}

#if (QT_VERSION == QTPI4_VERSION)
auto operator>>(QDataStream& stream, MessageType& messageType) noexcept -> QDataStream& {
  auto v = uint32_t{};
  stream >> v;
  messageType = static_cast<MessageType>(v);
  return stream;
}

auto operator<<(QDataStream& stream, const MessageType& messageType) noexcept -> QDataStream& {
  stream << static_cast<uint32_t>(messageType);
  return stream;
}
#endif
}
