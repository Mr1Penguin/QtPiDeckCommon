#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Network {

template<class TNum>
using addOverload = std::enable_if_t<std::is_same_v<TNum, uint64_t> && !std::is_same_v<quint64, uint64_t>>;

template<class TNum, typename = addOverload<TNum>>
auto operator<<(QDataStream& str, const TNum& number) -> QDataStream& {
  str << static_cast<quint64>(number);
  return str;
}

template<class TNum, typename = addOverload<TNum>>
auto operator>>(QDataStream& str, TNum& number) -> QDataStream& {
  quint64 tmp{};
  str >> tmp;
  number = static_cast<uint64_t>(tmp);
  return str;
}

auto operator<<(QDataStream& str, const MessageHeader& header) -> QDataStream& {
  str << header.dataSize;
  str << header.messageType;
  str << header.requestId;
  return str;
}

auto operator>>(QDataStream& str, MessageHeader& header) -> QDataStream& {
  str >> header.dataSize;
  str >> header.messageType;
  str >> header.requestId;
  return str;
}

#ifdef __GNUC__
#if (__GNUC__ == 8 && __GNUC_MINOR__ <= 3) || __GNUC__ < 8
auto operator>>(QDataStream& str, MessageType& messageType) -> QDataStream& {
  uint32_t v;
  str >> v;
  messageType = static_cast<MessageType>(v);
  return str;
}

auto operator<<(QDataStream& str, const MessageType& messageType) -> QDataStream& {
  str << static_cast<uint32_t>(messageType);
  return str;
}
#endif
#endif
}
