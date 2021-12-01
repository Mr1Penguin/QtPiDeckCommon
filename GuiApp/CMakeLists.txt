find_package(QT NAMES Qt6 Qt5 COMPONENTS Quick REQUIRED) 
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick REQUIRED)

find_package(Boost REQUIRED COMPONENTS log)

add_executable(GuiApp
               src/main.cpp
               qml.qrc)

target_link_libraries(GuiApp
                      PRIVATE 
                      Qt${QT_VERSION_MAJOR}::Quick
                      QtPiDeckCommon)

if(VISUAL_STUDIO)
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
        set(RELEASE_FLAG "--release")
    endif()

    add_custom_target(
        CopyQtPiDeckCommonGuiApp
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:QtPiDeckCommon>
            $<TARGET_FILE:Boost::log>
            ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS QtPiDeckCommon)

    add_dependencies(GuiApp CopyQtPiDeckCommonGuiApp)

    add_custom_command(
        TARGET GuiApp POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
            --verbose 0
            --qmldir ${CMAKE_CURRENT_SOURCE_DIR}
            --no-compiler-runtime
            ${RELEASE_FLAG}
            \"$<TARGET_FILE:GuiApp>\")

    unset(RELEASE_FLAG)
endif()
