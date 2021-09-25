#include <QQmlEngine>
#include <QtQml>
#include <QtQuickTest/quicktest.h>

#include <array>

#include "QmlTreeExplorer.hpp"
#include "QtPiDeckCommon.hpp"
#include "TestSetup.hpp"
#include "Utilities/QmlHelpers.hpp"
#include "ViewModels/CommandViewModel.hpp"
#include "Utilities/Literals.hpp"

namespace QtPiDeck::Tests {
auto makeImports() noexcept {
  using namespace Utilities::literals;
  return std::array{"qrc:/qml/components"_qs};
}

void registerTypes() noexcept {
  ViewModels::CommandViewModel::registerType();
  qmlRegisterType<QtPiDeck::Tests::QmlTreeExplorer>("QtPiDeck.Tests", 1, 0, "QmlTreeExplorer");
}

class Setup : public QObject {
  // NOLINTNEXTLINE
  Q_OBJECT

public:
  Setup() { hack.registerTypes(); }

public slots:                                              // NOLINT(readability-redundant-access-specifiers)
  void qmlEngineAvailable(MAYBE_UNUSED QQmlEngine* engine) // NOLINT(readability-convert-member-functions-to-static)
  {
    engine->rootContext()->setContextProperty("qh", &helper);
    addImports15(engine, makeImports);
    callRegisterTypes15(registerTypes);
  }

private:
  QtPiDeckCommon hack;
  QtPiDeck::Utilities::QmlHelper helper;
};
}

#if QT_VERSION == QTPI4_VERSION
QTPIDECK_QUICK_TEST_MAIN_WITH_SETUP(QtPiDeck::Tests::CommandTests, QtPiDeck::Tests::Setup, QtPiDeck::Tests::makeImports,
                                    QtPiDeck::Tests::registerTypes) // NOLINT
#else
QUICK_TEST_MAIN_WITH_SETUP(QtPiDeck::Tests::CommandTests, QtPiDeck::Tests::Setup) // NOLINT
#endif

#include "main.moc"
