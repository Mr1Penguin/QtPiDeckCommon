#define BOOST_TEST_MODULE ApplicationTests // NOLINT
#include "BoostUnitTest.hpp"

#include <array>
#include <chrono>
#include <thread>

#include <QAbstractEventDispatcher>
#include <QEventLoop>

#include "Application.inl"
#include "SignalCatcher.hpp"
#include "Utilities/Logging.hpp"

auto main(int argc, char* argv[]) -> int {
  QtPiDeck::Utilities::initLogging("ApplicationTests");
  return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

namespace {
class TestApplication final : public QtPiDeck::Application {
public:
protected:
  auto mainPage() const -> QUrl final { return QUrl(); }
};
}

CT_BOOST_AUTO_TEST_SUITE(ApplicationTests)
using namespace QtPiDeck::Utilities::literals;
using namespace QtPiDeck::Tests;

CT_BOOST_AUTO_TEST_CASE(CurrentFieldShouldPointToActiveApplicationAfterCreation) {
  { [[maybe_unused]] const auto _ = std::make_unique<TestApplication>(); } // reset s_current;
  const auto* current = TestApplication::current();
  const TestApplication app;
  CT_BOOST_TEST(TestApplication::current() != current);
  CT_BOOST_TEST(TestApplication::current() == &app);
}

CT_BOOST_AUTO_TEST_CASE(CCurrentFieldShouldPointToActiveApplicationAfterCreation) {
  { [[maybe_unused]] const auto _ = std::make_unique<TestApplication>(); } // reset s_current;
  const auto* current = TestApplication::ccurrent();
  const TestApplication app;
  CT_BOOST_TEST(TestApplication::ccurrent() != current);
  CT_BOOST_TEST(TestApplication::ccurrent() == &app);
}

CT_BOOST_AUTO_TEST_CASE(iocReturnsSameObjectEachTime) {
  TestApplication app;
  const auto& ioc      = app.ioc();
  const auto& constIoc = std::as_const(app).ioc();

  CT_BOOST_TEST(&ioc == &app.ioc());
  CT_BOOST_TEST(&constIoc == &std::as_const(app).ioc());
  CT_BOOST_TEST(&ioc == &constIoc);
}

class TestContext {
public:
  void setContextProperty(const QString& /*name*/, QObject* /*object*/) {}
};

enum class ObjectCreatedResult { None, Null, WrongObject, Both, Ok };

class TestApplicationEngine : public QObject {
  Q_OBJECT
public:
  TestApplicationEngine() : QObject(nullptr) { s_current = this; }
  void load(const QUrl& /*url*/) {
    m_loadCalled = true;
  }

  auto rootContext() -> TestContext* { return &m_testContext; }
  auto loadCalled() const -> bool { return m_loadCalled; }

  static auto current() -> TestApplicationEngine* { return s_current; }

signals:
  void objectCreated(QObject*, const QUrl&);

private:
  TestContext m_testContext;

  bool m_loadCalled{false};
  bool m_emitObjectCreated{false};

  static inline TestApplicationEngine* s_current{nullptr};
};

class TestGuiApplication : public QObject {
  Q_OBJECT
public:
  TestGuiApplication(int /*argc*/, char** /*argv*/) { s_exitCalled = false; }
  static void exit(int /*code*/) { s_exitCalled = true; }
  static auto exec() -> int {
    s_execCalled = true;
    // TestApplicationEngine::current()->emitObjectCreated();
    return 0;
  }

  static auto exitCalled() -> bool { return s_exitCalled; }
  static auto execCalled() -> bool { return s_execCalled; }

private:
  static inline bool s_exitCalled{false};
  static inline bool s_execCalled{false};
};

class CustomTestApplication final
    : public QtPiDeck::detail::Application<TestGuiApplication, TestApplicationEngine> {
public:
  CustomTestApplication(QCoreApplication* coreApp = nullptr) : m_coreApp(coreApp) {}
  auto initialPreparationsCalls() const -> std::size_t { return m_initialPreparationsCalls; }
  auto appCreatedCalls() const -> std::size_t { return m_appCreatedCalls; }
  auto engineCreatedCalls() const -> std::size_t { return m_engineCreatedCalls; }

protected:
  auto mainPage() const -> QUrl final { return "someUrl"_qurl; }
  void initialPreparations() final { ++m_initialPreparationsCalls; }
  void appCreated() final { ++m_appCreatedCalls; }
  void engineCreated(TestApplicationEngine& /*engine*/) final { ++m_engineCreatedCalls; }

private:
  std::size_t m_initialPreparationsCalls{0};
  std::size_t m_appCreatedCalls{0};
  std::size_t m_engineCreatedCalls{0};
  QCoreApplication* m_coreApp;
};

CT_BOOST_AUTO_TEST_CASE(startShouldCallDerivedAppCreated) {
  char arg0[]     = "TEST";
  std::array argv = {arg0};
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  CT_BOOST_TEST(app.initialPreparationsCalls() == 1);
}

CT_BOOST_AUTO_TEST_CASE(startShouldCallDerivedInitialPreparations) {
  char arg0[]     = "TEST";
  std::array argv = {arg0};
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  CT_BOOST_TEST(app.appCreatedCalls() == 1);
}

CT_BOOST_AUTO_TEST_CASE(startShouldCallDerivedEngineCreated) {
  char arg0[]     = "TEST";
  std::array argv = {arg0};
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  CT_BOOST_TEST(app.engineCreatedCalls() == 1);
}

CT_BOOST_AUTO_TEST_CASE(startShouldCallGuiApplicationExec) {
  char arg0[]     = "TEST";
  std::array argv = {arg0};
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  CT_BOOST_TEST(TestGuiApplication::execCalled());
}

CT_BOOST_AUTO_TEST_CASE(startShouldCallApplicationEngineLoad) {
  char arg0[]     = "TEST";
  std::array argv = {arg0};
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  CT_BOOST_TEST(TestApplicationEngine::current()->loadCalled());
}

CT_BOOST_AUTO_TEST_SUITE_END()

#include "ApplicationTests.moc"
