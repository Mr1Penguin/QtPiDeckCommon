#pragma once

#include <cstdint>

#include <QByteArray>

#include "Utilities/QBAutils.hpp"

namespace QtPiDeck::Bus {
struct Message {
  uint64_t messageType{};
  QByteArray payload{};
};

template<class T>
auto convertPayload(const Message& message) -> T {
  return Utilities::convert<T>(message.payload);
}

namespace detail {
template<class TArg>    
void addData(QDataStream& stream, TArg&& arg) {
  stream << arg;
}
}

template<class... TArgs>
auto createMessage(uint64_t messageType, TArgs&&... args) -> Message {
  auto message = Message{messageType, {}};
  if constexpr (sizeof...(TArgs) != 0) {
    auto stream = QDataStream{&message.payload, QIODevice::WriteOnly};
    (detail::addData(stream, std::forward<TArgs>(args)), ...);
  }

  return message;
}
}
