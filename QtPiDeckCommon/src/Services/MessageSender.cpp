#include "Services/MessageSender.hpp"

#include <cassert>

#include "Network/DeckDataStream.hpp"
#include "Utilities/ISerializable.hpp"

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
  const auto size            = headerSize(header) + header.dataSize;
  QByteArray buffer;
#if QT_VERSION_MAJOR == 5
  assert(size < std::numeric_limits<int>::max()); // NOLINT
  buffer.reserve(static_cast<int>(size));
#else
  buffer.reserve(size);
#endif
  Network::DeckDataStream outStream{&buffer, QIODevice::WriteOnly};
  header.write(outStream);
  asSerializable.write(outStream);
  send(buffer);
}

void MessageSender::send(const Network::MessageHeader& header) {
  const auto size = headerSize(header);
  QByteArray buffer;
#if QT_VERSION_MAJOR == 5
  assert(size < std::numeric_limits<int>::max()); // NOLINT
  buffer.reserve(static_cast<int>(size));
#else
  buffer.reserve(size);
#endif
  Network::DeckDataStream outStream{&buffer, QIODevice::WriteOnly};
  header.write(outStream);
  send(buffer);
}

void MessageSender::send(const QByteArray& data) { service<ISocketHolder>()->requestWrite(data); }
}
