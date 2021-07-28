find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_library(QtChecker INTERFACE
            include/QtVersion.hpp)
target_include_directories(QtChecker INTERFACE include/)
target_link_libraries(QtChecker INTERFACE Qt${QT_VERSION_MAJOR}::Core)
