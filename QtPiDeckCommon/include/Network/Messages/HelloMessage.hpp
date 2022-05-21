#pragma once

#include <cstdint>

#include <QChar>

namespace QtPiDeck::Network {
inline namespace Messages {
/*
 * First message from client to server
 * If client data is valid AcceptConnection will be in response
 * If not: RejectConnection
 * If there is already connected client: RejectConnection
 */
struct Hello {
  uint32_t interfaceVersion{s_currentInterfaceVersion};
  uint32_t qcharSize{s_qcharSize};

  static inline constexpr uint32_t s_currentInterfaceVersion{20211202};
  static inline constexpr uint32_t s_qcharSize{sizeof(decltype(std::declval<QChar>().unicode()))};
};
}
}


// prepareHeader(Message) -> MessageHeader
// convertMessage(Message) -> json
