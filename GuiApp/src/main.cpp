#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "QtDefinitions.hpp"
#include "QtPiDeckCommon.hpp"
#include "Utilities/Literals.hpp"
#include "Utilities/QmlHelper.hpp"
#include "ViewModels/CommandViewModel.hpp"

auto main(int argc, char* argv[]) -> int {
#if QT_VERSION_MAJOR < 6
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  using namespace QtPiDeck::Utilities::literals;
  QtPiDeckCommon qpdc;
  qpdc.registerTypes();
  engine.addImportPath(CT_QStringLiteral("qrc:/qml/components"));
  const auto url = QUrl{CT_QStringLiteral("qrc:/qml/app.qml")};
  QtPiDeck::Utilities::QmlHelper helper;
  engine.rootContext()->setContextProperty("qh", &helper);
  QtPiDeck::ViewModels::CommandViewModel::registerType();
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [&url, &helper](QObject* obj, const QUrl& objUrl) {
        if (obj == nullptr && url == objUrl) {
          QCoreApplication::exit(-1);
        }

        helper.windowCreated();
      },
      Qt::QueuedConnection);
  engine.load(url);

  return QGuiApplication::exec();
}

//#include "main.moc"
