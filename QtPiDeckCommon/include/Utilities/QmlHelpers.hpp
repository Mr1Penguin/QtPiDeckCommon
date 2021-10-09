#pragma once

#include <QGuiApplication>
#include <QObject>
#include <QScreen>
#include <QWindow>

#include "QtPiDeckCommon_global.hpp"

namespace QtPiDeck::Utilities {
class QTPIDECKCOMMON_EXPORT QmlHelper : public QObject {
  Q_OBJECT // NOLINT
public:
  QmlHelper() : QObject(nullptr), m_dpi(minDpi) {
    if (const auto* screen = QGuiApplication::primaryScreen(); screen) {
      if (const auto newDpi = screen->logicalDotsPerInch(); newDpi > m_dpi) {
        m_dpi = newDpi;
      }
    }
  }

  void windowCreated() {
    const auto& windowList = QGuiApplication::allWindows();
    if (std::empty(windowList)) {
      return;
    }

    constexpr size_t mainWindowIndex = 0;
    const auto* window               = windowList[mainWindowIndex];
    connect(window, &QWindow::screenChanged, this, &QmlHelper::screenChanged);
    connectToLogicalDpiChanged(window->screen());
  }

  Q_INVOKABLE double dp(double value) const { // NOLINT(modernize-use-trailing-return-type)
    return value * (m_dpi / baseDpi);
  }
  // font size to apply font scale later
  Q_INVOKABLE double sp(double value) const { // NOLINT(modernize-use-trailing-return-type)
    return dp(value);
  }

private:
  void screenChanged(QScreen* screen) {
    setDpi(screen->logicalDotsPerInch());
    connectToLogicalDpiChanged(screen);
  }

  void setDpi(double dpi) { m_dpi = std::max(minDpi, dpi); }

  void connectToLogicalDpiChanged(QScreen* screen) {
    disconnect(m_dpiChangedConnection);
    m_dpiChangedConnection = connect(screen, &QScreen::logicalDotsPerInchChanged, this, &QmlHelper::setDpi);
  }

  double m_dpi;
  QMetaObject::Connection m_dpiChangedConnection;

  constexpr static double baseDpi = 160;
  constexpr static double minDpi  = 160;
};
}
