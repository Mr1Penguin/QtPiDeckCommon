// NOLINTNEXTLINE
#define BOOST_TEST_MODULE LiteralTests
#include "BoostUnitTest.hpp"

#include "Utilities/Literals.hpp"
#include "Utilities/Logging.hpp"

auto main(int argc, char* argv[]) -> int {
  QtPiDeck::Utilities::initLogging("LiteralTests");
  return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

auto operator<<(std::ostream& ostr, const QUrl& right) -> std::ostream& {
  ostr << "QUrl[" << right.toString().toStdString() << "]";
  return ostr;
}

using namespace QtPiDeck::Utilities::literals;

CT_BOOST_AUTO_TEST_SUITE(LiteralTests)
CT_BOOST_AUTO_TEST_CASE(qsReturnsQString) {
  const auto literal = "str"_qs;
  const auto type    = QString{"str"};
  CT_BOOST_TEST(literal == type);
}

CT_BOOST_AUTO_TEST_CASE(qlsReturnsQLatin1String) {
  const auto literal = "str"_qls;
  const auto type    = QLatin1String{"str"};
  CT_BOOST_TEST(literal == type);
}

CT_BOOST_AUTO_TEST_CASE(qurlReturnsQUrl) {
  const auto literal = "str"_qurl;
  const auto type    = QUrl{QLatin1String{"str"}};
  CT_BOOST_TEST(literal == type);
}

CT_BOOST_AUTO_TEST_SUITE_END()