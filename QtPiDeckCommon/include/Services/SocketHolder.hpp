#pragma once

#include <QObject>

#include "Bus/CommonMessageIds.hpp"
#include "ISocketHolder.hpp"
#include "QtPiDeckCommon_global.hpp"
#include "Services/IMessageBus.hpp"
#include "Services/UseServices.hpp"

namespace QtPiDeck::Services {
class QTPIDECKCOMMON_EXPORT SocketHolder final : public QObject, public ISocketHolder, public UseServices<IMessageBus> {
  Q_OBJECT // NOLINT
public:
  SocketHolder(QObject* parent, std::shared_ptr<IMessageBus> bus) : QObject(parent) {
    connect(this, &SocketHolder::writeRequested, this, &SocketHolder::write, Qt::ConnectionType::QueuedConnection);
    setService(std::move(bus));
  }

  auto socket() -> QTcpSocket* final { return m_socket; }
  void requestWrite(const QByteArray& data) final { emit writeRequested(data); }
  void setSocket(QTcpSocket* socket) final {
    if (m_socket->isOpen()) {
      m_socket->close();
    }

    m_socket->deleteLater();
    m_socket = socket;
    service<IMessageBus>()->sendMessage(Bus::createMessage(Bus::SocketChanged));
  }

signals:
  void writeRequested(QByteArray data);

private:
  QTcpSocket* m_socket;

  void write(QByteArray data) { m_socket->write(data); }
};
}