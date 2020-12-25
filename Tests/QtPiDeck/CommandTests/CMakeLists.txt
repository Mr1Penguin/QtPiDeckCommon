find_package(QT NAMES Qt6 Qt5 COMPONENTS Quick QuickTest REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick QuickTest REQUIRED)

get_test_name(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR})
get_test_exe_name(EXE_NAME ${TEST_NAME})

add_executable(${EXE_NAME}
    main.cpp
    tst_commandtests.qml)
add_test(NAME ${TEST_NAME} COMMAND ${EXE_NAME} -platform offscreen)

target_link_libraries(${EXE_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::QuickTest
    Qt${QT_VERSION_MAJOR}::Quick
    QtPiDeckCommon
    QmlTestsCommon)
target_compile_definitions(${EXE_NAME} PRIVATE -DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

get_target_property(QtPiDeckCommon_SOURCE_DIR QtPiDeckCommon SOURCE_DIR)
set(QML_IMPORT_PATH ${QtPiDeckCommon_SOURCE_DIR}/qml/components CACHE STRING "Qt Creator qml import paths" FORCE)
