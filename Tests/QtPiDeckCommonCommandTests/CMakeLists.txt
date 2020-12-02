find_package(QT NAMES Qt6 Qt5 COMPONENTS Quick QuickTest REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick QuickTest REQUIRED)

add_executable(QtPiDeckCommonCommandTests
    src/main.cpp
    qml/tst_commandtests.qml)
add_test(NAME QtPiDeckCommonCommandTests COMMAND QtPiDeckCommonCommandTests -platform offscreen)

target_link_libraries(QtPiDeckCommonCommandTests
    PRIVATE
    Qt${QT_VERSION_MAJOR}::QuickTest
    Qt${QT_VERSION_MAJOR}::Quick
    QtPiDeckCommon
    QmlTestsCommon)
target_compile_definitions(QtPiDeckCommonCommandTests PRIVATE -DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

get_target_property(QtPiDeckCommon_SOURCE_DIR QtPiDeckCommon SOURCE_DIR)
set(QML_IMPORT_PATH ${QtPiDeckCommon_SOURCE_DIR}/qml/components CACHE STRING "Qt Creator qml import paths" FORCE)
