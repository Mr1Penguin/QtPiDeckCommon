#include "Services/MessageSender.hpp"

#include <cassert>

#include "Network/DeckDataStream.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/QStringUtils.hpp"

namespace QtPiDeck::Services {
namespace {
auto headerSize(const Network::MessageHeader& header) -> std::size_t {
  return sizeof(Network::MessageHeader::dataSize) + sizeof(Network::MessageHeader::messageType) +
         Utilities::qstringRawSize(header.requestId);
}

auto prepareBuffer(std::size_t size) -> QByteArray {
  auto buffer = QByteArray{};
#if QT_VERSION_MAJOR == 5
  using argType = int;
#else
  using argType = qsizetype;
#endif
  assert(size < static_cast<std::size_t>(std::numeric_limits<argType>::max())); // NOLINT
  buffer.reserve(static_cast<argType>(size));
  return buffer;
}

template<class... T>
auto createBuffer(std::size_t size, T... args) -> QByteArray {
  auto buffer = prepareBuffer(size);
  Network::DeckDataStream outStream{&buffer, QIODevice::WriteOnly};
  (outStream << ... << args);
  return buffer;
}

auto totalPayloadSize(const Network::MessageHeader& header) -> std::size_t {
  return headerSize(header) + header.dataSize;
}
}

void MessageSender::send(const Network::MessageHeader& header, const QString& payload) {
  send(createBuffer(totalPayloadSize(header), header, payload));
}

void MessageSender::send(const Network::MessageHeader& header) { send(createBuffer(totalPayloadSize(header), header)); }

void MessageSender::send(const QByteArray& data) { service<ISocketHolder>()->requestWrite(data); }
}
