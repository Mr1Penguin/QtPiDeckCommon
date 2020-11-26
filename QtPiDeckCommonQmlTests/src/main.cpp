#include <QtQuickTest/quicktest.h>
#include <QQmlEngine>

#include <QtQml>

#include "QtPiDeckCommon.hpp"
#include "Utils.hpp"
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
        qmlRegisterSingletonType<TestHelper>("QtPiDeck", 1, 0, "TestHelper", qmlSingletonProvider<TestHelper>);
    }
};
}

QUICK_TEST_MAIN_WITH_SETUP(CommandControl, QtPiDeck::Tests::Setup)

#include "main.moc"
