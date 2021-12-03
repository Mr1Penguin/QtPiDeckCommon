#pragma once

#include <QDataStream>

#include "MessageTraits.hpp"

namespace QtPiDeck::Network {
class DeckDataStream : public QDataStream {
public:
#if QT_VERSION_MAJOR == 5
    using OpenMode = QIODevice::OpenMode;
    using OpenModeFlag = QIODevice::OpenModeFlag;
#endif

    DeckDataStream() { setStreamParams(); }
    explicit DeckDataStream(QIODevice * device) : QDataStream(device) { setStreamParams(); }
    DeckDataStream(QByteArray * array, OpenMode flags) : QDataStream(array, flags) { setStreamParams(); }
    DeckDataStream(const QByteArray & array) : QDataStream(array) { setStreamParams(); }

private:
    void setStreamParams() {
        setByteOrder(QDataStream::BigEndian);
        setVersion(QDataStream::Qt_5_11);
    }
};
}
