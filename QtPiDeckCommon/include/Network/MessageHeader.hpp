#pragma once

#include <cstdint>

#include <QDataStream>
#if QT_VERSION_MAJOR == 6
#include <QString>
#endif

#include "QtPiDeckCommon_global.hpp"
#include "QtVersion.hpp"

namespace QtPiDeck::Network {
enum class MessageType : uint32_t { Ping, Pong };

struct MessageHeader {
  uint64_t dataSize;
  MessageType messageType;
  QString requestId;
};

inline auto getEmptyMessageHeader(MessageType messageType, QString requestId) {
  return MessageHeader{0, messageType, requestId};
}

QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const MessageHeader& header) -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, MessageHeader& header) -> QDataStream&;
#if (QT_VERSION == QTPI4_VERSION)
QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const MessageType& messageType) -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, MessageType& messageType) -> QDataStream&;
#endif
}
