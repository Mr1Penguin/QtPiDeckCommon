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

QTPIDECKCOMMON_EXPORT QDataStream& operator<<(QDataStream& str, MessageHeader & header);
QTPIDECKCOMMON_EXPORT QDataStream& operator>>(QDataStream& str, MessageHeader & header);
}
