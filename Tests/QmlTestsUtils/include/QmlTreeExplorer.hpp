#pragma once

#include <QObject>
#include <QtQml>
#include <QQuickItem>

namespace QtPiDeck::Tests {
//TODO: rename to something like "QmlTreeExplorer"
class QmlTreeExplorer : public QObject
{
    Q_OBJECT // NOLINT

public:
    explicit QmlTreeExplorer(QObject *parent = nullptr);

    Q_INVOKABLE QList<QObject*> findChildrenByTypeName(QQuickItem* parent, const QString & typeName) const noexcept; // NOLINT(modernize-use-trailing-return-type) trailing return type breaks moc
    Q_INVOKABLE QObject* findFirstChildByTypeName(QQuickItem* parent, const QString & typeName) const noexcept; // NOLINT(modernize-use-trailing-return-type)
};
}
