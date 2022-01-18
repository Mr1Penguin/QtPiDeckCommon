#pragma once

#include <cstdint>

#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Network {
inline namespace Messages {
struct Message {
  virtual ~Message() = default;

  virtual auto messageSize() const -> uint64_t = 0;
  virtual auto messageHeader(const QString& requestId) const -> MessageHeader = 0;
};
}
}