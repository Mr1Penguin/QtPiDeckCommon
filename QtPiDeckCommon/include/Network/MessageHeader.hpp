#pragma once

#include <array>
#include <cstdint>

#include <QDataStream>
#include <QString>

#include "QtPiDeckCommon_global.hpp"
#include "QtVersion.hpp"
#include "Utilities/ISerializable.hpp"

namespace QtPiDeck::Network {
enum class MessageType : uint32_t {
  Dummy,
  Ping,
  Pong,
  Hello,
  AcceptConnection,
  RejectConnection,
  Reserved = std::numeric_limits<uint32_t>::max()
};
constexpr inline std::array messageTypeNames = {"Dummy",           "Ping", "Pong", "Hello", "AcceptConnection",
                                                "RejectConnection"};

constexpr inline auto getMessageTypeName(MessageType type) -> const char* {
  return messageTypeNames.at(static_cast<uint32_t>(type));
}

struct MessageHeader {
  uint64_t dataSize;
  MessageType messageType;
  QString requestId;

  static auto make(uint64_t dataSize, MessageType messageType, QString requestId) -> MessageHeader;
};

QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& stream, const MessageHeader& header) -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& stream, MessageHeader& header) -> QDataStream&;

#if (QT_VERSION == QTPI4_VERSION)
QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& stream, const MessageType& messageType) noexcept -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& stream, MessageType& messageType) noexcept -> QDataStream&;
#endif
}
