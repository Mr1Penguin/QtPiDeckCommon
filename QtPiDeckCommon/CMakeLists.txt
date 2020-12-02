find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

add_library(QtPiDeckCommon SHARED
  src/Application.cpp
  src/QtPiDeckCommon.cpp
  include/Application.hpp
  include/QtPiDeckCommon.hpp
  include/QtPiDeckCommon_global.hpp
  include/Network/MessageHeader.hpp
  qmlCommon.qrc
)

target_include_directories(QtPiDeckCommon
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(QtPiDeckCommon
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG> QTPIDECKCOMMON_LIBRARY)
target_link_libraries(QtPiDeckCommon
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
