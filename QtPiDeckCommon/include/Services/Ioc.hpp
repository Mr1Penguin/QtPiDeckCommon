#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <algorithm>

#include "Services/ServiceInterface.hpp"
#include "Services/UseServices.hpp"

namespace QtPiDeck::Services {
class Ioc;

namespace detail {
template<class... TServices>
void SetServices(UseServices<TServices...> & service, const Ioc & ioc) noexcept {
   (detail::SetService<TServices>(service, ioc),...);
}

struct ServiceStub : ServiceInterface {};

struct ServiceImplementationMetaWrapperBase {
    friend struct ServiceMetaWrapperBase;
    ServiceImplementationMetaWrapperBase() = default;
    ServiceImplementationMetaWrapperBase(const ServiceImplementationMetaWrapperBase&) = default;
    ServiceImplementationMetaWrapperBase(ServiceImplementationMetaWrapperBase&&) = default;
    virtual ~ServiceImplementationMetaWrapperBase() = default;
    auto operator=(const ServiceImplementationMetaWrapperBase&) -> ServiceImplementationMetaWrapperBase& = default;
    auto operator=(ServiceImplementationMetaWrapperBase&&) -> ServiceImplementationMetaWrapperBase& = default;
private:
    virtual auto getRawImpl(const Ioc & ioc) noexcept -> ServiceInterface* = 0;
};

template<class TImplementation>
struct ServiceImplementationMetaWrapper : ServiceImplementationMetaWrapperBase {
    using type = TImplementation;
private:
    auto getRawImpl(const Ioc & ioc) noexcept -> TImplementation* final {
        auto* ptr = new TImplementation(); // NOLINT(cppcoreguidelines-owning-memory) unable to use covariance with smart pointers
        if constexpr (std::is_base_of_v<detail::HasDependecies, TImplementation>) {
            SetServices(*ptr, ioc);
        }
        return ptr;
    }
};

struct ServiceMetaWrapperBase {
    ServiceMetaWrapperBase() = default;
    ServiceMetaWrapperBase(std::unique_ptr<ServiceImplementationMetaWrapperBase> factory) : m_factory(std::move(factory)) {}
    ServiceMetaWrapperBase(const ServiceMetaWrapperBase&) = delete;
    ServiceMetaWrapperBase(ServiceMetaWrapperBase&&) = default;
    virtual ~ServiceMetaWrapperBase() = default;
    auto operator=(const ServiceMetaWrapperBase&) -> ServiceMetaWrapperBase& = delete;
    auto operator=(ServiceMetaWrapperBase&&) -> ServiceMetaWrapperBase& = default;
    auto getImpl(const Ioc & ioc) noexcept -> ServiceInterface* { return m_factory->getRawImpl(ioc); }
    auto factory() noexcept -> std::unique_ptr<ServiceImplementationMetaWrapperBase>& { return m_factory; }
private:
    std::unique_ptr<ServiceImplementationMetaWrapperBase> m_factory;
};

template<class TInterface>
struct ServiceMetaWrapper : ServiceMetaWrapperBase {
    static_assert(std::is_base_of_v<ServiceInterface, TInterface>);
    using type = TInterface;

    template<class TImplementation>
    ServiceMetaWrapper(TImplementation * /*implementation*/)
        : ServiceMetaWrapperBase(std::make_unique<ServiceImplementationMetaWrapper<TImplementation>>()) {
        static_assert(std::is_base_of_v<TInterface, TImplementation>);
    }
};
}

class Ioc {
public:
    template<class TInterface, class TImplementation>
    void registerService() noexcept {
        static_assert(std::is_base_of_v<TInterface, TImplementation>);
        static_assert(std::is_base_of_v<ServiceInterface, TInterface>);
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
    void registerSingleton(std::shared_ptr<TInterface> singleton) noexcept {
        static_assert(std::is_base_of_v<ServiceInterface, TInterface>);
        auto pred = [](std::shared_ptr<ServiceInterface> & service) {
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
    auto resolveService() const noexcept -> std::shared_ptr<TService> {
        auto pred = [] (const std::shared_ptr<detail::ServiceMetaWrapperBase> & s) {
            using ImplType = std::conditional_t<std::is_abstract_v<TService>, detail::ServiceStub, TService>;
            return dynamic_cast<detail::ServiceMetaWrapper<TService>*>(s.get()) != nullptr ||
                    dynamic_cast<detail::ServiceImplementationMetaWrapper<ImplType>*>(s->factory().get()) != nullptr;
        };
        if (auto it = std::find_if(std::cbegin(m_registeredServices), std::cend(m_registeredServices), pred);
                it != std::cend(m_registeredServices)) {
            auto service = std::shared_ptr<TService>(dynamic_cast<TService*>((*it)->getImpl(*this)));
            return service;
        }

        auto singletonPred = [](const std::shared_ptr<ServiceInterface> & singleton) {
            return dynamic_cast<TService*>(singleton.get()) != nullptr;
        };
        if (auto it = std::find_if(std::cbegin(m_singletons), std::cend(m_singletons), singletonPred);
                it != std::cend(m_singletons)) {
            auto service = std::dynamic_pointer_cast<TService>(*it);
            return service;
        }

        return nullptr;
    }

private:
    std::vector<std::shared_ptr<ServiceInterface>> m_singletons;
    std::vector<std::shared_ptr<detail::ServiceMetaWrapperBase>> m_registeredServices;
};

namespace detail {
template<class TService>
void SetService(ServiceUser<TService> & service, const Ioc & ioc) noexcept {
   service.SetService(ioc.resolveService<TService>());
}
}
}
