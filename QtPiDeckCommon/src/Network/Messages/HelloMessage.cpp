#include "Network/Messages/HelloMessage.hpp"

namespace QtPiDeck::Network {
inline namespace Messages {
auto operator<<(QDataStream& str, const Hello& message) -> QDataStream& {
  str << message.interfaceVersion;
  str << message.qcharSize;
  return str;
}

auto operator>>(QDataStream& str, Hello& message) -> QDataStream& {
  str >> message.interfaceVersion;
  str >> message.qcharSize;
  return str;
}
}
}
