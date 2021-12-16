find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

find_package(Boost REQUIRED COMPONENTS log)

add_library(QtPiDeckCommon SHARED
  src/Application.cpp
  src/QtPiDeckCommon.cpp
  src/Network/MessageHeader.cpp
  src/Network/Messages/HelloMessage.cpp
  src/Services/Ioc.cpp
  src/Services/MessageBus.cpp
  src/Services/ServiceInterface.cpp
  src/Utilities/Logging.cpp
  src/ViewModels/CommandViewModel.cpp
  include/Application.hpp
  include/Application.inl
  include/QtPiDeckCommon.hpp
  include/QtPiDeckCommon_global.hpp
  include/Network/DeckDataStream.hpp
  include/Network/MessageHeader.hpp
  include/Network/Messages/HelloMessage.hpp
  include/Services/IMessageBus.hpp
  include/Services/Ioc.hpp
  include/Services/ISettingsStorage.hpp
  include/Services/MessageBus.hpp
  include/Services/ServiceInterface.hpp
  include/Services/UseServices.hpp
  include/Utilities/Logging.hpp
  include/Utilities/Literals.hpp
  include/Utilities/Connection.hpp
  include/Utilities/MessageUtils.hpp
  include/Utilities/OnExit.hpp
  include/Utilities/QmlHelper.hpp
  include/Utilities/QmlHelperDetail.hpp
  include/Utilities/Traits.hpp
  include/Utilities/QBAutils.hpp
  include/ViewModels/CommandViewModel.hpp
  qmlCommon.qrc
  icons.qrc)

target_include_directories(QtPiDeckCommon
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(
    QtPiDeckCommon
    PRIVATE 
    QTPIDECKCOMMON_LIBRARY
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    # Warning from test applications makes it unable for visual studio to detect tests
    $<$<AND:$<BOOL:${VISUAL_STUDIO}>,$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>>:QT_QML_DEBUG_NO_WARNING>
)

if(CODECOV)
   if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(QtPiDeckCommon PUBLIC --coverage)
        target_link_options(QtPiDeckCommon PUBLIC --coverage)
   else() # clang
        target_compile_options(QtPiDeckCommon PUBLIC -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(QtPiDeckCommon PUBLIC -fprofile-instr-generate -fcoverage-mapping -fuse-ld=gold)
   endif()
endif()

set_warnings_flags(QtPiDeckCommon)

target_link_libraries(QtPiDeckCommon
  PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Boost::log
  QtChecker
  QtDefinitions)
