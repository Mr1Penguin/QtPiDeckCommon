#pragma once

#include "QtPiDeckCommon_global.hpp"

#include <QQmlApplicationEngine>
#include <QUrl>

#include "Ioc.hpp"

namespace QtPiDeck {
class QTPIDECKCOMMON_EXPORT Application {
public:
    Application();
    Application(const Application &) = delete;
    Application(Application &&) = delete;
    virtual ~Application() = default;

    auto operator=(const Application &) -> Application& = delete;
    auto operator=(Application &&) -> Application& = delete;

    auto start(int argc, char **argv) -> int;

    auto ioc() -> QtPiDeck::Ioc& { return m_ioc; }

    static auto current() -> Application* { return s_current; }

protected:
    virtual auto mainPage() -> QUrl = 0;
    virtual void appStartupPreparations();
    virtual void setupEngine(QQmlApplicationEngine & engine);

private:
    QtPiDeck::Ioc m_ioc;

    inline static Application* s_current;
};
}
