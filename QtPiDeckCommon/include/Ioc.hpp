#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <stdexcept>

#include "Services/ServiceInterface.hpp"

namespace QtPiDeck {
namespace detail {
struct ServiceImplementationMetaWrapperBase {
    virtual ~ServiceImplementationMetaWrapperBase() {}
    virtual Services::ServiceInterface* getRawImpl() = 0;
};

template<class TImplementation>
struct ServiceImplementationMetaWrapper : ServiceImplementationMetaWrapperBase {
    using type = TImplementation;
    TImplementation* getRawImpl() final { return new TImplementation(); }
};

struct ServiceMetaWrapperBase {
    virtual ~ServiceMetaWrapperBase() {}
    Services::ServiceInterface* getImpl() { return factory->getRawImpl(); }
    std::unique_ptr<ServiceImplementationMetaWrapperBase> factory;
};

template<class TInterface>
struct ServiceMetaWrapper : ServiceMetaWrapperBase {
    static_assert(std::is_base_of_v<Services::ServiceInterface, TInterface>);
    using type = TInterface;

    template<class TImplementation>
    ServiceMetaWrapper(TImplementation *) {
        static_assert(std::is_base_of_v<TInterface, TImplementation>);
        factory = std::make_unique<ServiceImplementationMetaWrapper<TImplementation>>();
    }
};
}

class Ioc {
public:
    template<class TInterface, class TImplementation>
    void RegisterService() {
        static_assert(std::is_base_of_v<TInterface, TImplementation>);
        static_assert(std::is_base_of_v<Services::ServiceInterface, TInterface>);
        auto pred = [] (std::shared_ptr<detail::ServiceMetaWrapperBase> & s) {
            return dynamic_cast<detail::ServiceMetaWrapper<TInterface>*>(s.get()) != nullptr;
        };
        auto it = std::find_if(std::begin(m_registeredServices), std::end(m_registeredServices), pred);
        if (it == std::end(m_registeredServices)) {
            m_registeredServices.push_back(std::make_unique<detail::ServiceMetaWrapper<TInterface>>(static_cast<TImplementation*>(nullptr)));
        }
        else {
            *it = std::make_unique<detail::ServiceMetaWrapper<TInterface>>(static_cast<TImplementation*>(nullptr));
        }
    }    

    template<class TService>
    std::shared_ptr<TService> ResolveService() {
        auto pred = [] (std::shared_ptr<detail::ServiceMetaWrapperBase> & s) {
            return dynamic_cast<detail::ServiceMetaWrapper<TService>*>(s.get()) != nullptr ||
                    dynamic_cast<detail::ServiceImplementationMetaWrapper<TService>*>(s->factory.get()) != nullptr;
        };
        auto it = std::find_if(std::begin(m_registeredServices), std::end(m_registeredServices), pred);
        if (it != std::end(m_registeredServices)) {
            return std::shared_ptr<TService>(dynamic_cast<TService*>((*it)->getImpl()));
        }

        return nullptr;
    }

private:
    std::vector<std::shared_ptr<Services::ServiceInterface>> m_singletons;
    std::vector<std::shared_ptr<detail::ServiceMetaWrapperBase>> m_registeredServices;
};
}
