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
enum class MessageType : uint32_t { Ping, Pong, Hello, AcceptConnection, RejectConnection };
constexpr inline std::array messageTypeNames = {"Ping", "Pong", "Hello", "AcceptConnection", "RejectConnection"};

struct QTPIDECKCOMMON_EXPORT MessageHeader final : public Utilities::ISerializable {
  uint64_t dataSize;
  MessageType messageType;
  QString requestId;

  // ISerializable
  void read(QDataStream& stream) final;
  void write(QDataStream& stream) const final;
};

#if (QT_VERSION == QTPI4_VERSION)
QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const MessageType& messageType) noexcept -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, MessageType& messageType) noexcept -> QDataStream&;
#endif
}
