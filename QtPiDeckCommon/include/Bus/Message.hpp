#pragma once

#include <cstdint>

#include <QByteArray>

namespace QtPiDeck::Bus {
struct Message {
    const uint64_t messageType{};
    const QByteArray payload{};
};
}
