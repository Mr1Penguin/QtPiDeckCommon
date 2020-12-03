find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

add_library(QmlTestsCommon STATIC
  src/TestHelper.cpp
  include/TestHelper.hpp
)

target_include_directories(QmlTestsCommon
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(QmlTestsCommon
  PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
