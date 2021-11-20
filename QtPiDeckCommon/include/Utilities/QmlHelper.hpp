// NOCOVERAGE

#pragma once

#include "QmlHelperDetail.hpp"

namespace QtPiDeck::Utilities {
template<class Derived>
using QmlHelperPrivate = detail::QmlHelperPrivate<Derived>;

// exclude untestable class
// LCOV_EXCL_START
class QTPIDECKCOMMON_EXPORT QmlHelper : public QmlHelperPrivate<QmlHelper> {
  Q_OBJECT // NOLINT
  friend QmlHelperPrivate<QmlHelper>;
  Q_PROPERTY(double dpi READ dpi NOTIFY dpiChanged)
public:
  QmlHelper() : QmlHelperPrivate() {}

  // does not work with several windows on several screens with different dpi
  Q_INVOKABLE double dp(double value, double dpi) const { // NOLINT(modernize-use-trailing-return-type)
    return QmlHelperPrivate::dp(value, dpi);
  }
  // font size to apply font scale later
  Q_INVOKABLE double sp(double value, double dpi) const { // NOLINT(modernize-use-trailing-return-type)
    return QmlHelperPrivate::sp(value, dpi);
  }

signals:
  void dpiChanged();

private:
  void updateDpi() { emit dpiChanged(); }
};
// LCOV_EXCL_STOP
}
