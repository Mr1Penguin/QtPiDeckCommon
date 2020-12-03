#include <QtQuickTest/quicktest.h>
#include <QQmlEngine>

#include <QtQml>

#include "QtPiDeckCommon.hpp"
#include "TestHelper.hpp"

namespace QtPiDeck::Tests {
class Setup : public QObject
{
    Q_OBJECT

public:
    Setup() {}

public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        QtPiDeckCommon hack;
        engine->addImportPath("qrc:/qml/components");
        qmlRegisterType<TestHelper>("QtPiDeck.Tests", 1, 0, "TestHelper");
    }
};
}

QUICK_TEST_MAIN_WITH_SETUP(QtPiDeck::Tests::CommandControl, QtPiDeck::Tests::Setup)

#include "main.moc"
