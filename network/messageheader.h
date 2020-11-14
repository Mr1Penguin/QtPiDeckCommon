#pragma once

#include <cstdint>

enum class MessageId : uint32_t {
    Ping,
    Pong
};

//force 64bit alignment due to difference in arch of client and server
struct alignas(64) MessageHeader {
    uint64_t dataSize;
    MessageId messageId;
};
