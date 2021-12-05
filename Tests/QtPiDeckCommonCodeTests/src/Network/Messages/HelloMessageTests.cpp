#include "BoostUnitTest.hpp"

#include <QDataStream>

#include "Network/DeckDataStream.hpp"
#include "Network/Messages/HelloMessage.hpp"
#include "QtDefinitions.hpp"
#include "QtVersion.hpp"

namespace QtPiDeck::Network {
auto operator<<(std::ostream& ostr, const MessageType& right) -> std::ostream&;
inline namespace Messages {
auto operator<<(std::ostream& ostr, const Hello& right) -> std::ostream& {
  ostr << "Hello{ interfaceVersion: " << right.interfaceVersion << ", qcharSize: " << right.qcharSize << " }";
  return ostr;
}

auto operator==(const QtPiDeck::Network::Hello& left, const QtPiDeck::Network::Hello& right) -> bool {
  return left.interfaceVersion == right.interfaceVersion && left.qcharSize == right.qcharSize;
}
}
}

CT_BOOST_AUTO_TEST_SUITE(HelloMessageTests)
using namespace QtPiDeck::Network;

CT_BOOST_AUTO_TEST_CASE(shouldBeSerializedAndDeserialized) {
  const auto helloMessage = Messages::Hello{};
  auto qba                = QByteArray{};
  auto in                 = QDataStream{&qba, DeckDataStream::OpenModeFlag::WriteOnly};
  in << helloMessage;
  auto out        = QDataStream{&qba, DeckDataStream::OpenModeFlag::ReadOnly};
  auto outMessage = Messages::Hello{0, 0};
  out >> outMessage;
  CT_BOOST_TEST(outMessage == helloMessage);
}

CT_BOOST_AUTO_TEST_CASE(getAppropiateHeader) {
  const auto helloMessage = Messages::Hello{};
  const auto requestId    = CT_QStringLiteral("request");
  const auto header       = helloMessage.messageHeader(requestId);
  CT_BOOST_TEST(header.dataSize == helloMessage.messageSize());
  CT_BOOST_TEST(header.messageType == MessageType::Hello);
  CT_BOOST_TEST(header.requestId == requestId);
}

CT_BOOST_AUTO_TEST_SUITE_END()
