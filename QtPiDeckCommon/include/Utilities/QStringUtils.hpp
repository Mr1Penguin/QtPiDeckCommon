#include <cstdint>

#include <QString>

namespace QtPiDeck::Utilities {
inline auto qstringRawSize(const QString& string) -> std::size_t {
  constexpr auto qstringHeaderSize = std::size_t{4};
  constexpr auto charSize          = std::size_t{sizeof(decltype(std::declval<QChar>().unicode()))};
  return qstringHeaderSize + charSize * string.size();
}
}
