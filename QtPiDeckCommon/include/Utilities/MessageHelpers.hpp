#pragma once

#include <QTcpSocket>
#include "DeckDataStream.hpp"
#include "MessageTraits.hpp"

inline void sendHeader(const MessageHeader& header, QTcpSocket& socket) {
  QByteArray tmp;
  tmp.reserve(sizeof(MessageHeader));
  DeckDataStream outStream{&tmp, QIODevice::WriteOnly};
  outStream << header;
  socket.write(tmp);
}

template<class Message>
inline void sendMessage(const Message& message, QTcpSocket& socket) {
  static_assert(is_valid_message<Message>);
  const auto header = message.messageHeader();
  QByteArray tmp;
  tmp.reserve(sizeof(MessageHeader) + header.dataSize);
  DeckDataStream outStream{&tmp, QIODevice::WriteOnly};
  outStream << header << message;
  socket.write(tmp);
}
