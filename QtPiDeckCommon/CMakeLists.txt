find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

add_library(QtPiDeckCommon SHARED
  src/Application.cpp
  src/QtPiDeckCommon.cpp
  src/Network/MessageHeader.cpp
  src/Services/MessageBus.cpp
  src/Services/ServiceInterface.cpp
  include/Application.hpp
  include/QtPiDeckCommon.hpp
  include/QtPiDeckCommon_global.hpp
  include/Bus/Message.hpp
  include/Network/DeckDataStream.hpp
  include/Network/MessageHeader.hpp
  include/Services/IMessageBus.hpp
  include/Services/Ioc.hpp
  include/Services/ISettingsStorage.hpp
  include/Services/MessageBus.hpp
  include/Services/ServiceInterface.hpp
  include/Services/UseServices.hpp
  qmlCommon.qrc
)

target_include_directories(QtPiDeckCommon
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(QtPiDeckCommon
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG> QTPIDECKCOMMON_LIBRARY)
target_link_libraries(QtPiDeckCommon
  PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  QtChecker)
