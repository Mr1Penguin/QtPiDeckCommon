CONFIG += warn_on qmltestcase

TEMPLATE = app

QT += quick

DISTFILES += \
    qml/tst_commandtests.qml

SOURCES += \
    src/main.cpp

CONFIG += c++17

QML_IMPORT_PATH = $$PWD/../QtPiDeckCommon/qml/components

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtPiDeckCommon/release/ -lQtPiDeckCommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtPiDeckCommon/debug/ -lQtPiDeckCommon
else:unix: LIBS += -L$$OUT_PWD/../QtPiDeckCommon/ -lQtPiDeckCommon

INCLUDEPATH += $$PWD/../QtPiDeckCommon/include
DEPENDPATH += $$PWD/../QtPiDeckCommon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QmlTestsCommon/release/ -lQmlTestsCommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QmlTestsCommon/debug/ -lQmlTestsCommon
else:unix: LIBS += -L$$OUT_PWD/../QmlTestsCommon/ -lQmlTestsCommon

INCLUDEPATH += $$PWD/../QmlTestsCommon/include
DEPENDPATH += $$PWD/../QmlTestsCommon

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlTestsCommon/release/libQmlTestsCommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlTestsCommon/debug/libQmlTestsCommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlTestsCommon/release/QmlTestsCommon.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../QmlTestsCommon/debug/QmlTestsCommon.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../QmlTestsCommon/libQmlTestsCommon.a
