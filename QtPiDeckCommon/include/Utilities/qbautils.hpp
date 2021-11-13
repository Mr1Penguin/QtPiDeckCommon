#include <QByteArray>
#include <QDataStream>

namespace QtPiDeck::Utilities {
template<class T>
auto convert(const QByteArray& qba) -> T {
  T result;
  QDataStream stream{qba};
  stream >> result;
  return T;
}
}