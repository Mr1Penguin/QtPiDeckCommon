#include "QtPiDeckCommon.hpp"

QtPiDeckCommon::QtPiDeckCommon() = default;

void QtPiDeckCommon::registerTypes()
{
    Q_INIT_RESOURCE(qmlCommon); // NOLINT
    Q_INIT_RESOURCE(icons); // NOLINT
}
