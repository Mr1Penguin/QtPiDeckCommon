#pragma once

#include <QObject>
#include <QtQml>

#include "Application.hpp"

namespace QtPiDeck::Services {
template <class TService>
class QmlService final : public QObject {
    Q_OBJECT // NOLINT
    QML_ELEMENT // NOLINT

    Q_PROPERTY(TService* service READ service CONSTANT FINAL)

public:
    explicit QmlService(QObject* parent = 0) : QObject(parent) {
        m_service = Application::Current()->Ioc().ResolveService<TService>();
    }

    ~QmlService() final = default;

    auto service() -> TService* { return m_service.get(); }

    static void RegisterType(const char* moduleName, int majorVersion, int minorVersion, const char* typeName) {
        qmlRegisterType<QmlService<TService>>(moduleName, majorVersion, minorVersion, typeName);
        qmlRegisterAnonymousType<TService>(moduleName, majorVersion);
    }

private:
    std::shared_ptr<TService> m_service;
};
}
