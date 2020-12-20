#include <QtQuickTest/quicktest.h>
#include <QQmlEngine>

#include <QtQml>

#include "QtPiDeckCommon.hpp"
#include "TestHelper.hpp"

void initStaticResources() {
    Q_INIT_RESOURCE(qmlCommon);
}

namespace QtPiDeck::Tests {
class Setup : public QObject
{
    // NOLINTNEXTLINE
    Q_OBJECT

public:
    Setup() = default;

public slots: // NOLINT(readability-redundant-access-specifiers)
    void qmlEngineAvailable(QQmlEngine *engine) // NOLINT(readability-convert-member-functions-to-static)
    {
        initStaticResources();
        engine->addImportPath("qrc:/qml/components");
        qmlRegisterType<TestHelper>("QtPiDeck.Tests", 1, 0, "TestHelper");
        qDebug() << "Registered types";
    }
};
}

QUICK_TEST_MAIN_WITH_SETUP(QtPiDeck::Tests::CommandControl, QtPiDeck::Tests::Setup) // NOLINT

#include "main.moc"
