#pragma once

#include <cstdint>

#include "Message.hpp"
#include "Network/MessageHeader.hpp"
#include "Utilities/ISerializable.hpp"

namespace QtPiDeck::Network {
inline namespace Messages {
/*
 * First message from client to server
 * If client data is valid AcceptConnection will be in response
 * If not: RejectConnection
 * If there is already connected client: RejectConnection
 */
struct QTPIDECKCOMMON_EXPORT Hello final : public Utilities::ISerializable, public Message {
  uint32_t interfaceVersion{s_currentInterfaceVersion};
  uint32_t qcharSize{s_qcharSize};

  // Message
  auto messageSize() const -> uint64_t final { return sizeof(interfaceVersion) + sizeof(qcharSize); }
  auto messageHeader(const QString& requestId) const -> MessageHeader final {
    return MessageHeader::make(messageSize(), MessageType::Hello, requestId);
  }

  // ISerializable
  void read(QDataStream& stream) final;
  void write(QDataStream& stream) const final;

  static inline uint32_t s_currentInterfaceVersion{20211202};
  static inline uint32_t s_qcharSize{sizeof(decltype(std::declval<QChar>().unicode()))};
};

auto QTPIDECKCOMMON_EXPORT operator<<(QDataStream& stream, const Hello& message) -> QDataStream&;
auto QTPIDECKCOMMON_EXPORT operator>>(QDataStream& stream, Hello& message) -> QDataStream&;
}
}
