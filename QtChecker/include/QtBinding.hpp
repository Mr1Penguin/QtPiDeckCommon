#pragma once

#include <QtCore/qglobal.h>

#if QT_VERSION_MAJOR == 5
#define Q_OBJECT_BINDABLE_PROPERTY(CLASS, TYPE, NAME, ...) TYPE NAME; // NOLINT(cppcoreguidelines-macro-usage)
#define BINDABLE(TYPE, NAME, PROPERTY_NAME)
#else
#define BINDABLE(TYPE, NAME, PROPERTY_NAME) QBindable<TYPE> NAME() { return &PROPERTY_NAME; }
#endif
