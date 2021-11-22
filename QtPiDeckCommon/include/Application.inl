#pragma once

#include "Application.hpp"

namespace QtPiDeck::detail {
template<class GuiApplication, class ApplicationEngine>
Application<GuiApplication, ApplicationEngine>::Application() {
  Application::s_current = this;
}

template<class GuiApplication, class ApplicationEngine>
auto Application<GuiApplication, ApplicationEngine>::start(int argc, char** argv) -> int {
  initialPreparations();
  GuiApplication app{argc, argv};
  appCreated();
  ApplicationEngine engine;
  engineCreated(engine);
  auto pageUrl = mainPage();
  QObject::connect(
      &engine, &ApplicationEngine::objectCreated, // clazy:exclude=connect-non-signal
      &app,
      [&url = pageUrl, this](QObject* obj, const QUrl& objUrl) {
        if (obj == nullptr && url == objUrl) {
          GuiApplication::exit(-1);
        }

        m_qmlHelper.windowCreated();
      },
      Qt::QueuedConnection);
  engine.load(pageUrl);
  return GuiApplication::exec();
}

template<class GuiApplication, class ApplicationEngine>
auto Application<GuiApplication, ApplicationEngine>::ioc() -> Services::Ioc& {
  return m_ioc;
}

template<class GuiApplication, class ApplicationEngine>
[[nodiscard]] auto Application<GuiApplication, ApplicationEngine>::ioc() const -> const Services::Ioc& {
  return m_ioc;
}

template<class GuiApplication, class ApplicationEngine>
[[nodiscard]] auto Application<GuiApplication, ApplicationEngine>::current() -> Application* {
  return s_current;
}

template<class GuiApplication, class ApplicationEngine>
[[nodiscard]] auto Application<GuiApplication, ApplicationEngine>::ccurrent() -> const Application* {
  return s_current;
}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::initialPreparations() {}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::appCreated() {}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::engineCreated(ApplicationEngine& engine) {
  engine.rootContext()->setContextProperty("qh", &m_qmlHelper);
}
}