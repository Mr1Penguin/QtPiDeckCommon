#pragma once

#include <QtCore/qglobal.h>

#ifndef QTPI4_VERSION
#define QTPI4_VERSION QT_VERSION_CHECK(5, 11, 3) // NOLINT
#endif
#ifndef QT5_15_LTS_VERSION
#define QT5_15_LTS_VERSION QT_VERSION_CHECK(5, 15, 1) // NOLINT
#endif

#if QT_VERSION > QTPI4_VERSION && QT_VERSION < QT5_15_LTS_VERSION
#error "New rasberrypi 4 qt version detected(?)"
#endif
