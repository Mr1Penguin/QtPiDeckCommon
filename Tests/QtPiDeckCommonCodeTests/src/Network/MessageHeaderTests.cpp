#include "BoostUnitTest.hpp"

#include <QDataStream>

#include "Network/DeckDataStream.hpp"
#include "Network/MessageHeader.hpp"
#include "QtDefinitions.hpp"
#include "QtVersion.hpp"

namespace QtPiDeck::Network {
auto operator<<(std::ostream& ostr, const MessageType& right) -> std::ostream& {
  ostr << static_cast<uint32_t>(right);
  return ostr;
}

auto operator<<(std::ostream& ostr, const MessageHeader& right) -> std::ostream& {
  ostr << "MessageHeader{ dataSize: " << right.dataSize << ", messageType: " << right.messageType
       << ", RequestId: " << right.requestId.toStdString().c_str() << " }";
  return ostr;
}

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