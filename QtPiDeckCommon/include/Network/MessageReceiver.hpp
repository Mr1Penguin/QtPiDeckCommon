#pragma once

#include <QObject>

#include <optional>

#include "Network/MessageHeader.hpp"
#include "Services/IDeckMessageToBusMessageMapper.hpp"
#include "Services/IMessageBus.hpp"
#include "Services/ISocketHolder.hpp"
#include "Services/UseServices.hpp"
#include "Utilities/Logging.hpp"

namespace QtPiDeck::Utilities {
class MessageReceiver : public QObject,
                        public Services::UseServices<Services::ISocketHolder, Services::IMessageBus,
                                                     Services::IDeckMessageToBusMessageMapper> {
  Q_OBJECT // NOLINT
public:
  MessageReceiver(std::shared_ptr<Services::ISocketHolder> holder, std::shared_ptr<Services::IMessageBus> bus,
                  std::shared_ptr<Services::IDeckMessageToBusMessageMapper> messageTypeMapper);

private:
  void readData();

  std::optional<Network::MessageHeader> m_savedHeader;
  mutable boost::log::sources::severity_logger<boost::log::trivial::severity_level> m_slg;
};
}