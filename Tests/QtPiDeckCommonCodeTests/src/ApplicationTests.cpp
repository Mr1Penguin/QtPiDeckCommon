#define BOOST_TEST_MODULE ApplicationTests
#include "BoostUnitTest.hpp"

#include "Application.hpp"

namespace {
class TestApplication final : public QtPiDeck::Application {
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