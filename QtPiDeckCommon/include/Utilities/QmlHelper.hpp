#pragma once

#include <QGuiApplication>
#include <QObject>
#include <QScreen>
#include <QWindow>

#include <cassert>

#include "QtPiDeckCommon_global.hpp"

namespace QtPiDeck::Utilities {
namespace detail {
template<class Derived, class GuiApp = QGuiApplication, class Window = QWindow, class Screen = QScreen>
class QmlHelperPrivate : public QObject {
public:
  QmlHelperPrivate() : QObject(nullptr), m_dpi(minDpi) {
    if (const auto* screen = GuiApp::primaryScreen(); screen != nullptr) {
      if (const auto newDpi = screen->logicalDotsPerInch(); newDpi > m_dpi) {
        m_dpi = newDpi;
      }
    }
  }

  void windowCreated() {
    const auto& windowList = GuiApp::allWindows();
    assert(!std::empty(windowList)); // LCOV_EXCL_LINE

    constexpr size_t mainWindowIndex = 0;
    const auto* window               = windowList[mainWindowIndex];

    // If windows somehow was created on different screen
    if (window->screen() != GuiApp::primaryScreen()) {
      setDpi(window->screen()->logicalDotsPerInch());
    }

    connect(window, &Window::screenChanged, this, &QmlHelperPrivate::screenChanged);
    connectToLogicalDpiChanged(window->screen());
  }

  [[nodiscard]] auto dp(double value, double dpi) const -> double {
    assert(value >= 0); // LCOV_EXCL_LINE
    return value * (dpi / baseDpi);
  }

  [[nodiscard]] auto sp(double value, double dpi) const -> double {
    assert(value >= 0); // LCOV_EXCL_LINE
    return dp(value, dpi);
  }

  [[nodiscard]] auto dpi() const -> double { return m_dpi; }

  constexpr static double baseDpi = 160;
  constexpr static double minDpi  = 160;

private:
  void screenChanged(const Screen* screen) {
    setDpi(screen->logicalDotsPerInch());
    connectToLogicalDpiChanged(screen);
  }

  void setDpi(double dpi) {
    const auto newDpi = std::max(minDpi, dpi);
    if (newDpi != m_dpi) {
      m_dpi = newDpi;
      static_cast<Derived*>(this)->updateDpi();
    }
  }

  void connectToLogicalDpiChanged(const Screen* screen) {
    QObject::disconnect(m_dpiChangedConnection);
    m_dpiChangedConnection =
        QObject::connect(screen, &Screen::logicalDotsPerInchChanged, this, &QmlHelperPrivate::setDpi);
  }

  double m_dpi;
  QMetaObject::Connection m_dpiChangedConnection;
};
}

template<class Derived>
using QmlHelperPrivate = detail::QmlHelperPrivate<Derived>;

// exclude untestabable class
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
