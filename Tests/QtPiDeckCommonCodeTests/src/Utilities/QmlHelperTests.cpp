#define BOOST_TEST_MODULE QmlHelpersTests // NOLINT
#include "BoostUnitTest.hpp"

#include "Utilities/Logging.hpp"
#include "Utilities/QmlHelper.hpp"

auto main(int argc, char* argv[]) -> int {
  QtPiDeck::Utilities::initLogging("QmlHelpersTests");
  return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

CT_BOOST_AUTO_TEST_SUITE(QmlHelpersTests)

class EmptyScreen;
class EmptyWindow;

class NoScreenGuiApp {
public:
  static auto primaryScreen() -> EmptyScreen* { return nullptr; }
};
class EmptyScreen {
public:
  [[nodiscard]] auto logicalDotsPerInch() const -> qreal { // NOLINT(readability-convert-member-functions-to-static)
    throw std::logic_error{"Should not be reachable"};     // LCOV_EXCL_LINE
  }
};
class EmptyWindow {};

using QtPiDeck::Utilities::detail::QmlHelperPrivate;

template<class Derived>
using NoScreenQmlHelper = QmlHelperPrivate<Derived, NoScreenGuiApp, EmptyWindow, EmptyScreen>;

template<template<typename> class Parent>
class Helper : public Parent<Helper<Parent>> {};

CT_BOOST_AUTO_TEST_CASE(dpiWithoutScreenShouldBeEqualToMinDpi) {
  Helper<NoScreenQmlHelper> helper;
  CT_BOOST_TEST(helper.dpi() == Helper<NoScreenQmlHelper>::minDpi);
}

template<class Screen>
class GuiAppWithScreen {
public:
  static auto primaryScreen() -> Screen* {
    static Screen screen{};
    return &screen;
  }
};

template<int64_t dpi>
class Screen {
public:
  [[nodiscard]] auto logicalDotsPerInch() const -> qreal { return static_cast<qreal>(dpi); }
};

constexpr uint64_t smallDpi   = 60;
constexpr uint64_t regularDpi = 200;

static_assert(smallDpi < regularDpi);
static_assert(smallDpi < Helper<NoScreenQmlHelper>::minDpi);
static_assert(Helper<NoScreenQmlHelper>::minDpi < regularDpi);

template<class Derived>
using SmallDpiQmlHelper = QmlHelperPrivate<Derived, GuiAppWithScreen<Screen<smallDpi>>, EmptyWindow, Screen<smallDpi>>;

CT_BOOST_AUTO_TEST_CASE(dpiWithScreenAndSmallDpiShouldBeEqualToMinDpi) {
  Helper<SmallDpiQmlHelper> helper;
  CT_BOOST_TEST(helper.dpi() == Helper<NoScreenQmlHelper>::minDpi);
}

template<class Derived>
using RegularDpiQmlHelper =
    QmlHelperPrivate<Derived, GuiAppWithScreen<Screen<regularDpi>>, EmptyWindow, Screen<regularDpi>>;

CT_BOOST_AUTO_TEST_CASE(dpiWithScreenAndRegularDpiShouldBeEqualToRegularDpi) {
  Helper<RegularDpiQmlHelper> helper;
  CT_BOOST_TEST(helper.dpi() == regularDpi);
}

using namespace boost::unit_test;

const auto dpDataSet = data::make({0.0, static_cast<double>(Helper<NoScreenQmlHelper>::baseDpi),
                                   static_cast<double>(regularDpi), 1000.0}) ^
                       data::make({0.0, static_cast<double>(regularDpi), 250.0, 1250.0});

CT_BOOST_DATA_TEST_CASE(dpShouldBeCalculatedCorrectly, dpDataSet, input, expected) {
  Helper<RegularDpiQmlHelper> helper;
  CT_BOOST_TEST(helper.dp(input, helper.dpi()) == expected);
}

const auto& spDataSet = dpDataSet; // same until font scale is used in sp(...) function

CT_BOOST_DATA_TEST_CASE(spShouldBeCalculatedCorrectly, spDataSet, input, expected) {
  Helper<RegularDpiQmlHelper> helper;
  CT_BOOST_TEST(helper.sp(input, helper.dpi()) == expected);
}
CT_BOOST_AUTO_TEST_SUITE_END()
