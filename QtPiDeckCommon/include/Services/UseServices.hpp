#pragma once

#include <memory>

namespace QtPiDeck::Services {
template <class TService>
class ServiceUser {
    friend class Ioc;
protected:
    void setService(std::shared_ptr<TService> service) noexcept {
        m_service = std::move(service);
    }

    [[nodiscard]] auto service() noexcept -> std::shared_ptr<TService>&  {
        return m_service;
    }

    [[nodiscard]] auto service() const noexcept -> const std::shared_ptr<TService>&  {
        return m_service;
    }
private:
    std::shared_ptr<TService> m_service;
};

namespace detail {
class HasDependecies {};
}

template <class... TServices>
class UseServices : public detail::HasDependecies, public ServiceUser<TServices>... {
    friend class Ioc;
protected:
    template<class TService>
    [[nodiscard]] auto service() const noexcept -> const std::shared_ptr<TService>& {
        return ServiceUser<TService>::service();
    }

    template<class TService>
    auto service() noexcept -> std::shared_ptr<TService>& {
        return ServiceUser<TService>::service();
    }

    template<class TService>
    void setService(std::shared_ptr<TService> service) noexcept {
        ServiceUser<TService>::setService(std::move(service));
    }
};
}
