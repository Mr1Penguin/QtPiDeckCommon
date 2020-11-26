#pragma once

#include <QQmlEngine>

namespace QtPiDeck {
template<class T>
T* qmlSingletonProvider(QQmlEngine* engine, QJSEngine*) {
    struct Enablr : public T { explicit Enablr(QObject * parent = nullptr) : T(parent) {}};
    return new Enablr(engine);
}
}
