#pragma once

#include <memory>

namespace QtPiDeck::Services {
template <class TService>
class ServiceUser;

class Ioc;

namespace detail {
template<class T>
void setService(QtPiDeck::Services::ServiceUser<T> & service, const Ioc & ioc) noexcept;
}

template <class TService>
class ServiceUser {
    template<class T>
    friend void detail::setService(ServiceUser<T> & service, const Ioc & ioc) noexcept; // NOLINT(readability-redundant-declaration)
protected:
    void setService(std::shared_ptr<TService> service) noexcept {
        m_service = std::move(service);
    }
    auto service() noexcept -> std::shared_ptr<TService>&  {
        return m_service;
    }
    auto service() const noexcept -> const std::shared_ptr<TService>&  {
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
protected:
    template<class TService>
    auto service() -> std::shared_ptr<TService> & {
        return ServiceUser<TService>::service();
    }
};
}
