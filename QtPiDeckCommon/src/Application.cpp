#include "Application.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#ifdef A1
#error A1
#endif

#ifdef A2
#error A2
#endif

#ifdef A3
#error A3
#endif

#ifdef A4
#error A4
#endif

#ifdef A5
#error A5
#endif

namespace QtPiDeck {
Application::Application() {
    Application::s_current = this;
}

auto Application::start(int argc, char **argv) -> int {
    initialPreparations();
    QGuiApplication app{argc, argv};
    appCreated();
    QQmlApplicationEngine engine;
    engineCreated(engine);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, // clazy:exclude=connect-non-signal
                     &app, [url = mainPage()](QObject *obj, const QUrl &objUrl) {
        if (obj == nullptr && url == objUrl) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);
    engine.load(mainPage());
    return QGuiApplication::exec();
}

void Application::initialPreparations() {

}

void Application::appCreated() {

}

void Application::engineCreated(QQmlApplicationEngine & /*engine*/) {

}
}
