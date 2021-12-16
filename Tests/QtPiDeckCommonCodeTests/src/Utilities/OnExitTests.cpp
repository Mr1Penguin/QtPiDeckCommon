#include "BoostUnitTest.hpp"

#include <map>

#include "Utilities/OnExit.hpp"

CT_BOOST_AUTO_TEST_SUITE(ConnectionTests)

using namespace QtPiDeck::Utilities;

constexpr auto newValue = 12;

CT_BOOST_AUTO_TEST_CASE(CallOnScopeExit) {
  auto value             = 0;
  {
    auto scope = OnExit([&value] { ++value; });
    value      = newValue;
  }

  CT_BOOST_TEST(value == 13);
}
CT_BOOST_AUTO_TEST_SUITE_END()