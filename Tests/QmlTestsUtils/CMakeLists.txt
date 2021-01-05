find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

add_library(QmlTestsUtils STATIC
  src/QmlTreeExplorer.cpp
  include/TestSetup.hpp
  include/QmlTreeExplorer.hpp
)

target_include_directories(QmlTestsUtils
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(QmlTestsUtils
  PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  QtChecker)
