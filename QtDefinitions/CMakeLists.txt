find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_library(QtDefinitions INTERFACE
    include/QtDefinitions.hpp
    include/QtPlatform.hpp
    include/QtStyle.hpp)
target_include_directories(QtDefinitions INTERFACE include/)
target_link_libraries(QtDefinitions INTERFACE Qt${QT_VERSION_MAJOR}::Core)
