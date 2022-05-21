#include "BoostUnitTest.hpp"

#include <QDataStream>

#include "Network/DeckDataStream.hpp"
#include "Network/MessageHeader.hpp"
#include "QtDefinitions.hpp"
#include "QtVersion.hpp"

#include "printers.hpp"

namespace QtPiDeck::Network {
auto operator==(const QtPiDeck::Network::MessageHeader& left, const QtPiDeck::Network::MessageHeader& right) -> bool {
  return left.dataSize == right.dataSize && left.messageType == right.messageType &&
         left.requestId == right.requestId;
}
}

BOOST_AUTO_TEST_SUITE(MessageHeaderTests) // NOLINT
using namespace QtPiDeck::Network;

BOOST_AUTO_TEST_CASE(shouldBeSerializedAndDeserialized) // NOLINT
{
  const auto messageHeader = MessageHeader::make(0, MessageType::Pong, CT_QStringLiteral("a-random-id"));
  QByteArray qba;
  QDataStream in{&qba, DeckDataStream::OpenModeFlag::WriteOnly};
  in << messageHeader;
  QDataStream out{&qba, DeckDataStream::OpenModeFlag::ReadOnly};
  MessageHeader outMessageHeader{};
  out >> outMessageHeader;
  BOOST_TEST(outMessageHeader == messageHeader); // NOLINT
}

BOOST_AUTO_TEST_CASE(shouldGetNameForKnownMessageType) // NOLINT
{
  constexpr auto v = getMessageTypeName(MessageType::Dummy);
  BOOST_TEST(v != nullptr); // NOLINT
}

BOOST_AUTO_TEST_CASE(shouldThrowForUnknownMessageType) // NOLINT
{
  BOOST_CHECK_THROW(getMessageTypeName(MessageType::Reserved), std::out_of_range); // NOLINT
}

BOOST_AUTO_TEST_SUITE_END() // NOLINT

//#include "MessageHeaderTests.moc"
