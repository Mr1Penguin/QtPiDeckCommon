set(TESTS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(get_test_name OUT PATH)
    string(REPLACE ${TESTS_ROOT_DIR} "" TMP ${PATH})
    string(SUBSTRING ${TMP} 1 -1 TMP)
    string(REPLACE "/" "::" TMP ${TMP})
    set(${OUT} ${TMP} PARENT_SCOPE)
endfunction()

function(get_test_exe_name OUT TEST_NAME)
    string(REPLACE "::" "_" TMP ${TEST_NAME})
    set(${OUT} ${TMP} PARENT_SCOPE)
endfunction()

macro(add_cpp_test PATH)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

    get_test_name(TEST_NAME ${PATH})
    get_test_exe_name(EXE_NAME ${TEST_NAME})

    add_executable(${EXE_NAME} ${PATH}/main.cpp)
    add_test(NAME ${TEST_NAME} COMMAND ${EXE_NAME})

    target_link_libraries(${EXE_NAME}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        QtPiDeckCommon)

    message(STATUS "${TEST_NAME} ${EXE_NAME}")
endmacro()

add_subdirectory(QmlTestsCommon)

add_subdirectory(QtPiDeck/CommandTests)
add_subdirectory(QtPiDeck/Services/IocTests)
add_subdirectory(QtPiDeck/Services/MessageBusTests)
