#include "BoostUnitTest.hpp"

#include "Network/Messages/HelloMessageSerialization.hpp"
#include "QStringLiteral.hpp"

BOOST_AUTO_TEST_SUITE(sss) // NOLINT

using namespace QtPiDeck::Network::Messages;

namespace {
const auto interfaceVersionKey = QLatin1String{"interfaceVersion"};
const auto qcharSizeKey        = QLatin1String{"qcharSize"};
}

BOOST_AUTO_TEST_CASE(ShouldCreateJsonString) // NOLINT
{
  const auto message = Hello{12, 34};
  const auto json    = prepareJsonString(message);
  BOOST_TEST(json.contains(interfaceVersionKey));     // NOLINT
  BOOST_TEST(json.contains(CT_QStringLiteral("12"))); // NOLINT
  BOOST_TEST(json.contains(qcharSizeKey));            // NOLINT
  BOOST_TEST(json.contains(CT_QStringLiteral("34"))); // NOLINT
}

BOOST_AUTO_TEST_CASE(ShouldCreateFromJsonString) // NOLINT
{
  const auto json = CT_QStringLiteral("{\"") + interfaceVersionKey + CT_QStringLiteral("\": 12, \"") + qcharSizeKey +
                    CT_QStringLiteral("\": 34 }");
  const auto message = prepareHelloMessage(json);
  BOOST_TEST(message.interfaceVersion == 12); // NOLINT
  BOOST_TEST(message.qcharSize == 34);        // NOLINT
}

BOOST_AUTO_TEST_SUITE_END() // NOLINT
