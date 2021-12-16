#pragma once

namespace QtPiDeck::Utilities {
template<class TAction>
class OnExit {
public:
  explicit OnExit(TAction action) : m_action(std::move(action)) {}
  ~OnExit() { m_action(); }
  OnExit(const OnExit&) = delete;
  OnExit(OnExit&&)      = delete;
  auto operator=(const OnExit&) -> OnExit& = delete;
  auto operator=(OnExit&&) -> OnExit& = delete;

private:
  TAction m_action;
};
}