find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_library(BoostTestWrapper INTERFACE
    include/BoostUnitTest.hpp)
target_include_directories(BoostTestWrapper INTERFACE include/)
target_link_libraries(BoostTestWrapper INTERFACE Boost::boost Boost::unit_test_framework)
