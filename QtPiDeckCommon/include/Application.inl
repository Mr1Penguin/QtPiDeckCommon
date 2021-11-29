#pragma once

#include "ApplicationExt.inl"

#include "Utilities/Literals.hpp"

namespace QtPiDeck::detail {
template<class GuiApplication, class ApplicationEngine>
Application<GuiApplication, ApplicationEngine>::Application() {
  Application::s_current = this;
}

template<class GuiApplication, class ApplicationEngine>
auto Application<GuiApplication, ApplicationEngine>::start(int argc, char** argv) -> int {
  initialPreparationsBase();
  GuiApplication app{argc, argv};
  appCreatedBase();
  ApplicationEngine engine;
  engineCreatedBase(engine);
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
void Application<GuiApplication, ApplicationEngine>::initialPreparationsBase() {
  initialPreparations();
}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::appCreatedBase() {
  appCreated();
}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::engineCreatedBase(ApplicationEngine& engine) {
  using namespace Utilities::literals;
  engine.rootContext()->setContextProperty("qh"_qs, &m_qmlHelper);
  engineCreated(engine);
}
}