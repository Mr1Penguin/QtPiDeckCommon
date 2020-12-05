#pragma once

#include "QtPiDeckCommon_global.hpp"

#include <QQmlApplicationEngine>
#include <QUrl>

#include "Ioc.hpp"

namespace QtPiDeck {
namespace detail {
class QTPIDECKCOMMON_EXPORT Application {
public:
    Application();
    Application(const Application &) = delete;
    Application(Application &&) = delete;
    virtual ~Application() = default;

    auto operator=(const Application &) -> Application& = delete;
    auto operator=(Application &&) -> Application& = delete;

    auto start(int &argc, char **argv) -> int;

    auto Ioc() -> QtPiDeck::Ioc& { return m_ioc; }

    static auto Current() -> Application* { return current; }

protected:
    virtual auto mainPage() -> QUrl = 0;
    virtual void appStartupPreparations();
    virtual void setupEngine(QQmlApplicationEngine & engine);

private:
    QtPiDeck::Ioc m_ioc;

    inline static Application* current;
};
}
}
