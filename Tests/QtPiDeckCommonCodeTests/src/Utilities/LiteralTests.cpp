#include "BoostUnitTest.hpp"

#include "Utilities/Literals.hpp"
#include "Utilities/Logging.hpp"

auto operator<<(std::ostream& ostr, const QUrl& right) -> std::ostream& {
  ostr << "QUrl[" << right.toString().toStdString() << "]";
  return ostr;
}

using namespace QtPiDeck::Utilities::literals;

CT_BOOST_AUTO_TEST_SUITE(LiteralTests)

CT_BOOST_AUTO_TEST_CASE(qlsReturnsQLatin1String) {
  const auto literal = "str"_qls;
  const auto type    = QLatin1String{"str"};
  CT_BOOST_TEST(literal == type);
}
CT_BOOST_AUTO_TEST_SUITE_END()