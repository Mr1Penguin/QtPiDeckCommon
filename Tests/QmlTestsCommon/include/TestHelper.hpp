#pragma once

#include <QObject>
#include <QtQml>
#include <QQuickItem>

namespace QtPiDeck::Tests {
class TestHelper : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit TestHelper(QObject *parent = nullptr);

    Q_INVOKABLE QList<QObject*> findChildrenByTypeName(QQuickItem* parent, QString typeName);
    Q_INVOKABLE QObject* findFirstChildByTypeName(QQuickItem* parent, QString typeName);
};
}
