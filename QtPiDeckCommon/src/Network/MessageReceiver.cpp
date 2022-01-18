#include "Network/MessageReceiver.hpp"

#include <cassert>
#include <unordered_map>

#include "Network/DeckDataStream.hpp"
#include "Network/Messages.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/OnExit.hpp"
#include "Utilities/Traits.hpp"

namespace QtPiDeck::Network {
MessageReceiver::MessageReceiver(std::shared_ptr<Services::ISocketHolder> holder,
                                 std::shared_ptr<Services::IMessageBus> bus,
                                 std::shared_ptr<Services::IDeckMessageToBusMessageMapper> messageTypeMapper) {
  Utilities::initLogger(m_slg, "MessageReceiver");
  connect(holder->socket(), &QTcpSocket::readyRead, this, &MessageReceiver::readData);
  setService(std::move(holder));
  setService(std::move(bus));
  setService(std::move(messageTypeMapper));
}

namespace {
auto readHeader(QIODevice& socket) -> std::optional<Network::MessageHeader> {
  auto inStream = Network::DeckDataStream{&socket};
  auto object   = Network::MessageHeader{};

  inStream.startTransaction();
  object.read(inStream);
  if (inStream.commitTransaction()) {
    return object;
  }

  return std::nullopt;
}

#if __cpp_concepts >= 201907L
template<std::derived_from<Utilities::ISerializable> T>
#else
template<class T, std::enable_if_t<Utilities::derived_from<T, Utilities::ISerializable>, bool> = true>
#endif
auto readObject(QIODevice& socket) -> T {
  auto inStream = Network::DeckDataStream{&socket};
  auto object   = T{};

  static_cast<Utilities::ISerializable&>(object).read(inStream);
  return object;
}

#if __cpp_concepts >= 201907L
template<std::derived_from<Utilities::ISerializable> TMessage>
#else
template<class TMessage, std::enable_if_t<Utilities::derived_from<TMessage, Utilities::ISerializable>, bool> = true>
#endif
auto repack(QIODevice& socket, std::size_t dataSize) -> QByteArray {
  auto payload = QByteArray{};
  if (dataSize) {
#if QT_VERSION_MAJOR == 5
    using argType = int;
#else
    using argType = qsizetype;
#endif
    assert(dataSize < static_cast<std::size_t>(std::numeric_limits<argType>::max())); // NOLINT
    payload.reserve(static_cast<argType>(dataSize));
  }

  const auto message = readObject<TMessage>(socket);
  auto stream        = QDataStream{&payload, QIODevice::WriteOnly};
  message.write(stream);
  return payload;
}

using repackSignature = std::function<QByteArray(QIODevice& socket, std::size_t dataSize)>;

const std::unordered_map<Network::MessageType, repackSignature> repackMap = {
    {Network::MessageType::Hello, repack<Network::Messages::Hello>}};
}

void MessageReceiver::readData() {
  auto* socket       = service<Services::ISocketHolder>()->socket();
  const auto& header = m_savedHeader.has_value() ? m_savedHeader : readHeader(*socket);
  if (!header.has_value()) {
    return;
  }

  auto payload          = QByteArray{};
  const auto hasPayload = header->dataSize > 0;

  if (hasPayload) {
    if (socket->bytesAvailable() < qint64{0} || static_cast<quint64>(socket->bytesAvailable()) < header->dataSize) {
      if (!m_savedHeader.has_value()) {
        m_savedHeader = header;
      }

      return;
    }

    auto guard = Utilities::OnExit([this] { m_savedHeader.reset(); });

    try {
      payload = repackMap.at(header->messageType)(*socket, header->dataSize);
    } catch (const std::out_of_range& /*e*/) {
      BOOST_LOG_SEV(m_slg, Utilities::severity::error)
          << "Unable to process " << getMessageTypeNane(header->messageType) << " message. Skipping message.";

      // send ERROR as response?
      return;
    }
  }

  const auto& messageTypeMapper = service<Services::IDeckMessageToBusMessageMapper>();
  const auto busMessageType     = messageTypeMapper->getBusMessageType(header->messageType);
  if (!busMessageType.has_value()) {
    return;
  }

  const auto busMessage = hasPayload ? Bus::createMessage(*busMessageType, header->messageType, payload)
                                     : Bus::createMessage(*busMessageType, header->messageType);

  service<Services::IMessageBus>()->sendMessage(busMessage);
}
}