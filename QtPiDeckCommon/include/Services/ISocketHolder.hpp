#pragma once

#include <QTcpSocket>

#include "ServiceInterface.hpp"

namespace QtPiDeck::Services {
class ISocketHolder : public ServiceInterface {
public:
  virtual void requestWrite(const QByteArray& data) = 0;
  virtual void setSocket(QIODevice* socket)         = 0;
  [[nodiscard]] virtual auto socket() -> QIODevice* = 0;
};
}
