#define BOOST_TEST_MODULE QtPiDeckCommonCodeTests // NOLINT
#include "BoostUnitTest.hpp"

#include "Utilities/Logging.hpp"

struct GlobalInit {
  GlobalInit() { QtPiDeck::Utilities::initLogging("QtPiDeckCommonCodeTests"); }
};

const GlobalInit init;