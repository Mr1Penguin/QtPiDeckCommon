#include "Application.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace QtPiDeck {
Application::Application() { Application::s_current = this; }

auto Application::start(int argc, char** argv) -> int {
  initialPreparations();
  QGuiApplication app{argc, argv};
  appCreated();
  QQmlApplicationEngine engine;
  engineCreated(engine);
  auto pageUrl = mainPage();
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, // clazy:exclude=connect-non-signal
      &app,
      [&url = pageUrl, this](QObject* obj, const QUrl& objUrl) {
        if (obj == nullptr && url == objUrl) {
          QCoreApplication::exit(-1);
        }

        m_qmlHelper.windowCreated();
      },
      Qt::QueuedConnection);
  engine.load(pageUrl);
  return QGuiApplication::exec();
}

void Application::initialPreparations() {}

void Application::appCreated() {}

void Application::engineCreated(QQmlApplicationEngine& engine) {
  engine.rootContext()->setContextProperty("qh", &m_qmlHelper);
}
}
