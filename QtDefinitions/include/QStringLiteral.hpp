#pragma once

#include "QtVersion.hpp"

#include <QString>

#if QT_VERSION == QTPI4_VERSION 
#define CT_QStringLiteral(x) QStringLiteral(x) // NOLINT
#else
#define CT_QStringLiteral(x) QStringLiteral(x)
#endif