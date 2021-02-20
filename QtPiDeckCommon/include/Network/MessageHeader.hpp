#pragma once

#include <cstdint>

#include <QDataStream>

#include "QtPiDeckCommon_global.hpp"
#include "QtVersion.hpp"

namespace QtPiDeck::Network {
enum class MessageType : uint32_t { Ping, Pong };

struct MessageHeader {
  uint64_t dataSize;
  MessageType messageType;
  QString RequestId;
};

QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const MessageHeader& header) -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, MessageHeader& header) -> QDataStream&;
#if (QT_VERSION == QTPI4_VERSION)
QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const MessageType& messageType) -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, MessageType& messageType) -> QDataStream&;
#endif
}
