#pragma once

#include <cstdint>

namespace QtPiDeck::Bus {
enum Dummy : uint64_t {DummyId = 0};
enum SocketMessages : uint64_t { SocketChanged = 0x1000 };
}