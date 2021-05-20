find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_library(CodeTestsUtils STATIC
  src/SignalCatcher.cpp
  include/SignalCatcher.hpp
)

target_include_directories(CodeTestsUtils
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(CodeTestsUtils
  PUBLIC
  Qt${QT_VERSION_MAJOR}::Core)
