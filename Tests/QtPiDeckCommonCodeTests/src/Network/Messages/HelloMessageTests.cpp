#include "BoostUnitTest.hpp"

#include <QDataStream>

#include "Network/DeckDataStream.hpp"
#include "Network/Messages/HelloMessage.hpp"
#include "QtDefinitions.hpp"
#include "QtVersion.hpp"

#include "printers.hpp"

namespace QtPiDeck::Network {
inline namespace Messages {
auto operator==(const QtPiDeck::Network::Hello& left, const QtPiDeck::Network::Hello& right) -> bool {
  return left.interfaceVersion == right.interfaceVersion && left.qcharSize == right.qcharSize;
}
}
}

BOOST_AUTO_TEST_SUITE(HelloMessageTests) // NOLINT
using namespace QtPiDeck::Network;

BOOST_AUTO_TEST_CASE(shouldBeSerializedAndDeserialized) // NOLINT
{
  const auto helloMessage = Messages::Hello{};
  auto qba                = QByteArray{};
  auto in                 = QDataStream{&qba, DeckDataStream::OpenModeFlag::WriteOnly};
  in << helloMessage;
  auto out                    = QDataStream{&qba, DeckDataStream::OpenModeFlag::ReadOnly};
  auto outMessage             = Messages::Hello{};
  outMessage.interfaceVersion = 0;
  outMessage.qcharSize        = 0;
  out >> outMessage;
  BOOST_TEST(outMessage == helloMessage); // NOLINT
}

BOOST_AUTO_TEST_CASE(getAppropiateHeader) // NOLINT
{
  const auto helloMessage = Messages::Hello{};
  const auto requestId    = CT_QStringLiteral("request");
  const auto header       = helloMessage.messageHeader(requestId);
  BOOST_TEST(header.dataSize() == helloMessage.messageSize()); // NOLINT
  BOOST_TEST(header.messageType() == MessageType::Hello);      // NOLINT
  BOOST_TEST(header.requestId() == requestId);                 // NOLINT
}

BOOST_AUTO_TEST_SUITE_END() // NOLINT
