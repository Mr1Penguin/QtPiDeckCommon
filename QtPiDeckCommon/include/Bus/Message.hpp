#pragma once

#include <cstdint>

#include <QByteArray>

#include "Utilities/qbautils.hpp"

namespace QtPiDeck::Bus {
struct Message {
    const uint64_t messageType{};
    const QByteArray payload{};
};

template<class T>
auto convertPayload(const Message& message) -> T {
  return Utilities::convert<T>(message.payload);
}
}
