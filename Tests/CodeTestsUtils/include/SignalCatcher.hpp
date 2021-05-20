#include <QObject>

namespace QtPiDeck::Tests {
class SignalCatcher {
public:
  template<class T, class... Args>
  SignalCatcher(T* obj, void (T::*signal)(Args...))
      : m_connection(QObject::connect(obj, signal, [this]() { inc(); })), m_count() {}

  SignalCatcher(SignalCatcher&&)      = delete;
  SignalCatcher(const SignalCatcher&) = delete;

  auto operator=(SignalCatcher&&) -> SignalCatcher& = delete;
  auto operator=(const SignalCatcher&) -> SignalCatcher& = delete;

  ~SignalCatcher() { QObject::disconnect(m_connection); }

  auto count() const noexcept -> uint64_t { return m_count; }

private:
  void inc() { ++m_count; }

  QMetaObject::Connection m_connection;
  uint64_t m_count;
};
}