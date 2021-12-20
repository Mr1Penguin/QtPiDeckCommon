#pragma once

#include "ApplicationExt.inl"

#include "QtDefinitions.hpp"
#include "Services/DeckMessageToBusMessageMapper.hpp"
#include "Services/MessageBus.hpp"
#include "Services/MessageSender.hpp"
#include "Services/SocketHolder.hpp"

namespace QtPiDeck::detail {
template<class GuiApplication, class ApplicationEngine>
Application<GuiApplication, ApplicationEngine>::Application() {
  Application::s_current = this;
}

template<class GuiApplication, class ApplicationEngine>
auto Application<GuiApplication, ApplicationEngine>::start(int argc, char** argv) -> int {
  initialPreparationsBase();
  GuiApplication app{argc, argv};
  appCreatedBase();
  ApplicationEngine engine;
  engineCreatedBase(engine);
  auto pageUrl = mainPage();
  QObject::connect(
      &engine, &ApplicationEngine::objectCreated, // clazy:exclude=connect-non-signal
      &app,
      [&url = pageUrl, this](QObject* obj, const QUrl& objUrl) {
        if (obj == nullptr && url == objUrl) {
          GuiApplication::exit(-1);
        }

        m_qmlHelper.windowCreated();
      },
      Qt::QueuedConnection);
  engine.load(pageUrl);
  return GuiApplication::exec();
}

template<class GuiApplication, class ApplicationEngine>
auto Application<GuiApplication, ApplicationEngine>::ioc() -> Services::Ioc& {
  return m_ioc;
}

template<class GuiApplication, class ApplicationEngine>
[[nodiscard]] auto Application<GuiApplication, ApplicationEngine>::ioc() const -> const Services::Ioc& {
  return m_ioc;
}

template<class GuiApplication, class ApplicationEngine>
[[nodiscard]] auto Application<GuiApplication, ApplicationEngine>::current() -> Application* {
  return s_current;
}

template<class GuiApplication, class ApplicationEngine>
[[nodiscard]] auto Application<GuiApplication, ApplicationEngine>::ccurrent() -> const Application* {
  return s_current;
}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::initialPreparationsBase() {
  using namespace Services;
  m_ioc.registerSingleton<IMessageBus>(m_ioc.make<MessageBus, CreationType::SharedPointer>());
  m_ioc.registerSingleton<ISocketHolder>(m_ioc.make<SocketHolder, CreationType::SharedPointer>());
  m_ioc.registerService<IMessageSender, MessageSender>();
  m_ioc.registerService<IDeckMessageToBusMessageMapper, DeckMessageToBusMessageMapper>();

  initialPreparations();
}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::appCreatedBase() {
  appCreated();
}

template<class GuiApplication, class ApplicationEngine>
void Application<GuiApplication, ApplicationEngine>::engineCreatedBase(ApplicationEngine& engine) {
  engine.rootContext()->setContextProperty(CT_QStringLiteral("qh"), &m_qmlHelper);
  engineCreated(engine);
}
}