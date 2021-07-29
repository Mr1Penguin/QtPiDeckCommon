#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "QtPiDeckCommon.hpp"
#include "Utilities/Literals.hpp"

auto main(int argc, char* argv[]) -> int {
#if QT_VERSION_MAJOR < 6
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  using namespace QtPiDeck::Utilities::literals;
  QtPiDeckCommon qpdc;
  qpdc.registerTypes();
  engine.addImportPath("qrc:/qml/components"_qs);
  const QUrl url = "qrc:/qml/app.qml"_qurl;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [&url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}