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
  MessageHeader() : dataSize(uint64_t{}), messageType(MessageType{}), requestId(QString{}) {}
  MessageHeader(uint64_t dataSizeArg, MessageType messageTypeArg, QString requestIdArg)
      : dataSize(dataSizeArg), messageType(messageTypeArg), requestId(requestIdArg) {}

  MessageHeader(const MessageHeader&) noexcept = default;
  MessageHeader(MessageHeader&&) noexcept      = default;
  auto operator=(const MessageHeader&) noexcept -> MessageHeader& = default;
  auto operator=(MessageHeader&&) noexcept -> MessageHeader& = default;

  ~MessageHeader() noexcept = default;

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
