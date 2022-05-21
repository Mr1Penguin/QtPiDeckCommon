#include "Network/Messages/HelloMessageSerialization.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <qjsondocument.h>

namespace QtPiDeck::Network {
inline namespace Messages {
namespace {
const auto interfaceVersionKey = QLatin1String{"interfaceVersion"};
const auto qcharSizeKey        = QLatin1String{"qcharSize"};
}

auto QTPIDECKCOMMON_EXPORT prepareJsonString(const Hello& message) -> QString {
  auto document = QJsonDocument{};
  auto object   = document.object();
  object.insert(interfaceVersionKey, static_cast<qint64>(message.interfaceVersion));
  object.insert(qcharSizeKey, static_cast<qint64>(message.qcharSize));
  return document.toJson();
}

auto QTPIDECKCOMMON_EXPORT prepareMessage(const QString& json) -> Hello {
  auto message             = Hello{};
  const auto document      = QJsonDocument::fromJson(json.toUtf8());
  const auto object        = document.object();
  message.interfaceVersion = object.value(interfaceVersionKey).toInt();
  message.qcharSize        = object.value(qcharSizeKey).toInt();
  return message;
}
}
}
