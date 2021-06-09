find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_library(BoostTestWrapper INTERFACE
    include/BoostUnitTest.hpp)
target_include_directories(BoostTestWrapper INTERFACE include/ ${Boost_INCLUDE_DIRS})