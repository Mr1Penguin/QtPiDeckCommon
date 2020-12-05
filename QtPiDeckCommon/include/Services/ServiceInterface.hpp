#pragma once

#include "QtPiDeckCommon_global.hpp"

namespace QtPiDeck::Services {
class QTPIDECKCOMMON_EXPORT ServiceInterface {
public:
    virtual ~ServiceInterface() = 0;
protected:
    ServiceInterface() = default;
    ServiceInterface(const ServiceInterface &) = default;
    ServiceInterface(ServiceInterface &&) = default;

    auto operator=(const ServiceInterface &) -> ServiceInterface& = default;
    auto operator=(ServiceInterface &&) -> ServiceInterface& = default;
};
}
