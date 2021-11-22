#include "Application.inl"

#include <QQmlContext>

namespace QtPiDeck::detail {
#if defined(__GNUC__)
template class Application<>;
#else
template class QTPIDECKCOMMON_EXPORT Application<>;
#endif
}
