#include "TestHelper.hpp"

#include <algorithm>

namespace QtPiDeck::Tests {
TestHelper::TestHelper(QObject *parent) : QObject(parent) {
}

QList<QObject*> TestHelper::findChildrenByTypeName(QQuickItem* parent, const QString & typeName) { // NOLINT(readability-convert-member-functions-to-static, modernize-use-trailing-return-type)
    QList<QObject*> children;
    std::copy_if(std::begin(parent->childItems()), std::end(parent->childItems()), std::back_inserter(children), [typeName](QQuickItem * child) {
        auto className = QString::fromLatin1(child->metaObject()->className());
        return className == typeName || className == "QQuick" + typeName;
    });
    return children;
}

QObject* TestHelper::findFirstChildByTypeName(QQuickItem* parent, const QString & typeName) { // NOLINT(readability-convert-member-functions-to-static, modernize-use-trailing-return-type)
    auto list = findChildrenByTypeName(parent, typeName);
    return list.empty() ? nullptr : list.first();
}
}
