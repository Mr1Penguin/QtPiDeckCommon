#if defined(_MSC_VER) && defined(__clang__) && defined(__INTELLISENSE__)
#pragma push_macro("__clang__")
#undef __clang__
#include <boost/test/unit_test.hpp>
#pragma pop_macro("__clang__")
#else
#include <boost/test/unit_test.hpp>
#endif

#include <QDataStream>

#include "Network/MessageHeader.hpp"

namespace boost {
bool operator==(const QtPiDeck::Network::MessageHeader& left, const QtPiDeck::Network::MessageHeader& right) {
  return left.dataSize == right.dataSize && left.messageType == right.messageType && left.requestId == right.requestId;
}
}

namespace QtPiDeck::Network {
std::ostream& operator<<(std::ostream& ostr, const MessageHeader& right) {
  ostr << "MessageHeader{ dataSize: " << right.dataSize << ", messageType: " << static_cast<uint32_t>(right.messageType)
       << ", RequestId: " << right.requestId.toStdString().c_str() << " }";
  return ostr;
}
}

#if !defined(_MSC_VER) && 0
namespace boost::test_tools::tt_detail {
std::ostream& boost_test_print_type(std::ostream& ostr, const QtPiDeck::Network::MessageHeader& right) {
  ostr << "MessageHeader{ dataSize: " << right.dataSize << ", messageType: " << static_cast<uint32_t>(right.messageType)
       << ", RequestId: " << right.requestId.toStdString().c_str() << " }";
  return ostr;
}
}
#endif

BOOST_AUTO_TEST_SUITE(MessageHeaderTests)
using namespace QtPiDeck::Network;

BOOST_AUTO_TEST_CASE(serialize_and_deserialize)
{
  const MessageHeader messageHeader{0, MessageType::Pong, QStringLiteral("a-random-id")};
  QByteArray qba;
  QDataStream in{&qba, QIODevice::WriteOnly};
  in << messageHeader;
  QDataStream out{&qba, QIODevice::ReadOnly};
  MessageHeader outMessageHeader{};
  out >> outMessageHeader;
  BOOST_TEST(outMessageHeader == messageHeader);
}

BOOST_AUTO_TEST_SUITE_END()

//#include "MessageHeaderTests.moc"
