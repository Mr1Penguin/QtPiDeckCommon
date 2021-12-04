#pragma once

#include "boost/tti/has_member_function.hpp"

#include <QString>
#include <QTcpSocket>

#include "Network/DeckDataStream.hpp"
#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Utilities {
BOOST_TTI_HAS_MEMBER_FUNCTION(messageSize)
template<class T>
constexpr bool has_messageSize = has_member_function_messageSize<const T, uint64_t>::value;
BOOST_TTI_HAS_MEMBER_FUNCTION(messageHeader)
template<class T>
constexpr bool has_messageHeader =
    has_member_function_messageHeader<const T, Network::MessageHeader, boost::mpl::vector<const QString&>>::value;

template<class T>
constexpr bool is_valid_message = has_messageSize<T>&& has_messageHeader<T>;

inline auto headerSize(const Network::MessageHeader& header) -> std::size_t {
  constexpr auto qstringHeaderSize = std::size_t{4};
  constexpr auto charSize          = std::size_t{sizeof(decltype(std::declval<QChar>().unicode()))};
  return sizeof(Network::MessageHeader::dataSize) + sizeof(Network::MessageHeader::messageType) +
         header.requestId.size() * charSize + qstringHeaderSize;
}

template<class Socket = QTcpSocket>
inline void sendHeader(const Network::MessageHeader& header, Socket& socket) {
  QByteArray tmp;
  tmp.reserve(headerSize(header));
  Network::DeckDataStream outStream{&tmp, QIODevice::WriteOnly};
  outStream << header;
  [[maybe_unused]] const auto bytes = socket.write(tmp);
}

template<class Message, class Socket = QTcpSocket>
void sendMessage(const Message& message, Socket& socket, const QString& requestId) {
  static_assert(is_valid_message<Message>);
  const auto header = message.messageHeader(requestId);
  QByteArray tmp;
  tmp.reserve(headerSize(header) + header.dataSize);
  Network::DeckDataStream outStream{&tmp, QIODevice::WriteOnly};
  outStream << header << message;
  [[maybe_unused]] const auto bytes = socket.write(tmp);
}
}
