#pragma once

#include <QDataStream>

namespace QtPiDeck::Network {
class DeckDataStream : public QDataStream {
public:
#if QT_VERSION_MAJOR == 6
    using OpenModeFlags = OpenMode;
#else
    using OpenModeFlags = QIODevice::OpenMode;
    using OpenModeFlag = QIODevice::OpenModeFlag;
#endif

    DeckDataStream() { setStreamParams(); }
    explicit DeckDataStream(QIODevice * device) : QDataStream(device) { setStreamParams(); }
    DeckDataStream(QByteArray * array, OpenModeFlags flags) : QDataStream(array, flags) { setStreamParams(); }
    DeckDataStream(const QByteArray & array) : QDataStream(array) { setStreamParams(); }

private:
    void setStreamParams() {
        setByteOrder(QDataStream::BigEndian);
        setVersion(QDataStream::Qt_5_11);
    }
};
}
