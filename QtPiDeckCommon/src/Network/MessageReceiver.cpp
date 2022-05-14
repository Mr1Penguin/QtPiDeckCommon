#include "Network/MessageReceiver.hpp"

#include <QJsonDocument>
#include <cassert>
#include <unordered_map>

#include "Network/DeckDataStream.hpp"
#include "Network/Messages.hpp"
#include "Services/ISocketHolder.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/OnExit.hpp"
#include "Utilities/Traits.hpp"

namespace QtPiDeck::Network {
MessageReceiver::MessageReceiver(dependency_list dependencies) {
  Utilities::initLogger(m_slg, "MessageReceiver");
  setServices(std::move(dependencies));
  connect(service<Services::ISocketHolder>()->socket(), &QTcpSocket::readyRead, this, &MessageReceiver::readData);
}

namespace {
#if 0
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
#endif

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
}

void MessageReceiver::readData() {
#if 0
  auto* socket       = service<Services::ISocketHolder>()->socket();
  const auto& header = m_savedHeader.has_value() ? m_savedHeader : readHeader(*socket);
  if (!header.has_value()) {
    return;
  }

  auto payload          = QJsonDocument{};
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
          << "Unable to process " << getMessageTypeName(header->messageType) << " message. Skipping message.";

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
#endif
}
}
