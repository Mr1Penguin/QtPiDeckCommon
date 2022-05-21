#pragma once

#include "QtPiDeckCommon_global.hpp"
#include "Services/IMessageSender.hpp"
#include "Services/ISocketHolder.hpp"
#include "Services/UseServices.hpp"

namespace QtPiDeck::Services {
class QTPIDECKCOMMON_EXPORT MessageSender final : public IMessageSender, public UseServices<ISocketHolder> {
public:
  void send(const Network::MessageHeader& header, const QString& payload) final;
  void send(const Network::MessageHeader& header) final;

private:
  void send(const QByteArray& data);
};
}
