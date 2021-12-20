#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <QByteArray>
#include <QDataStream>

namespace QtPiDeck::Utilities {
template<class T>
auto convert(const QByteArray& qba) -> T {
  T result;
  QDataStream stream{qba};
  stream >> result;
  return result;
}

namespace detail {
template<class... TArgs, std::size_t... I>
auto convertImpl(const QByteArray& qba, std::index_sequence<I...>) -> std::tuple<TArgs...> {
  auto result = std::tuple<TArgs...>{};
  auto stream = QDataStream{qba};
  (stream >> ... >> std::get<I>(result));
  return result;
}
}

template<class... TArgs>
auto convert(const QByteArray& qba) -> std::enable_if_t<(sizeof...(TArgs) > 1), std::tuple<TArgs...>> {
  return detail::convertImpl<TArgs...>(qba, std::index_sequence_for<TArgs...>{});
}
}
