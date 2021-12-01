#include "BoostUnitTest.hpp"

#include <array>

#include "Application.inl"
#include "SignalCatcher.hpp"
#include "Utilities/Logging.hpp"

namespace {
class TestApplication final : public QtPiDeck::Application {
public:
protected:
  auto mainPage() const -> QUrl final { return QUrl(); }
};
}

CT_BOOST_AUTO_TEST_SUITE(ApplicationTests)
using namespace QtPiDeck::Utilities;
using namespace QtPiDeck::Tests;
using namespace std::string_literals;

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
  void setContextProperty(const QString& name, QObject* object) {
    m_contextProperties.emplace(name.toStdString(), object);
  }
  auto contextProperties() const -> std::unordered_map<std::string, QObject*> { return m_contextProperties; }

private:
  std::unordered_map<std::string, QObject*> m_contextProperties;
};

class TestApplicationEngine;

void setLastEngine(const TestApplicationEngine& /*eng*/);

class TestApplicationEngine : public QObject {
  Q_OBJECT // NOLINT
public:
  TestApplicationEngine() : QObject(nullptr) {}
  TestApplicationEngine(const TestApplicationEngine& other)
      : QObject(nullptr), m_testContext(other.m_testContext), m_loadCalled(other.m_loadCalled) {}
  TestApplicationEngine(TestApplicationEngine&& other) noexcept
      : QObject(nullptr), m_testContext(std::move(other.m_testContext)), m_loadCalled(other.m_loadCalled) {}
  ~TestApplicationEngine() override { setLastEngine(*this); }
  auto operator=(const TestApplicationEngine& other) -> TestApplicationEngine& {
    m_testContext = other.m_testContext;
    m_loadCalled  = other.m_loadCalled;
    return *this;
  }

  auto operator=(TestApplicationEngine&& other) noexcept -> TestApplicationEngine& {
    m_testContext = std::move(other.m_testContext);
    m_loadCalled  = other.m_loadCalled;
    return *this;
  }
  void load(const QUrl& /*url*/) { m_loadCalled = true; }

  auto rootContext() -> TestContext* { return &m_testContext; }
  [[nodiscard]] auto loadCalled() const -> bool { return m_loadCalled; }

signals:
  void objectCreated(QObject* /*obj*/, const QUrl& /*url*/);

private:
  TestContext m_testContext;
  bool m_loadCalled{false};
};

TestApplicationEngine g_last{}; // NOLINT

void setLastEngine(const TestApplicationEngine& eng) { g_last = eng; }

class TestGuiApplication : public QObject {
  Q_OBJECT // NOLINT
public:
  TestGuiApplication(int /*argc*/, char** /*argv*/) { s_exitCalled = false; }
  static void exit(int /*code*/) { s_exitCalled = true; }
  static auto exec() -> int {
    s_execCalled = true;
    return 0;
  }

  static auto exitCalled() -> bool { return s_exitCalled; }
  static auto execCalled() -> bool { return s_execCalled; }

private:
  static inline bool s_exitCalled{false}; // NOLINT
  static inline bool s_execCalled{false}; // NOLINT
};

class CustomTestApplication final : public QtPiDeck::detail::Application<TestGuiApplication, TestApplicationEngine> {
public:
  auto initialPreparationsCalls() const -> std::size_t { return m_initialPreparationsCalls; }
  auto appCreatedCalls() const -> std::size_t { return m_appCreatedCalls; }
  auto engineCreatedCalls() const -> std::size_t { return m_engineCreatedCalls; }

  auto engine() -> TestApplicationEngine& { return m_engine; }

protected:
  auto mainPage() const -> QUrl final { return "someUrl"_qurl; }
  void initialPreparations() final { ++m_initialPreparationsCalls; }
  void appCreated() final { ++m_appCreatedCalls; }
  void engineCreated(TestApplicationEngine& engine) final {
    ++m_engineCreatedCalls;
    m_engine = engine;
  }

private:
  std::size_t m_initialPreparationsCalls{0};
  std::size_t m_appCreatedCalls{0};
  std::size_t m_engineCreatedCalls{0};

  TestApplicationEngine m_engine{};
};

CT_BOOST_AUTO_TEST_CASE(startShouldCallDerivedAppCreated) {
  char arg0[]     = "TEST"; // NOLINT
  std::array argv = {arg0}; // NOLINT
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  CT_BOOST_TEST(app.initialPreparationsCalls() == 1);
}

CT_BOOST_AUTO_TEST_CASE(startShouldCallDerivedInitialPreparations) {
  char arg0[]     = "TEST"; // NOLINT
  std::array argv = {arg0}; // NOLINT
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  CT_BOOST_TEST(app.appCreatedCalls() == 1);
}

CT_BOOST_AUTO_TEST_CASE(startShouldCallDerivedEngineCreated) {
  char arg0[]     = "TEST"; // NOLINT
  std::array argv = {arg0}; // NOLINT
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  CT_BOOST_TEST(app.engineCreatedCalls() == 1);
}

CT_BOOST_AUTO_TEST_CASE(startShouldCallGuiApplicationExec) {
  char arg0[]     = "TEST"; // NOLINT
  std::array argv = {arg0}; // NOLINT
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  CT_BOOST_TEST(TestGuiApplication::execCalled());
}

CT_BOOST_AUTO_TEST_CASE(startShouldCallApplicationEngineLoad) {
  char arg0[]     = "TEST"; // NOLINT
  std::array argv = {arg0}; // NOLINT
  CustomTestApplication app;
  app.start(argv.size(), argv.data());
  const auto res = g_last.loadCalled();
  CT_BOOST_TEST(res == true);
}

CT_BOOST_AUTO_TEST_CASE(startShouldAddQmlHelper) {
  char arg0[]     = "TEST"; // NOLINT
  std::array argv = {arg0}; // NOLINT
  [[maybe_unused]] CustomTestApplication app;
  app.start(argv.size(), argv.data());
  const auto props    = g_last.rootContext()->contextProperties();
  const auto key      = "qh"s;
  const auto it       = props.find(key);
  const auto contains = it != props.end();
  CT_BOOST_TEST(contains == true);
  const auto* val = it->second;
  CT_BOOST_TEST(dynamic_cast<const QmlHelper*>(val) != nullptr);
}

CT_BOOST_AUTO_TEST_SUITE_END()

#include "ApplicationTests.moc"
