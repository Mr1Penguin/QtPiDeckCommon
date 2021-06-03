cmake_minimum_required (VERSION 3.15)

project("QtPiDeckCommon" LANGUAGES CXX)

option(VISUAL_STUDIO "" OFF)

include(CTest)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    option(FORCE_CPP17 "" OFF)
    list(FIND CMAKE_CXX_COMPILE_FEATURES "cxx_std_20" CXX20_POS)
    find_package(QT NAMES Qt6 Qt5 REQUIRED)
    if (FORCE_CPP17 OR ${CXX20_POS} EQUAL -1 OR (${QT_VERSION} VERSION_EQUAL "6.1.0" AND MSVC))
        set(CMAKE_CXX_STANDARD 17)
    else() 
        set(CMAKE_CXX_STANDARD 20)
    endif()
    unset(CXX20_POS)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    if (MSVC)
        # warning level 4 and all warnings as errors
        # waiting for /external to be fixed
        # add_compile_options(/W4 /WX)
        # add_compile_options(/WX)
    else()
        # lots of warnings and all warnings as errors
        add_compile_options(-Wall -Wextra -pedantic -Werror)
    endif()
endif()

option(BUILD_COMMON_TESTS "" ON)
option(CODECOV "" OFF)

add_subdirectory(QtPiDeckCommon)
add_subdirectory(QtChecker)
add_subdirectory(QtDefinitions)
add_subdirectory(Tests)