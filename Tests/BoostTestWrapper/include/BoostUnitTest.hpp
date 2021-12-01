#if defined(_MSC_VER) && defined(__clang__) && defined(__INTELLISENSE__)
#pragma push_macro("__clang__")
#undef __clang__
#include <boost/test/unit_test.hpp>
#pragma pop_macro("__clang__")
#else
#include <boost/test/unit_test.hpp>
#endif

#include <boost/test/data/test_case.hpp>

#define CT_BOOST_AUTO_TEST_SUITE(suite) BOOST_AUTO_TEST_SUITE(suite) // NOLINT
#define CT_BOOST_TEST(test) BOOST_TEST(test) // NOLINT
#define CT_BOOST_AUTO_TEST_SUITE_END() BOOST_AUTO_TEST_SUITE_END() // NOLINT
#define CT_BOOST_AUTO_TEST_CASE(testcase) BOOST_AUTO_TEST_CASE(testcase) // NOLINT
#define CT_BOOST_FIXTURE_TEST_SUITE(suite, fixture) BOOST_FIXTURE_TEST_SUITE(suite, fixture) // NOLINT
#define CT_BOOST_FIXTURE_TEST_CASE(testcase, fixture) BOOST_FIXTURE_TEST_CASE(testcase, fixture) // NOLINT
#define CT_BOOST_CHECK_THROW(test, exception) BOOST_CHECK_THROW(test, exception) // NOLINT
#define CT_BOOST_DATA_TEST_CASE(testcase, data, var1, var2) BOOST_DATA_TEST_CASE(testcase, data, var1, var2) // NOLINT
