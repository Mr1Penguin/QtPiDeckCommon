#include "BoostUnitTest.hpp"

#include <QDataStream>

#include "Network/DeckDataStream.hpp"
#include "Network/MessageHeader.hpp"
#include "QtDefinitions.hpp"
#include "QtVersion.hpp"

#include "printers.hpp"

namespace QtPiDeck::Network {
auto operator==(const QtPiDeck::Network::MessageHeader& left, const QtPiDeck::Network::MessageHeader& right) -> bool {
  return left.dataSize == right.dataSize && left.messageType == right.messageType && left.requestId == right.requestId;
}
}

CT_BOOST_AUTO_TEST_SUITE(MessageHeaderTests)
using namespace QtPiDeck::Network;

CT_BOOST_AUTO_TEST_CASE(shouldBeSerializedAndDeserialized) {
  const auto messageHeader = []() {
    auto header        = MessageHeader{};
    header.dataSize    = 0;
    header.messageType = MessageType::Pong;
    header.requestId   = CT_QStringLiteral("a-random-id");
    return header;
  }();
  QByteArray qba;
  QDataStream in{&qba, DeckDataStream::OpenModeFlag::WriteOnly};
  messageHeader.write(in);
  QDataStream out{&qba, DeckDataStream::OpenModeFlag::ReadOnly};
  MessageHeader outMessageHeader{};
  outMessageHeader.read(out);
  CT_BOOST_TEST(outMessageHeader == messageHeader);
}

CT_BOOST_AUTO_TEST_SUITE_END()

//#include "MessageHeaderTests.moc"