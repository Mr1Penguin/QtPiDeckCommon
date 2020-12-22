#include <QtQuickTest/quicktest.h>
#include <QQmlEngine>
#include <QtQml>

#include <array>

#include "QtPiDeckCommon.hpp"
#include "TestHelper.hpp"
#include "TestSetup.hpp"

namespace QtPiDeck::Tests {
auto makeImports() noexcept {
    return std::array<std::string, 1> {"qrc:/qml/components"};
}

void registerTypes() noexcept {
    qmlRegisterType<QtPiDeck::Tests::TestHelper>("QtPiDeck.Tests", 1, 0, "TestHelper");
}

class Setup : public QObject
{
    // NOLINTNEXTLINE
    Q_OBJECT

public:
    Setup() { hack.registerTypes(); }

public slots: // NOLINT(readability-redundant-access-specifiers)
    void qmlEngineAvailable(MAYBE_UNUSED QQmlEngine *engine) // NOLINT(readability-convert-member-functions-to-static)
    {
        addImports15(engine, makeImports);
        callRegisterTypes15(registerTypes);
    }

private:
    QtPiDeckCommon hack;
};
}

#if QT_VERSION == QTPI4_VERSION
QTPIDECK_QUICK_TEST_MAIN_WITH_SETUP(QtPiDeck::Tests::CommandControl, QtPiDeck::Tests::Setup, QtPiDeck::Tests::makeImports, QtPiDeck::Tests::registerTypes) // NOLINT
#else
QUICK_TEST_MAIN_WITH_SETUP(QtPiDeck::Tests::CommandControl, QtPiDeck::Tests::Setup) // NOLINT
#endif

#include "main.moc"
