#include "BoostUnitTest.hpp"

#include "Utilities/OnExit.hpp"

CT_BOOST_AUTO_TEST_SUITE(ConnectionTests)

using namespace QtPiDeck::Utilities;

CT_BOOST_AUTO_TEST_CASE(CallOnScopeExit) {
  auto value             = 0;
  {
    auto scope = OnExit([&value] { ++value; });
    value      = 12;
  }

  CT_BOOST_TEST(value == 13);
}

CT_BOOST_AUTO_TEST_SUITE_END()