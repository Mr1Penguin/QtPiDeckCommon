// NOLINTNEXTLINE
#define BOOST_TEST_MODULE ConnectionTests
#include "BoostUnitTest.hpp"

#include <QObject>
#include "Utilities/Connection.hpp"
#include "Utilities/Logging.hpp"

auto main(int argc, char* argv[]) -> int {
  QtPiDeck::Utilities::initLogging("ConnectionTests");
  return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

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
  int counter       = 0;
  auto Connection = std::make_unique<QtPiDeck::Utilities::Connection>(
      QObject::connect(&emitter, &SignalEmitter::signal, [&counter] { ++counter; }));
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
  auto Connection2 = std::make_unique<QtPiDeck::Utilities::Connection>(std::move(*Connection));
  Connection.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 2);
  Connection2.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 2);
}

CT_BOOST_AUTO_TEST_CASE(SubscripionIsEmptyAfterMove) {
  SignalEmitter emitter;
  int counter       = 0;
  auto Connection = std::make_unique<QtPiDeck::Utilities::Connection>(
      QObject::connect(&emitter, &SignalEmitter::signal, [&counter] { ++counter; }));
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
  auto Connection2 = std::make_unique<QtPiDeck::Utilities::Connection>(std::move(*Connection));
  Connection2.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
  Connection.reset();
  emitter.emitSignal();
  CT_BOOST_TEST(counter == 1);
}

CT_BOOST_AUTO_TEST_SUITE_END()

#include "ConnectionTests.moc"