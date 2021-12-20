#include "Services/MessageSender.hpp"

#include "Network/DeckDataStream.hpp"
#include "Utilities/ISerializable.hpp"

#include <boost/pool/pool.hpp>

namespace QtPiDeck::Services {
namespace {
auto headerSize(const Network::MessageHeader& header) -> std::size_t {
  constexpr auto qstringHeaderSize = std::size_t{4};
  constexpr auto charSize          = std::size_t{sizeof(decltype(std::declval<QChar>().unicode()))};
  return sizeof(Network::MessageHeader::dataSize) + sizeof(Network::MessageHeader::messageType) +
         header.requestId.size() * charSize + qstringHeaderSize;
}
}

void MessageSender::send(const Network::Messages::Message& message, const QString& id) {
  const auto& asSerializable = dynamic_cast<const Utilities::ISerializable&>(message);
  const auto header          = message.messageHeader(id);
  QByteArray buffer;
  buffer.reserve(headerSize(header) + header.dataSize);
  Network::DeckDataStream outStream{&buffer, QIODevice::WriteOnly};
  header.write(outStream);
  asSerializable.write(outStream);
  send(buffer);
}

void MessageSender::send(const Network::MessageHeader& header) {
  QByteArray buffer;
  buffer.reserve(headerSize(header));
  Network::DeckDataStream outStream{&buffer, QIODevice::WriteOnly};
  header.write(outStream);
  send(buffer);
}

void MessageSender::send(const QByteArray& data) { service<ISocketHolder>()->requestWrite(data); }
}
