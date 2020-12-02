#include "Application.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

namespace QtPiDeck::detail {
Application::Application() {

}

Application::~Application() {

}

int Application::start(int &argc, char **argv) {
    appStartupPreparations();
    QGuiApplication app{argc, argv};
    QQmlApplicationEngine engine;
    setupEngine(engine);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url = mainPage()](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(mainPage());
    return app.exec();
}

void Application::appStartupPreparations() {

}

void Application::setupEngine(QQmlApplicationEngine &) {

}
}
