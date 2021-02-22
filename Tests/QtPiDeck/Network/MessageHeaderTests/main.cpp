#include <QDataStream>
#include <QtTest>

#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Network::Tests {
class MessageHeaderTests : public QObject {
  Q_OBJECT // NOLINT

private slots:
  void serialize_and_deserialize();
};

void MessageHeaderTests::serialize_and_deserialize() { // NOLINT(readability-convert-member-functions-to-static)
  const MessageHeader messageHeader{0, MessageType::Pong, QStringLiteral("a-random-id")};
  QByteArray qba;
  QDataStream in{&qba, QIODevice::WriteOnly};
  in << messageHeader;
  QDataStream out{&qba, QIODevice::ReadOnly};
  MessageHeader outMessageHeader{};
  out >> outMessageHeader;
  QCOMPARE(outMessageHeader.dataSize, messageHeader.dataSize);
  QCOMPARE(outMessageHeader.messageType, messageHeader.messageType);
  QCOMPARE(outMessageHeader.requestId, messageHeader.requestId);
}
}

QTEST_APPLESS_MAIN(QtPiDeck::Network::Tests::MessageHeaderTests) // NOLINT

#include "main.moc"
