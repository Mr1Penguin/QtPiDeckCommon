#include "BoostUnitTest.hpp"

#include <QDataStream>

#include "Network/DeckDataStream.hpp"
#include "Network/Messages/HelloMessage.hpp"
#include "QtVersion.hpp"
#include "Utilities/Literals.hpp"

namespace QtPiDeck::Network {
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
using namespace QtPiDeck::Utilities;

CT_BOOST_AUTO_TEST_CASE(shouldBeSerializedAndDeserialized) {
  const Messages::Hello helloMessage;
  QByteArray qba;
  QDataStream in{&qba, DeckDataStream::OpenModeFlag::WriteOnly};
  in << helloMessage;
  QDataStream out{&qba, DeckDataStream::OpenModeFlag::ReadOnly};
  Messages::Hello outMessage{0, 0};
  out >> outMessage;
  CT_BOOST_TEST(outMessage == helloMessage);
}

CT_BOOST_AUTO_TEST_SUITE_END()

//#include "MessageHeaderTests.moc"