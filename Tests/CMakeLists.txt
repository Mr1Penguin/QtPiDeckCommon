set(TESTS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

macro(add_cpp_test PATH)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

    string(REPLACE ${TESTS_ROOT_DIR} "" TEST_NAME ${PATH})
    string(SUBSTRING ${TEST_NAME} 1 -1 TEST_NAME)
    string(REPLACE "/" "::" TEST_NAME ${TEST_NAME})

    string(REPLACE "::" "_" EXE_NAME ${TEST_NAME})

    add_executable(${EXE_NAME} ${PATH}/main.cpp)
    add_test(NAME ${TEST_NAME} COMMAND ${EXE_NAME})

    target_link_libraries(${EXE_NAME}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        QtPiDeckCommon)

    message(STATUS "${TEST_NAME} ${EXE_NAME}")
endmacro()

add_subdirectory(QmlTestsCommon)
add_subdirectory(QtPiDeckCommonCommandTests)

add_subdirectory(QtPiDeck/IocTests)
add_subdirectory(QtPiDeck/Services/MessageBusTests)
