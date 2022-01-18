#pragma once

#include <type_traits>

namespace QtPiDeck::Utilities {
#if __cpp_concepts < 201907L
template<class Derived, class Base>
constexpr bool derived_from =
    std::is_base_of_v<Base, Derived>&& std::is_convertible_v<const volatile Derived*, const volatile Base*>;
#endif

template<class ViewModel>
class ViewModelTraits {
private:
  static void Check() { [[maybe_unused]] void (*registerType)() = &ViewModel::registerType; }
};
}