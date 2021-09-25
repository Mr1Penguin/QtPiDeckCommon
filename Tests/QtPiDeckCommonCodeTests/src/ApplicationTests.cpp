#define BOOST_TEST_MODULE ApplicationTests // NOLINT
#include "BoostUnitTest.hpp"

#include "Application.hpp"
#include "Utilities/Logging.hpp"

auto main(int argc, char* argv[]) -> int {
  QtPiDeck::Utilities::initLogging("ApplicationTests");
  return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

namespace {
class TestApplication final : public QtPiDeck::Application {
protected:
  auto mainPage() -> QUrl final { return QUrl(); }
};
}

CT_BOOST_AUTO_TEST_SUITE(ApplicationTests)
CT_BOOST_AUTO_TEST_CASE(CurrentFieldShouldPointToActiveApplicationAfterCreation) {
  const auto* current = TestApplication::current();
  TestApplication app;
  CT_BOOST_TEST(TestApplication::current() != current);
  CT_BOOST_TEST(TestApplication::current() == &app);
}
CT_BOOST_AUTO_TEST_SUITE_END()
