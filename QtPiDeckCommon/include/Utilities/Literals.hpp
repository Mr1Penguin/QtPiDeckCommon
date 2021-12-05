#pragma once

#include <QString>
#include <qstringliteral.h>
#include <QUrl>

namespace QtPiDeck::Utilities {
inline namespace literals {
inline auto operator"" _qs(const char* str, size_t /*unused*/) noexcept -> QString { return QString{str}; }
inline auto operator"" _qls(const char* str, size_t /*unused*/) noexcept -> QLatin1String { return QLatin1String{str}; }
inline auto operator"" _qurl(const char* str, size_t /*unused*/) noexcept -> QUrl { return QUrl{QLatin1String{str}}; }
}
}