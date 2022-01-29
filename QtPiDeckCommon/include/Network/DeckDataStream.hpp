#pragma once

#include <QDataStream>

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

template<class TNum>
using addOverload = std::enable_if_t<std::is_same_v<TNum, uint64_t> && !std::is_same_v<quint64, uint64_t>>;

template<class TNum, typename = addOverload<TNum>>
auto operator<<(QDataStream& str, const TNum& number) noexcept -> QDataStream& {
  str << static_cast<quint64>(number);
  return str;
}

template<class TNum, typename = addOverload<TNum>>
auto operator>>(QDataStream& str, TNum& number) noexcept -> QDataStream& {
  quint64 tmp{};
  str >> tmp;
  number = static_cast<uint64_t>(tmp);
  return str;
}
}
