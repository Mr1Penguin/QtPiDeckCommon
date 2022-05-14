#pragma once

#include <cstdint>
#include <tuple>

#include <QJsonDocument>

namespace QtPiDeck::Bus {
struct Message {
    const uint64_t messageType{};
    const QJsonDocument payload{};
};
}
