QT += quick

TEMPLATE = lib
DEFINES += QTPIDECKCOMMON_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/QtPiDeckCommon.cpp

HEADERS += \
    include/QtPiDeckCommon_global.hpp \
    include/network/MessageHeader.hpp \
    include/QtPiDeckCommon.hpp

INCLUDEPATH += include

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qmlCommon.qrc
