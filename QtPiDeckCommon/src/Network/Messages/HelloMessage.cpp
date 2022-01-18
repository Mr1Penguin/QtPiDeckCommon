#include "Network/Messages/HelloMessage.hpp"

namespace QtPiDeck::Network {
inline namespace Messages {
void Hello::read(QDataStream& stream) {
  stream >> interfaceVersion;
  stream >> qcharSize;
}

void Hello::write(QDataStream& stream) const {
  stream << interfaceVersion;
  stream << qcharSize;
}

auto QTPIDECKCOMMON_EXPORT operator<<(QDataStream& stream, const Hello& message) -> QDataStream& {
  message.write(stream);
  return stream;
}

auto QTPIDECKCOMMON_EXPORT operator>>(QDataStream& stream, Hello& message) -> QDataStream& {
  message.read(stream);
  return stream;
}
}
}
