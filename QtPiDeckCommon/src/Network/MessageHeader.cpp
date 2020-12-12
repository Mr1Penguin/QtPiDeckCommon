#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Network {
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
