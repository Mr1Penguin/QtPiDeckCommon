find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

add_executable(QtPiDeckCommonIocTests
    src/tst_QtPiDeckCommonIocTests.cpp)
add_test(NAME QtPiDeckCommonIocTests COMMAND QtPiDeckCommonIocTests)

target_link_libraries(QtPiDeckCommonIocTests
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    QtPiDeckCommon)
