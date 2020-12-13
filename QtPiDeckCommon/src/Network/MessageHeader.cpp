#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Network {

constexpr bool Int64AreDifferent = !std::is_same_v<quint64, uint64_t>;

template <typename = std::enable_if_t<Int64AreDifferent>>
QDataStream& operator<<(QDataStream& str, uint64_t number) {
    str << static_cast<quint64>(number);
    return str;
}

template <typename = std::enable_if_t<Int64AreDifferent>>
QDataStream& operator>>(QDataStream& str, uint64_t & number) {
    quint64 tmp;
    str >> tmp;
    number = static_cast<uint64_t>(tmp);
    return str;
}

QDataStream& operator<<(QDataStream& str, MessageHeader & header) {
    str << header.dataSize;
    str << header.messageId;
    return str;
}

QDataStream& operator>>(QDataStream& str, MessageHeader & header) {
    str >> header.dataSize;
    str >> header.messageId;
    return str;
}
}
