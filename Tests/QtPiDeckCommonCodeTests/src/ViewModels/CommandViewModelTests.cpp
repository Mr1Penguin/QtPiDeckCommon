#include "BoostUnitTest.hpp"

#include <QQmlEngine>

#include "ViewModels/CommandViewModel.hpp"
#include "Utilities/Logging.hpp"
#include "SignalCatcher.hpp"
#include "Utilities/Literals.hpp"

CT_BOOST_AUTO_TEST_SUITE(CommandViewModelTests)
using namespace QtPiDeck::ViewModels;
using namespace QtPiDeck::Utilities::literals;
using namespace QtPiDeck::Tests;

CT_BOOST_AUTO_TEST_CASE(textChangedSignalShouldBeEmitedNewValue) {
  CommandViewModel vm;
  const auto text = "Test"_qs;
  vm.setText(text);
  const auto text2 = "Test2"_qs;
  SignalCatcher sc(&vm, &CommandViewModel::textChanged);
  vm.setText(text2);
  CT_BOOST_TEST(sc.count() == 1);
}

CT_BOOST_AUTO_TEST_CASE(textChangedSignalShouldNotBeEmitedSameValue) {
  CommandViewModel vm;
  const auto text = "Test"_qs;
  vm.setText(text);
  SignalCatcher sc(&vm, &CommandViewModel::textChanged);
  vm.setText(text);
  CT_BOOST_TEST(sc.count() == 0);
}

CT_BOOST_AUTO_TEST_CASE(imagePathChangedSignalShouldBeEmitedNewValue) {
  CommandViewModel vm;
  const auto text = "Test"_qs;
  vm.setImagePath(text);
  const auto text2 = "Test2"_qs;
  SignalCatcher sc(&vm, &CommandViewModel::imagePathChanged);
  vm.setImagePath(text2);
  CT_BOOST_TEST(sc.count() == 1);
}

CT_BOOST_AUTO_TEST_CASE(imagePathChangedSignalShouldNotBeEmitedSameValue) {
  CommandViewModel vm;
  const auto text = "Test"_qs;
  vm.setImagePath(text);
  SignalCatcher sc(&vm, &CommandViewModel::imagePathChanged);
  vm.setImagePath(text);
  CT_BOOST_TEST(sc.count() == 0);
}

CT_BOOST_AUTO_TEST_SUITE_END()