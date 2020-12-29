#include "TestHelper.hpp"

#include <algorithm>
#include <array>
#include <optional>
#include <utility>

namespace QtPiDeck::Tests {
TestHelper::TestHelper(QObject *parent) : QObject(parent) {
}

namespace {
auto operator "" _q(const char* text, size_t /*unused*/) -> QString {
    return QString{text};
}

auto getQtQmlTypeName(const QString& name) noexcept -> QString {
    const std::array qQuickTypes{"Text"_q};
    const std::array qmlTypeTypes{"TextField"_q};
    auto getTypeName = [&name](
            auto && types,
            const std::optional<QString>& append = std::nullopt,
            const std::optional<QString>& prepend = std::nullopt) noexcept -> std::optional<QString> {
        if (const auto it = std::find(std::cbegin(types), std::cend(types), name);
                it != std::cend(types)) {
            QString typeName = *it;
            if (append) {
                typeName = *append + typeName;
            }

            if (prepend) {
                typeName += *prepend;
            }

            return typeName;
        }

        return std::nullopt;
    };

    if (const auto typeName = getTypeName(qQuickTypes, "QQuick"_q);
            typeName) {
        return *typeName;
    }

    if (const auto typeName = getTypeName(qmlTypeTypes, std::nullopt, "_QMLTYPE"_q);
            typeName) {
        return *typeName;
    }

    qWarning() << "Failed to find rule for type '" << name << "'. Returning as is.";

    return name;
}
}

QList<QObject*> TestHelper::findChildrenByTypeName(QQuickItem* parent, const QString & typeName) const noexcept { // NOLINT(readability-convert-member-functions-to-static, modernize-use-trailing-return-type)
    QList<QObject*> children;
    std::copy_if(std::begin(parent->childItems()), std::end(parent->childItems()), std::back_inserter(children), [&typeName](QQuickItem * child) {
        auto className = QString::fromLatin1(child->metaObject()->className());
        return className == typeName || [&className, typeName = getQtQmlTypeName(typeName)]{
            return className == typeName || className.indexOf(typeName) != -1;
        }();
    });
    return children;
}

QObject* TestHelper::findFirstChildByTypeName(QQuickItem* parent, const QString & typeName) const noexcept { // NOLINT(readability-convert-member-functions-to-static, modernize-use-trailing-return-type)
    auto list = findChildrenByTypeName(parent, typeName);
    return list.empty() ? nullptr : list.first();
}
}
