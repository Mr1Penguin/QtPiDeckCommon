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
    virtual ~Application();

    int start(int &argc, char **argv);

    QtPiDeck::Ioc& Ioc() { return m_ioc; }

    static Application* Current() { return current; }

protected:
    virtual QUrl mainPage() = 0;
    virtual void appStartupPreparations();
    virtual void setupEngine(QQmlApplicationEngine & engine);

    QtPiDeck::Ioc m_ioc;

private:
    inline static Application* current;
};
}
}
