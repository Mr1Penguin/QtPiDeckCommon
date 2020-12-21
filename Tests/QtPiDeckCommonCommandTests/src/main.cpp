#include <QtQuickTest/quicktest.h>
#include <QQmlEngine>

#include <QtQml>

#include "QtPiDeckCommon.hpp"
#include "TestHelper.hpp"

//int Q_DECL_IMPORT qInitResources_qmlCommon();

void initStaticResources() {
    //qInitResources_qmlCommon();
    //Q_INIT_RESOURCE(qmlCommon);
}

namespace QtPiDeck::Tests {
class Setup : public QObject
{
    // NOLINTNEXTLINE
    Q_OBJECT

public:
    Setup() { qWarning() << "Setup created"; }

public slots: // NOLINT(readability-redundant-access-specifiers)
    void qmlEngineAvailable(QQmlEngine *engine) // NOLINT(readability-convert-member-functions-to-static)
    {
        //Q_INIT_RESOURCE(qmlCommon);
        QtPiDeckCommon hack;
        hack.registerTypes();
        engine->addImportPath("qrc:/qml/components");
        qmlRegisterType<TestHelper>("QtPiDeck.Tests", 1, 0, "TestHelper");
        qWarning() << "Loaded";
    }
};
}

QUICK_TEST_MAIN_WITH_SETUP(QtPiDeck::Tests::CommandControl, QtPiDeck::Tests::Setup) // NOLINT

#include "main.moc"
