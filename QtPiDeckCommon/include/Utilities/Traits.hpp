#pragma once

#include <type_traits>

namespace QtPiDeck::Utilities {
template<class ViewModel>
class ViewModelTraits {
private:
  static void Check() { [[maybe_unused]] void (*registerType)() = &ViewModel::registerType; }
};
}