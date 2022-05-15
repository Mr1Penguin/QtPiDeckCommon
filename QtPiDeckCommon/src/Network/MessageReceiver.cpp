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

void MessageReceiver::readData() {}
}
