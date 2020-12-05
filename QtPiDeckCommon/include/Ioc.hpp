#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <stdexcept>

#include "Services/ServiceInterface.hpp"

namespace QtPiDeck {
namespace detail {
struct ServiceStub : Services::ServiceInterface {};

// Ioc might be refactored to use std::any

struct ServiceImplementationMetaWrapperBase { // NOLINT
    virtual ~ServiceImplementationMetaWrapperBase() = default;
    virtual Services::ServiceInterface* getRawImpl() = 0; // NOLINT
};

template<class TImplementation>
struct ServiceImplementationMetaWrapper : ServiceImplementationMetaWrapperBase {
    using type = TImplementation;
    TImplementation* getRawImpl() final { return new TImplementation(); } // NOLINT
};

struct ServiceMetaWrapperBase { // NOLINT
    virtual ~ServiceMetaWrapperBase() = default;
    Services::ServiceInterface* getImpl() { return factory->getRawImpl(); } // NOLINT
    std::unique_ptr<ServiceImplementationMetaWrapperBase> factory; // NOLINT
};

template<class TInterface>
struct ServiceMetaWrapper : ServiceMetaWrapperBase {
    static_assert(std::is_base_of_v<Services::ServiceInterface, TInterface>);
    using type = TInterface;

    template<class TImplementation>
    ServiceMetaWrapper(TImplementation *) { // NOLINT
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
        if (auto it = std::find_if(std::begin(m_registeredServices), std::end(m_registeredServices), pred);
                it == std::end(m_registeredServices)) {
            m_registeredServices.push_back(std::make_shared<detail::ServiceMetaWrapper<TInterface>>(static_cast<TImplementation*>(nullptr)));
        }
        else {
            *it = std::make_shared<detail::ServiceMetaWrapper<TInterface>>(static_cast<TImplementation*>(nullptr));
        }
    }

    template<class TInterface>
    void RegisterSingleton(std::shared_ptr<TInterface> singleton) {
        static_assert(std::is_base_of_v<Services::ServiceInterface, TInterface>);
        auto pred = [](std::shared_ptr<Services::ServiceInterface> & service) {
            return dynamic_cast<TInterface*>(service.get()) != nullptr;
        };
        if (auto it = std::find_if(std::begin(m_singletons), std::end(m_singletons), pred);
                it == std::end(m_singletons)) {
            m_singletons.emplace_back(std::move(singleton));
        }
        else {
            *it = std::move(singleton);
        }
    }

    template<class TService>
    auto ResolveService() -> std::shared_ptr<TService> {
        auto pred = [] (std::shared_ptr<detail::ServiceMetaWrapperBase> & s) {
            using ImplType = std::conditional_t<std::is_abstract_v<TService>, detail::ServiceStub, TService>;
            return dynamic_cast<detail::ServiceMetaWrapper<TService>*>(s.get()) != nullptr ||
                    dynamic_cast<detail::ServiceImplementationMetaWrapper<ImplType>*>(s->factory.get()) != nullptr;
        };
        if (auto it = std::find_if(std::begin(m_registeredServices), std::end(m_registeredServices), pred);
                it != std::end(m_registeredServices)) {
            return std::shared_ptr<TService>(dynamic_cast<TService*>((*it)->getImpl()));
        }

        auto singletonPred = [](std::shared_ptr<Services::ServiceInterface> & singleton) {
            return dynamic_cast<TService*>(singleton.get()) != nullptr;
        };
        if (auto it = std::find_if(std::begin(m_singletons), std::end(m_singletons), singletonPred);
                it != std::end(m_singletons)) {
            return std::dynamic_pointer_cast<TService>(*it);
        }

        return nullptr;
    }

private:
    std::vector<std::shared_ptr<Services::ServiceInterface>> m_singletons;
    std::vector<std::shared_ptr<detail::ServiceMetaWrapperBase>> m_registeredServices;
};
}
