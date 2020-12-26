#pragma once

#include <QString>
#include <QVariant>

#include "ServiceInterface.hpp"

namespace QtPiDeck::Services {
class ISettingsStorage : public ServiceInterface {
public:
    virtual auto read(const QString & key) const noexcept -> QVariant = 0;
    virtual void store(const QString & key, const QVariant & value) noexcept = 0;
};
}
