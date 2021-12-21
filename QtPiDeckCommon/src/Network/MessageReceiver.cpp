#include "Network/MessageReceiver.hpp"

#include <cassert>
#include <unordered_map>

#include "Network/DeckDataStream.hpp"
#include "Network/Messages.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/OnExit.hpp"

namespace QtPiDeck::Utilities {
MessageReceiver::MessageReceiver(std::shared_ptr<Services::ISocketHolder> holder,
                                 std::shared_ptr<Services::IMessageBus> bus,
                                 std::shared_ptr<Services::IDeckMessageToBusMessageMapper> messageTypeMapper) {
  initLogger(m_slg, "MessageReceiver");
  connect(holder->socket(), &QTcpSocket::readyRead, this, &MessageReceiver::readData);
  setService(std::move(holder));
  setService(std::move(bus));
  setService(std::move(messageTypeMapper));
}

namespace {
template<class T>
auto readObject(QTcpSocket& socket) -> std::optional<T> {
  auto inStream = Network::DeckDataStream{&socket};
  static_assert(std::is_base_of_v<ISerializable, T>);
  T object{};
  auto& asSerializable = static_cast<ISerializable&>(object);

  inStream.startTransaction();
  asSerializable.read(inStream);
  if (inStream.commitTransaction()) {
    return object;
  }

  return std::nullopt;
}

template<class TMessage>
auto repack(QTcpSocket& socket, std::size_t dataSize) -> QByteArray {
  auto payload = QByteArray{};
  if (dataSize) {
#if QT_VERSION_MAJOR == 5
    assert(dataSize < std::numeric_limits<int>::max()); // NOLINT
    payload.reserve(static_cast<int>(dataSize));
#else
    payload.reserve(dataSize);
#endif
  }

  const auto message = readObject<TMessage>(socket);
  auto stream        = QDataStream{&payload, QIODevice::WriteOnly};
  message->write(stream);
  return payload;
}

using repackSignature = std::function<QByteArray(QTcpSocket& socket, std::size_t dataSize)>;

const std::unordered_map<Network::MessageType, repackSignature> repackMap = {
    {Network::MessageType::Hello, repack<Network::Messages::Hello>}};
}

void MessageReceiver::readData() {
  auto* socket       = service<Services::ISocketHolder>()->socket();
  const auto& header = m_savedHeader.has_value() ? m_savedHeader : readObject<Network::MessageHeader>(*socket);
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

    auto guard = OnExit([this] { m_savedHeader.reset(); });

    try {
      payload = repackMap.at(header->messageType)(*socket, header->dataSize);
    } catch (const std::out_of_range& /*e*/) {
      BOOST_LOG_SEV(m_slg, Utilities::severity::error)
          << "Unable to process " << Network::messageTypeNames.at(static_cast<std::size_t>(header->messageType))
          << " message. Skipping message.";

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