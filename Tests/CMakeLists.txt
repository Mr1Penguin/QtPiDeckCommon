set(TESTS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TESTED_TARGET QtPiDeckCommon)

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

macro(add_cpp_test)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

    get_test_name(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR})
    get_test_exe_name(EXE_NAME ${TEST_NAME})

    add_executable(${EXE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
    add_test(NAME ${TEST_NAME} COMMAND ${EXE_NAME})

    target_link_libraries(${EXE_NAME}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        ${TESTED_TARGET})
endmacro()

macro(add_qml_test QML_FILE)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Quick QuickTest REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick QuickTest REQUIRED)

    get_test_name(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR})
    get_test_exe_name(EXE_NAME ${TEST_NAME})

    add_executable(${EXE_NAME}
        main.cpp
        ${QML_FILE})
    add_test(NAME ${TEST_NAME} COMMAND ${EXE_NAME} -platform offscreen)

    target_link_libraries(${EXE_NAME}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::QuickTest
        Qt${QT_VERSION_MAJOR}::Quick
        ${TESTED_TARGET}
        QmlTestsUtils)
    target_compile_definitions(${EXE_NAME} PRIVATE -DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

    get_target_property(QtPiDeckCommon_SOURCE_DIR QtPiDeckCommon SOURCE_DIR)
    set(QML_IMPORT_PATH ${QtPiDeckCommon_SOURCE_DIR}/qml/components CACHE STRING "Qt Creator qml import paths" FORCE)

endmacro()

add_subdirectory(QmlTestsUtils)

add_subdirectory(QtPiDeck/CommandTests)
add_subdirectory(QtPiDeck/Network/MessageHeaderTests)
add_subdirectory(QtPiDeck/Services/IocTests)
add_subdirectory(QtPiDeck/Services/MessageBusTests)
