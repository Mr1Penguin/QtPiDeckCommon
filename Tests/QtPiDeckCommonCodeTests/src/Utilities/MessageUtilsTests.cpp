#include "BoostUnitTest.hpp"

#include "Utilities/Literals.hpp"
#include "Utilities/MessageUtils.hpp"

namespace QtPiDeck::Network {
auto operator<<(std::ostream& ostr, const MessageType& right) -> std::ostream&;
auto operator<<(std::ostream& ostr, const MessageHeader& right) -> std::ostream&;
auto operator==(const QtPiDeck::Network::MessageHeader& left, const QtPiDeck::Network::MessageHeader& right) -> bool;
}

CT_BOOST_AUTO_TEST_SUITE(MessageUtilsTests)
using namespace QtPiDeck::Utilities;
using namespace QtPiDeck::Network;

struct Valid {
  [[nodiscard]] auto messageSize() const -> uint64_t // NOLINT
  {
    return 0;
  }
  [[nodiscard]] auto messageHeader(const QString& /*requestId*/) const -> MessageHeader // NOLINT
  {
    return {};
  }
};

CT_BOOST_AUTO_TEST_CASE(is_hasMessageSizeValid) { CT_BOOST_TEST(has_messageSize<Valid>); }
CT_BOOST_AUTO_TEST_CASE(is_hasMessageHeaderValid) { CT_BOOST_TEST(has_messageHeader<Valid>); }
CT_BOOST_AUTO_TEST_CASE(is_valid_messageValid) { CT_BOOST_TEST(is_valid_message<Valid>); }

struct InvalidNoMessageSize {
  [[nodiscard]] auto messageHeader(const QString& /*requestId*/) const -> MessageHeader // NOLINT
  {
    return {};
  }
};

struct InvalidNoMessageHeader {
  [[nodiscard]] auto messageSize() const -> uint64_t // NOLINT
  {
    return 0;
  }
  [[nodiscard]] auto messageHeader() const -> MessageHeader // NOLINT
  {
    return {};
  }
};

CT_BOOST_AUTO_TEST_CASE(is_valid_messageInvalidSize) { CT_BOOST_TEST(!is_valid_message<InvalidNoMessageSize>); }
CT_BOOST_AUTO_TEST_CASE(is_valid_messageInvalidHeader) { CT_BOOST_TEST(!is_valid_message<InvalidNoMessageHeader>); }

CT_BOOST_AUTO_TEST_CASE(headerSizeReturnsCorrectSizeNoRequestId) { CT_BOOST_TEST(headerSize({}) == 16); }
CT_BOOST_AUTO_TEST_CASE(headerSizeReturnsCorrectSizeRequestId) {
  CT_BOOST_TEST(headerSize({0, MessageType::Ping, "Hello"_qs}) == 26);
}

class MockSocket {
public:
  [[nodiscard]] auto byteArray() const -> const QByteArray& { return m_byteArray; }
  [[nodiscard]] auto write(const QByteArray& data) -> quint64 {
    m_byteArray = data;
    return data.size();
  }

private:
  QByteArray m_byteArray;
};

CT_BOOST_AUTO_TEST_CASE(sendHeaderSuccess) {
  const auto header = MessageHeader{0, MessageType::Ping, "Hello"_qs};
  auto socket       = MockSocket{};
  sendHeader(header, socket);
  const auto& byteArray = socket.byteArray();
  CT_BOOST_TEST(byteArray.size() == 26);
  auto parsed = MessageHeader{};
  auto stream = DeckDataStream{byteArray};
  stream >> parsed;
  CT_BOOST_TEST(parsed == header);
}

struct Message {
  uint8_t c;
  [[nodiscard]] auto messageSize() const -> uint64_t // NOLINT
  {
    return 1;
  }
  [[nodiscard]] auto messageHeader(const QString& requestId) const -> MessageHeader // NOLINT
  {
    return {1, MessageType::Ping, requestId};
  }
};

auto operator<<(QDataStream& str, const Message& message) -> QDataStream& {
  str << message.c;
  return str;
}
auto operator>>(QDataStream& str, Message& message) -> QDataStream& {
  str >> message.c;
  return str;
}

auto operator<<(std::ostream& ostr, const Message& right) -> std::ostream& {
  ostr << "Message{ c: " << right.c << " }";
  return ostr;
}

auto operator==(const Message& left, const Message& right) -> bool { return left.c == right.c; }

CT_BOOST_AUTO_TEST_CASE(sendMessageSuccess) {
  const auto requestId = "Hello"_qs;
  const auto message   = Message{'o'};
  auto socket          = MockSocket{};
  sendMessage(message, socket, requestId);
  const auto& byteArray = socket.byteArray();
  CT_BOOST_TEST(byteArray.size() == 27);
  auto parsedHeader = MessageHeader{};
  auto parsedMessage = Message{};
  auto stream = DeckDataStream{byteArray};
  stream >> parsedHeader;
  CT_BOOST_TEST(parsedHeader == message.messageHeader(requestId));
  stream >> parsedMessage;
  CT_BOOST_TEST(parsedMessage == message);
}

CT_BOOST_AUTO_TEST_SUITE_END()
