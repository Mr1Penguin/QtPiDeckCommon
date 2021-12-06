#include "BoostUnitTest.hpp"

#include <QObject>

#include "Utilities/Connection.hpp"
#include "Utilities/Logging.hpp"

CT_BOOST_AUTO_TEST_SUITE(ConnectionTests)

class SignalEmitter : public QObject {
  Q_OBJECT // NOLINT
public:
  void emitSignal() { emit signal(); }
signals:
  void signal();
};

CT_BOOST_AUTO_TEST_CASE(SubscripionExpiresOnDestructor) {
  SignalEmitter emitter;
  int counter = 0;
  {
    QtPiDeck::Utilities::Connection connection{
        QObject::connect(&emitter, &SignalEmitter::signal, [&counter] { ++counter; })};
    emitter.emitSignal();
  }
  CT_BOOST_TEST(counter == 1);
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
}

CT_BOOST_AUTO_TEST_CASE(SubscripionExpiresOnReset) {
  SignalEmitter emitter;
  int counter = 0;
  QtPiDeck::Utilities::Connection connection{
      QObject::connect(&emitter, &SignalEmitter::signal, [&counter] { ++counter; })};
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
  connection.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
}

CT_BOOST_AUTO_TEST_CASE(SubscripionDoesNotAffectConnectionAfterMove) {
  SignalEmitter emitter;
  int counter     = 0;
  auto connection = std::make_unique<QtPiDeck::Utilities::Connection>(
      QObject::connect(&emitter, &SignalEmitter::signal, [&counter] { ++counter; }));
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
  auto connection2 = std::make_unique<QtPiDeck::Utilities::Connection>(std::move(*connection));
  connection.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 2);
  connection2.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 2);
}

CT_BOOST_AUTO_TEST_CASE(SubscripionIsEmptyAfterMove) {
  SignalEmitter emitter;
  int counter     = 0;
  auto connection = std::make_unique<QtPiDeck::Utilities::Connection>(
      QObject::connect(&emitter, &SignalEmitter::signal, [&counter] { ++counter; }));
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
  auto connection2 = std::make_unique<QtPiDeck::Utilities::Connection>(std::move(*connection));
  connection2.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
  connection.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
}

CT_BOOST_AUTO_TEST_CASE(SubscripionIsEmptyAfterMoveAssign) {
  SignalEmitter emitter;
  int counter     = 0;
  auto connection = std::make_unique<QtPiDeck::Utilities::Connection>(
      QObject::connect(&emitter, &SignalEmitter::signal, [&counter] { ++counter; }));
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
  auto connection2 = std::make_unique<QtPiDeck::Utilities::Connection>();
  *connection2     = std::move(*connection);
  connection2.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
  connection.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
}

CT_BOOST_AUTO_TEST_CASE(moveFromEmpty) {
  auto emptyConnection   = QtPiDeck::Utilities::Connection{};
  auto connection        = std::move(emptyConnection);
  auto anotherConnection = QtPiDeck::Utilities::Connection{std::move(connection)};
}

CT_BOOST_AUTO_TEST_SUITE_END()

#include "ConnectionTests.moc"