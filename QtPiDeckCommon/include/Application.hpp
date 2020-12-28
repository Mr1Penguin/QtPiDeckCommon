#pragma once

#include "QtPiDeckCommon_global.hpp"

#include <QQmlApplicationEngine>
#include <QUrl>

#include "Services/Ioc.hpp"

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

    auto ioc() -> Services::Ioc& { return m_ioc; }
    auto ioc() const -> const Services::Ioc& { return m_ioc; }

    static auto current() -> Application* { return s_current; }

protected:
    virtual auto mainPage() -> QUrl = 0;
    virtual void initialPreparations();
    virtual void appCreated();
    virtual void engineCreated(QQmlApplicationEngine & engine);

private:
    Services::Ioc m_ioc;

    inline static Application* s_current; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
}
