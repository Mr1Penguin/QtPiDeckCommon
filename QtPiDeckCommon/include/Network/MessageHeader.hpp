#pragma once

#include <array>
#include <cstdint>

#include <QDataStream>
#if QT_VERSION_MAJOR == 6
#include <QString>
#endif

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

struct QTPIDECKCOMMON_EXPORT MessageHeader final : public Utilities::ISerializable {
  uint64_t dataSize;
  MessageType messageType;
  QString requestId;

  // ISerializable
  void read(QDataStream& stream) final;
  void write(QDataStream& stream) const final;

  static auto make(uint64_t dataSize, MessageType messageType, QString requestId) -> MessageHeader;
};

#if (QT_VERSION == QTPI4_VERSION)
QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const MessageType& messageType) noexcept -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, MessageType& messageType) noexcept -> QDataStream&;
#endif
}
