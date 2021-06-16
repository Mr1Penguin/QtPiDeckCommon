find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

function(add_common_cpp_test testName filename)
    add_executable(${testName}
                   ${filename})
    target_link_libraries(${testName}
        PRIVATE
        BoostTestWrapper
        QtPiDeckCommon)
    set_warnings_flags(${testName})
    add_test(NAME ${testName} COMMAND ${testName})
    target_compile_definitions(${testName} PRIVATE BOOST_TEST_NO_MAIN BOOST_TEST_ALTERNATIVE_INIT_API)
endfunction()

# I don't like file-per-test but 
# this is the only way to enable 
# custom main and cross platform
add_common_cpp_test(ApplicationTests src/ApplicationTests.cpp)
add_common_cpp_test(MessageHeaderTests src/Network/MessageHeaderTests.cpp)
add_common_cpp_test(IocTests src/Services/IocTests.cpp)
add_common_cpp_test(MessageBusTests src/Services/MessageBusTests.cpp)
add_common_cpp_test(LiteralTests src/Utilities/LiteralTests.cpp)

# visual studio test manager requires to have these libs in PATH
# so we just copy required libs to test binary
if(VISUAL_STUDIO)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Qml Gui Network REQUIRED)
    add_custom_command(
        TARGET ApplicationTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:QtPiDeckCommon>
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Core>
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Qml>
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Gui>
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Network>
        ${CMAKE_CURRENT_BINARY_DIR})
endif()