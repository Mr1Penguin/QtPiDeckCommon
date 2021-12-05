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
#if defined(_MSC_VER)
  // weird false(?) positive for unreachable code when compiling with test template arguments
#pragma warning(push)
#pragma warning(disable : 4702)
#endif
  QmlHelperPrivate() : QObject(nullptr), m_dpi(minDpi) {
    if (const auto* screen = GuiApp::primaryScreen(); screen != nullptr) {
      if (const auto newDpi = screen->logicalDotsPerInch(); newDpi > m_dpi) {
        m_dpi = newDpi;
      }
    }
  }
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

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
}
