#pragma once

#include <cstdint>
#include <tuple>

#include <QJsonDocument>

namespace QtPiDeck::Bus {
struct Message {
  uint64_t messageType{};
  QJsonDocument payload{};
};
}
