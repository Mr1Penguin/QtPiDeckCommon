#pragma once

#include <QObject>

#include <optional>

#include "Network/DeckDataStream.hpp"
#include "Network/MessageHeader.hpp"
#include "Services/IDeckMessageToBusMessageMapper.hpp"
#include "Services/IMessageBus.hpp"
#include "Services/ISocketHolder.hpp"
#include "Services/UseServices.hpp"
#include "Utilities/Connection.hpp"
#include "Utilities/Logging.hpp"

#include "QtPiDeckCommon_global.hpp"

namespace QtPiDeck::Network {
class QTPIDECKCOMMON_EXPORT MessageReceiver
    : public QObject,
      public Services::UseServices<Services::ISocketHolder, Services::IMessageBus,
                                   Services::IDeckMessageToBusMessageMapper> {
  Q_OBJECT // NOLINT
public:
  MessageReceiver(dependency_list dependencies);

private:
  void readData();
  void updateSocketConnection();

  std::optional<Network::MessageHeader> m_savedHeader;
  Utilities::Connection m_socketUpdateConnection;
  mutable boost::log::sources::severity_logger<boost::log::trivial::severity_level> m_slg;
};
}
