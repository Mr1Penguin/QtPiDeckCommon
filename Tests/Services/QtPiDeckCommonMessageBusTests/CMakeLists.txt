find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

add_executable(QtPiDeckCommonMessageBusTests
    src/tst_QtPiDeckCommonMessageBusTests.cpp)
add_test(NAME QtPiDeckCommonMessageBusTests COMMAND QtPiDeckCommonMessageBusTests)

target_link_libraries(QtPiDeckCommonMessageBusTests
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QtPiDeckCommon)
