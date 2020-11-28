#include "TestHelper.hpp"

#include <algorithm>

namespace QtPiDeck::Tests {
TestHelper::TestHelper(QObject *parent) : QObject(parent) {
}

QList<QObject*> TestHelper::findChildrenByTypeName(QQuickItem* parent, QString typeName) {
    QList<QObject*> children;
    std::copy_if(std::begin(parent->childItems()), std::end(parent->childItems()), std::back_inserter(children), [typeName](QQuickItem * child) {
        auto className = QString::fromLatin1(child->metaObject()->className());
        return className == typeName || className == "QQuick" + typeName;
    });
    return children;
}

QObject* TestHelper::findFirstChildByTypeName(QQuickItem* parent, QString typeName) {
    auto list = findChildrenByTypeName(parent, typeName);
    return list.empty() ? nullptr : list.first();
}
}
