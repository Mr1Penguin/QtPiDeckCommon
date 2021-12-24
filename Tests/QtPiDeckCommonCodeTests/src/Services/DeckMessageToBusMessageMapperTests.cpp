#include "BoostUnitTest.hpp"

#include "Services/DeckMessageToBusMessageMapper.hpp"

#include "printers.hpp"

CT_BOOST_AUTO_TEST_SUITE(DeckMessageToBusMessageMapperTests)

using namespace QtPiDeck::Services;
using namespace QtPiDeck::Network;

CT_BOOST_AUTO_TEST_CASE(returnNulloptForUnknownValue) {
  const auto mapper = DeckMessageToBusMessageMapper{};
  constexpr auto type = MessageType::Reserved;
  CT_BOOST_TEST(mapper.getBusMessageType(type) == std::nullopt);
}

CT_BOOST_AUTO_TEST_SUITE_END()