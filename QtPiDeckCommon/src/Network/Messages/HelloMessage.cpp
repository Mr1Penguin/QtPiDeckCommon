#include "Network/Messages/HelloMessage.hpp"

namespace QtPiDeck::Network {
inline namespace Messages {
auto operator<<(QDataStream& str, const Hello& header) -> QDataStream& {
  str << header.interfaceVersion;
  str << header.qcharSize;
  return str;
}

auto operator>>(QDataStream& str, Hello& header) -> QDataStream& {
  str >> header.interfaceVersion;
  str >> header.qcharSize;
  return str;
}
}
}
