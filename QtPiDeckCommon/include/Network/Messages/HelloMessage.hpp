#pragma once

#include <cstdint>

#include <QDataStream>

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

  static inline uint32_t s_currentInterfaceVersion{20211202};
  static inline uint32_t s_qcharSize{sizeof(decltype(std::declval<QChar>().unicode()))};

  constexpr auto getMessageSize() const -> uint64_t { return sizeof(interfaceVersion) + sizeof(qcharSize); }
  static auto getMessageHeader(QString requestId, const Hello& message) -> MessageHeader {
    return MessageHeader{message.getMessageSize(), MessageType::Hello, std::move(requestId)};
  }
};

QTPIDECKCOMMON_EXPORT auto operator<<(QDataStream& str, const Hello& header) -> QDataStream&;
QTPIDECKCOMMON_EXPORT auto operator>>(QDataStream& str, Hello& header) -> QDataStream&;
}
}
