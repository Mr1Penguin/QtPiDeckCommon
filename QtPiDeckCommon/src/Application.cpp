#include "Application.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

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
