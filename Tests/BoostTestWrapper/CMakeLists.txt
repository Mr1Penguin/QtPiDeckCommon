find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_library(BoostTestWrapper INTERFACE
    include/BoostUnitTest.hpp)
target_link_libraries(BoostTestWrapper 
    INTERFACE
      Boost::unit_test_framework)
target_include_directories(BoostTestWrapper INTERFACE include/ ${Boost_INCLUDE_DIRS})