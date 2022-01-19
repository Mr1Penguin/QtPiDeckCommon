#pragma once

#include <memory>

namespace QtPiDeck::Services {
template<class TService>
class ServiceUser {
  friend class Ioc;

protected:
  void setService(std::shared_ptr<TService> service) noexcept { m_service = std::move(service); }

  [[nodiscard]] auto service() noexcept -> std::shared_ptr<TService>& { return m_service; }

  [[nodiscard]] auto service() const noexcept -> const std::shared_ptr<TService>& { return m_service; }

private:
  std::shared_ptr<TService> m_service;
};

namespace detail {
class HasDependecies {};
}

template<class... TServices>
class UseServices : public detail::HasDependecies, public ServiceUser<TServices>... {
  friend class Ioc;

public:
  using dependency_list = std::tuple<std::shared_ptr<TServices>...>;

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

  void setServices(dependency_list list) {
    (setService<TServices>(std::move(std::get<std::shared_ptr<TServices>>(list))), ...);
  }

  template<class TService>
  static auto service(dependency_list& list) -> std::shared_ptr<TService>& {
    return std::get<std::shared_ptr<TService>>(list);
  }

  template<class TService>
  [[nodiscard]] static auto service(const dependency_list& list) -> const std::shared_ptr<TService>& {
    return std::get<std::shared_ptr<TService>>(list);
  }
};
}
