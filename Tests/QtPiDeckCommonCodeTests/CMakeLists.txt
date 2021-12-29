find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

add_executable(QtPiDeckCommonCodeTests
    src/main.cpp
    src/ApplicationTests.cpp
    src/printers.hpp
    src/Bus/MessageTests.cpp
    src/Network/DeckDataStreamTests.cpp
    src/Network/MessageHeaderTests.cpp
    src/Network/Messages/HelloMessageTests.cpp
    src/Services/DeckMessageToBusMessageMapperTests.cpp
    src/Services/IocTests.cpp
    src/Services/MessageBusTests.cpp
    src/Services/MessageSenderTests.cpp
    src/Utilities/ConnectionTests.cpp
    src/Utilities/LiteralTests.cpp
    src/Utilities/OnExitTests.cpp
    src/Utilities/QBAutilsTests.cpp
    src/Utilities/QmlHelperTests.cpp
    src/ViewModels/CommandViewModelTests.cpp)

target_include_directories(QtPiDeckCommonCodeTests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(QtPiDeckCommonCodeTests
    PRIVATE
      Qt${QT_VERSION_MAJOR}::Quick
      BoostTestWrapper
      QtPiDeckCommon
      CodeTestsUtils)

set_warnings_flags(QtPiDeckCommonCodeTests)
add_test(NAME QtPiDeckCommonCodeTests COMMAND QtPiDeckCommonCodeTests)
target_compile_definitions(QtPiDeckCommonCodeTests
    PRIVATE
      $<$<BOOL:${WIN32}>:BOOST_TEST_DYN_LINK>
      $<$<AND:$<BOOL:${WIN32}>,$<EQUAL:${QT_VERSION_MAJOR},5>>:NOMINMAX>)

# visual studio test manager requires to have these libs in PATH
# so we just copy required libs to test binary
if(VISUAL_STUDIO)
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
        set(RELEASE_FLAG "--release")
    endif()

    add_custom_target(
        CopyQtPiDeckCommonTests
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:QtPiDeckCommon>
            #$<TARGET_FILE:Boost::log>
            ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS QtPiDeckCommon)

    add_dependencies(QtPiDeckCommonCodeTests CopyQtPiDeckCommonTests)

    add_custom_command(
        TARGET QtPiDeckCommonCodeTests POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
            --verbose 0
            --qmldir ${CMAKE_CURRENT_SOURCE_DIR}
            --no-compiler-runtime
            ${RELEASE_FLAG}
            \"$<TARGET_FILE:QtPiDeckCommonCodeTests>\"
            \"$<TARGET_FILE:QtPiDeckCommon>\")

    unset(RELEASE_FLAG)
endif()
