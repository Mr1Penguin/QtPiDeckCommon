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

struct QTPIDECKCOMMON_EXPORT MessageHeader final : public Utilities::ISerializable {
  [[nodiscard]] auto dataSize() const -> uint64_t { return m_dataSize; }
  void dataSize(uint64_t value) { m_dataSize = value; }
  [[nodiscard]] auto messageType() const -> MessageType { return m_messageType; }
  void messageType(MessageType value) { m_messageType = value; }
  [[nodiscard]] auto requestId() const -> QString { return m_requestId; }
  void requestId(QString value) { m_requestId = std::move(value); }

  // ISerializable
  void read(QDataStream& stream) final;
  void write(QDataStream& stream) const final;

  static auto make(uint64_t dataSize, MessageType messageType, QString requestId) -> MessageHeader;

private:
  uint64_t m_dataSize;
  MessageType m_messageType;
  QString m_requestId;
};

#if (QT_VERSION == QTPI4_VERSION)
QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const MessageType& messageType) noexcept -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, MessageType& messageType) noexcept -> QDataStream&;
#endif
}
