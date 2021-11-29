#pragma once

#include "QtPiDeckCommon_global.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>

#include "Services/Ioc.hpp"
#include "Utilities/QmlHelper.hpp"

namespace QtPiDeck {
namespace detail {
template<class GuiApplication = QGuiApplication, class ApplicationEngine = QQmlApplicationEngine>
class Application {
public:
  Application();
  Application(const Application&) = delete;
  Application(Application&&)      = delete;
  virtual ~Application()          = default;

  auto operator=(const Application&) -> Application& = delete;
  auto operator=(Application&&) -> Application& = delete;

  auto start(int argc, char** argv) -> int;

  auto ioc() -> Services::Ioc&;
  [[nodiscard]] auto ioc() const -> const Services::Ioc&;

  [[nodiscard]] static auto current() -> Application*;
  [[nodiscard]] static auto ccurrent() -> const Application*;

protected:
  virtual auto mainPage() const -> QUrl = 0;
  virtual void initialPreparations();
  virtual void appCreated();
  virtual void engineCreated(ApplicationEngine& engine);

private:
  void initialPreparationsBase();
  void appCreatedBase();
  void engineCreatedBase(ApplicationEngine& engine);

  Services::Ioc m_ioc;
  Utilities::QmlHelper m_qmlHelper;

  inline static Application* s_current; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};

extern template class Application<>;
}

using Application = detail::Application<>;
}
