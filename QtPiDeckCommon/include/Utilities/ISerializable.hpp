#pragma once

#include <QDataStream>

namespace QtPiDeck::Utilities {
class ISerializable {
public:
  virtual ~ISerializable()                      = default;
  virtual void read(QDataStream& stream)        = 0;
  virtual void write(QDataStream& stream) const = 0;
};
}