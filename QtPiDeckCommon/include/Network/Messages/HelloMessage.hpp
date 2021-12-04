#pragma once

#include <cstdint>

#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Network {
inline namespace Messages {
/*
 * First message from client to server
 * If client data is valid AcceptConnection will be in response
 * If not: RejectConnection
 */
struct Hello {
  uint32_t interfaceVersion{s_currentInterfaceVersion};
  uint32_t qcharSize{s_qcharSize};

  auto messageSize() const -> uint64_t { return sizeof(interfaceVersion) + sizeof(qcharSize); }
  auto messageHeader(const QString& requestId) const -> MessageHeader {
    return MessageHeader{messageSize(), MessageType::Hello, requestId};
  }

  static inline uint32_t s_currentInterfaceVersion{20211202};
  static inline uint32_t s_qcharSize{sizeof(decltype(std::declval<QChar>().unicode()))};
};

QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const Hello& message) -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, Hello& message) -> QDataStream&;
}
}
