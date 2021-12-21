#include "Network/MessageHeader.hpp"

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
