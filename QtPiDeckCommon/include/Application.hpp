#pragma once

#include "QtPiDeckCommon_global.hpp"
#include <boost/di.hpp>

#include <QQmlApplicationEngine>
#include <QUrl>

namespace QtPiDeck {
template<class... TInjectors>
class Ioc {
public:
    Ioc(TInjectors&&... injectors) : injector(boost::di::make_injector(injectors()...)) {
    }

    template<class T>
    auto get() {
        return injector.template create<T>();
    }
private:
    decltype(boost::di::make_injector(std::declval<TInjectors&>()()...)) injector;
};

using EmptyIoc = Ioc<>;

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

template <class Ioc = EmptyIoc>
class Application : public detail::Application {
  public:
    static Ioc ioc;
};
}
