// NOLINTNEXTLINE
#define BOOST_TEST_MODULE QtPiDeckCommonCodeTests
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include "BoostUnitTest.hpp"
#include "Utilities/Logging.hpp"

auto main(int argc, char* argv[]) -> int {
  QtPiDeck::Utilities::initLogging("qtpideck");
  return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}