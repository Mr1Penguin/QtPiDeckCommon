#include "HelloMessage.hpp"

#include "QtPiDeckCommon_global.hpp"

namespace QtPiDeck::Network {
inline namespace Messages {
auto QTPIDECKCOMMON_EXPORT prepareJsonString(const Hello& message) -> QString;
auto QTPIDECKCOMMON_EXPORT prepareMessage(const QString& json) -> Hello;
}
}
