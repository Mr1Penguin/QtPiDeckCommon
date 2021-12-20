#pragma once

#include <cstdint>
#include <tuple>

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

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

template<class... TArgs>
auto convertPayload(const Message& message) -> std::tuple<TArgs...> {
  return Utilities::convert<TArgs...>(message.payload);
}

template<class... TArgs>
auto createMessage(uint64_t messageType, TArgs&&... args) -> Message {
  auto message = Message{messageType, {}};
  if constexpr (sizeof...(TArgs) != 0) {
    auto stream = QDataStream{&message.payload, QIODevice::WriteOnly};
    (stream << ... << args);
  }

  return message;
}
}
