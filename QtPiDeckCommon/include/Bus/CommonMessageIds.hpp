#pragma once

#include <cstdint>

namespace QtPiDeck::Bus {
enum SocketMessages : uint64_t { SocketChanged = 0x1000 };
}