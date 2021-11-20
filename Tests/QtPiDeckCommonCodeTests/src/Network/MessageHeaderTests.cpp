#include <QDataStream>

// NOLINTNEXTLINE
#define BOOST_TEST_MODULE MessageHeaderTests
#include "BoostUnitTest.hpp"

#include "Network/DeckDataStream.hpp"
#include "Network/MessageHeader.hpp"
#include "Utilities/Literals.hpp"

#include "Utilities/Logging.hpp"

auto main(int argc, char* argv[]) -> int {
  QtPiDeck::Utilities::initLogging("MessageHeaderTests");
  return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

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
using namespace QtPiDeck::Utilities;

CT_BOOST_AUTO_TEST_CASE(shouldBeSerializedAndDeserialized) {
  const MessageHeader messageHeader{0, MessageType::Pong, QStringLiteral("a-random-id")};
  QByteArray qba;
  QDataStream in{&qba, DeckDataStream::OpenModeFlag::WriteOnly};
  in << messageHeader;
  QDataStream out{&qba, DeckDataStream::OpenModeFlag::ReadOnly};
  MessageHeader outMessageHeader{};
  out >> outMessageHeader;
  CT_BOOST_TEST(outMessageHeader == messageHeader);
}

CT_BOOST_AUTO_TEST_CASE(shouldGetHeaderWithZeroDataSize) {
  const auto messageType   = MessageType::Ping;
  const auto requestId     = "PingId"_qs;
  const auto messageHeader = getEmptyMessageHeader(messageType, requestId);
  CT_BOOST_TEST(messageHeader.dataSize == 0);
  CT_BOOST_TEST(messageHeader.messageType == messageType);
  CT_BOOST_TEST(messageHeader.requestId == requestId);
}

CT_BOOST_AUTO_TEST_SUITE_END()

//#include "MessageHeaderTests.moc"