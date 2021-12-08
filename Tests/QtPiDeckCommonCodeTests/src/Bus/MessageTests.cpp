#include "BoostUnitTest.hpp"

#include <QByteArray>
#include <QString>

#include "Bus/Message.hpp"

CT_BOOST_AUTO_TEST_SUITE(ConnectionTests)
using namespace QtPiDeck::Bus;

CT_BOOST_AUTO_TEST_CASE(createEmptyMessage) {
  const auto type    = uint64_t{1};
  const auto message = createMessage(type);
  CT_BOOST_TEST(message.messageType == type);
  CT_BOOST_TEST(message.payload.isEmpty());
}

CT_BOOST_AUTO_TEST_CASE(createMessageOneArgument) {
  const auto message = createMessage(1, uint32_t{42});
  CT_BOOST_TEST(message.payload.size() == sizeof(uint32_t));
}

CT_BOOST_AUTO_TEST_CASE(createMessageThreeArguments) {
  const auto message = createMessage(1, uint32_t{42}, uint8_t{1}, QStringLiteral("12"));
  CT_BOOST_TEST(message.payload.size() == sizeof(uint32_t) + sizeof(uint8_t) + 8);
}

CT_BOOST_AUTO_TEST_SUITE_END()