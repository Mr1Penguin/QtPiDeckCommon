#pragma once

#include <QObject>
#include <QtQml>
#include <QQuickItem>

namespace QtPiDeck::Tests {
//TODO: rename to something like "QmlTreeExplorer"
class TestHelper : public QObject
{
    // NOLINTNEXTLINE
    Q_OBJECT

public:
    explicit TestHelper(QObject *parent = nullptr);

    Q_INVOKABLE QList<QObject*> findChildrenByTypeName(QQuickItem* parent, const QString & typeName); // NOLINT(modernize-use-trailing-return-type) trailing return type breaks moc
    Q_INVOKABLE QObject* findFirstChildByTypeName(QQuickItem* parent, const QString & typeName); // NOLINT(modernize-use-trailing-return-type)
};
}
