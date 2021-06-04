#pragma once

#include "Bus/Message.hpp"

#include "IMessageBus.hpp"
#include "QtPiDeckCommon_global.hpp"

namespace QtPiDeck::Services {
class QTPIDECKCOMMON_EXPORT MessageBus final : public QObject, public IMessageBus {
  Q_OBJECT // NOLINT
public:
  explicit MessageBus(QObject* parent) : QObject(parent) {}

  [[nodiscard]] auto subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method) noexcept
      -> Subscription final;
  [[nodiscard]] auto subscribe(QObject* context, const std::function<void(const Bus::Message&)>& method,
                               uint64_t messageType) noexcept -> Subscription final;
  void unsubscribe(Subscription& connection) noexcept final;
  void sendMessage(Bus::Message message) noexcept final;

signals:
  void newMessage(Bus::Message message);
};
}
