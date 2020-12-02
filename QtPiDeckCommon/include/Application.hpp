#pragma once

#include "QtPiDeckCommon_global.hpp"

#include <QQmlApplicationEngine>
#include <QUrl>

namespace QtPiDeck {
namespace detail {
class QTPIDECKCOMMON_EXPORT Application {
public:
    Application();
    virtual ~Application();

    int start(int &argc, char **argv);

protected:
    virtual QUrl mainPage() = 0;
    virtual void appStartupPreparations();
    virtual void setupEngine(QQmlApplicationEngine & engine);

private:
};
}
}
