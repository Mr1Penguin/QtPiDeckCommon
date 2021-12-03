#include "BoostUnitTest.hpp"

#include "Utilities/Literals.hpp"
#include "Utilities/MessageUtils.hpp"

CT_BOOST_AUTO_TEST_SUITE(MessageUtilsTests)
using namespace QtPiDeck::Utilities;
using namespace QtPiDeck::Network;
struct Valid {
  auto messageSize() const -> uint64_t { return 0l; }
  auto messageHeader(const QString& /*requestId*/) const -> MessageHeader { return {}; }
};

CT_BOOST_AUTO_TEST_CASE(is_hasMessageSizeValid) { CT_BOOST_TEST(has_messageSize<Valid>); }
CT_BOOST_AUTO_TEST_CASE(is_hasMessageHeaderValid) { CT_BOOST_TEST(has_messageHeader<Valid>); }
CT_BOOST_AUTO_TEST_CASE(is_valid_messageValid) { CT_BOOST_TEST(is_valid_message<Valid>); }

struct InvalidNoMessageSize {
  auto messageHeader(const QString& /*requestId*/) const -> MessageHeader { return {}; }
};

struct InvalidNoMessageHeader {
  auto messageSize() const -> uint64_t { return 0l; }
  auto messageHeader() const -> MessageHeader { return {}; }
};

CT_BOOST_AUTO_TEST_CASE(is_valid_messageInvalidSize) { CT_BOOST_TEST(!is_valid_message<InvalidNoMessageSize>); }
CT_BOOST_AUTO_TEST_CASE(is_valid_messageInvalidHeader) { CT_BOOST_TEST(!is_valid_message<InvalidNoMessageHeader>); }

CT_BOOST_AUTO_TEST_CASE(headerSizeReturnsCorrectSizeNoRequestId) { CT_BOOST_TEST(headerSize({}) == 16); }
CT_BOOST_AUTO_TEST_CASE(headerSizeReturnsCorrectSizeRequestId) {
  CT_BOOST_TEST(headerSize({0, MessageType::Ping, "Hello"}) == 26);
}

CT_BOOST_AUTO_TEST_SUITE_END()