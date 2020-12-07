#include "Application.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Services/QmlService.hpp"

namespace QtPiDeck {
Application::Application() {
    Application::current = this;
}

auto Application::start(int &argc, char **argv) -> int {
    appStartupPreparations();
    QGuiApplication app{argc, argv};
    QQmlApplicationEngine engine;
    setupEngine(engine);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url = mainPage()](QObject *obj, const QUrl &objUrl) {
        if (obj == nullptr && url == objUrl) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);
    engine.load(mainPage());
    return QGuiApplication::exec();
}

void Application::appStartupPreparations() {

}

void Application::setupEngine(QQmlApplicationEngine & /*engine*/) {
}
}
