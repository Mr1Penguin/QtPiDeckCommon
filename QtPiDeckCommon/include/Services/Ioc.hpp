#pragma once

#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include <QLoggingCategory>
#include <QObject>

#include "Services/ServiceInterface.hpp"
#include "Services/UseServices.hpp"

QTPIDECKCOMMON_EXPORT Q_DECLARE_LOGGING_CATEGORY(Ioc) // NOLINT

namespace QtPiDeck::Services {
enum class CreationType { SharedPointer, UniquePointer, Copy, RawInMemory, Raw };

class Ioc {
public:
  template <class TInterface, class TImplementation = TInterface>
  void registerService() noexcept {
    static_assert(std::is_base_of_v<TInterface, TImplementation>);
    static_assert(std::is_base_of_v<ServiceInterface, TInterface>);

    auto createWrapper = [] {
      using wrapperType = ServiceImplementationMetaWrapper<TImplementation>;
      return std::unique_ptr<ServiceImplementationMetaWrapperBase>(new wrapperType());
    };
    auto record = std::make_pair(createWrapper(), std::type_index{typeid(TImplementation)});
    auto it     = m_services.find({typeid(TInterface)});
    const auto end = std::cend(m_services);
    if ( it != end) {
      m_typeReferences.erase(it->second.second);
      it->second = std::move(record);
      qCDebug(::Ioc, "Replaced implementation for service '%s' with '%s'", std::type_index{typeid(TInterface)}.name(),
              std::type_index{typeid(TImplementation)}.name());
    } else {
      m_services.emplace(std::type_index{typeid(TInterface)}, std::move(record));
      qCDebug(::Ioc, "Added service '%s' with implementation '%s'", std::type_index{typeid(TInterface)}.name(),
              std::type_index{typeid(TImplementation)}.name());
    }

    m_typeReferences.emplace(std::type_index{typeid(TImplementation)}, std::type_index{typeid(TInterface)});
  }

  template <class TInterface>
  void registerSingleton(std::shared_ptr<TInterface> singleton) noexcept {
    static_assert(std::is_base_of_v<ServiceInterface, TInterface>);
    Q_ASSERT(singleton != nullptr); // LCOV_EXCL_BR_LINE
    if (auto it = m_singletons.find({typeid(TInterface)}); it != std::cend(m_singletons)) {
      it->second = std::move(singleton);
      qCDebug(::Ioc, "Replaced implementation for singleton '%s'", std::type_index{typeid(TInterface)}.name());
    } else {
      m_singletons.emplace(std::type_index{typeid(TInterface)}, std::move(singleton));
      qCDebug(::Ioc, "Add singleton '%s'", std::type_index{typeid(TInterface)}.name());
    }
  }

  template <class TService>
  [[nodiscard]] auto resolveService() const noexcept -> std::shared_ptr<TService> {
    if (const auto it = m_services.find({typeid(TService)}); it != std::cend(m_services)) {
      return std::shared_ptr<TService>(static_cast<TService*>(it->second.first->getRawImpl(*this)));
    }

    if (const auto it = m_typeReferences.find({typeid(TService)}); it != std::cend(m_typeReferences)) {
      const auto& wrapper = m_services.at(it->second).first;
      return std::shared_ptr<TService>(static_cast<TService*>(wrapper->getRawImpl(*this)));
    }

    if (const auto it = m_singletons.find({typeid(TService)}); it != std::cend(m_singletons)) {
      return std::static_pointer_cast<TService>(it->second);
    }

    qCDebug(::Ioc, "Service or singleton '%s' not found", std::type_index{typeid(TService)}.name());

    return nullptr;
  }

  template <class TObject, CreationType creationType = CreationType::UniquePointer>
  [[nodiscard]] auto make(void* memory = nullptr) const noexcept {
    if constexpr (!std::is_base_of_v<detail::HasDependecies, TObject>) {
      return createImpl<TObject, creationType>(memory);
    } else {
      return makeWithDependencies<TObject, creationType>(static_cast<TObject*>(nullptr), memory);
    }
  }

private:
  struct ServiceImplementationMetaWrapperBase {
    ServiceImplementationMetaWrapperBase() = default;
    ServiceImplementationMetaWrapperBase(const ServiceImplementationMetaWrapperBase&) = default;
    ServiceImplementationMetaWrapperBase(ServiceImplementationMetaWrapperBase&&) = default;
    virtual ~ServiceImplementationMetaWrapperBase() = default;
    auto operator=(const ServiceImplementationMetaWrapperBase&) -> ServiceImplementationMetaWrapperBase& = default;
    auto operator=(ServiceImplementationMetaWrapperBase&&) -> ServiceImplementationMetaWrapperBase& = default;
    [[nodiscard]] virtual auto getRawImpl(const Ioc& ioc) const noexcept -> ServiceInterface* = 0;
  };

  template <class TImplementation>
  struct ServiceImplementationMetaWrapper : ServiceImplementationMetaWrapperBase {
    [[nodiscard]] auto getRawImpl(const Ioc& ioc) const noexcept -> TImplementation* final {
      return ioc.make<TImplementation, CreationType::Raw>();
    }
  };

  template <class TImplementation, CreationType creationType, class... TDeps>
  auto makeWithDependencies(Services::UseServices<TDeps...>* /*deductor*/, void* memory = nullptr) const noexcept {
    if constexpr (std::is_constructible_v<TImplementation, std::shared_ptr<TDeps>...>) {
      return createImpl<TImplementation, creationType>(memory, resolveService<TDeps>()...);
    } else if constexpr (std::is_constructible_v<TImplementation, QObject*, std::shared_ptr<TDeps>...>) {
      return createImpl<TImplementation, creationType>(memory, nullptr, resolveService<TDeps>()...);
    } else {
      auto p = createImpl<TImplementation, creationType>(memory); // NOLINT(readability-qualified-auto)
      if constexpr (creationType == CreationType::Copy) {
        setServices(p);
      } else {
        setServices(*p);
      }

      return p;
    }
  }

  template <class TImplementation, CreationType creationType, class... TArgs>
  auto createImpl([[maybe_unused]] void* memory, TArgs... args) const noexcept {
    if constexpr (creationType == CreationType::SharedPointer) {
      return std::make_shared<TImplementation>(args...);
    } else if constexpr (creationType == CreationType::UniquePointer) {
      return std::make_unique<TImplementation>(args...);
    } else if constexpr (creationType == CreationType::Raw) {
      return new TImplementation(args...); // NOLINT(cppcoreguidelines-owning-memory)
                                           // I really don't want to include gsl for now
    } else if constexpr (creationType == CreationType::Copy) {
      return TImplementation{args...};
    } else {
      return new (memory) TImplementation(args...); // NOLINT(cppcoreguidelines-owning-memory)
    }
  }

  template <class... TServices>
  void setServices(UseServices<TServices...>& service) const noexcept {
    (service.template setService<TServices>(resolveService<TServices>()), ...);
  }

  using ValueType = std::pair<std::unique_ptr<ServiceImplementationMetaWrapperBase>, std::type_index>;
  std::unordered_map<std::type_index, ValueType> m_services;
  std::unordered_map<std::type_index, std::type_index> m_typeReferences;
  std::unordered_map<std::type_index, std::shared_ptr<ServiceInterface>> m_singletons;
};
}
