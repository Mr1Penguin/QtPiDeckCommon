#pragma once

#include <cstdint>

#include <QDataStream>

#include "QtPiDeckCommon_global.hpp"

namespace QtPiDeck::Network {
enum class MessageId : uint32_t {
    Ping,
    Pong
};

struct MessageHeader {
    uint64_t dataSize;
    MessageId messageId;
};

QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const MessageHeader & header) -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, MessageHeader & header) -> QDataStream&;
#if (QT_VERSION == QTPI4_VERSION)
QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, MessageId & messageId) -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, MessageId & messageId) -> QDataStream&;
#endif
}
