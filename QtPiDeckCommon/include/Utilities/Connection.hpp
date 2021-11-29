#pragma once

#include <optional>

namespace QtPiDeck::Utilities {
class Connection {
public:
  Connection() = default;
  explicit Connection(QMetaObject::Connection connection) : m_connection(connection) {}
  Connection(const Connection&) = delete;
  Connection(Connection&& other) noexcept {
    m_connection = std::move(*other.m_connection);
    other.m_connection.reset();
  }
  auto operator=(const Connection&) -> Connection& = delete;
  auto operator                                    =(Connection&& other) noexcept -> Connection& {
    m_connection = std::move(*other.m_connection);
    other.m_connection.reset();
    return *this;
  };
  ~Connection() { reset(); }

  void reset() noexcept {
    if (m_connection) {
      QObject::disconnect(*m_connection);
      m_connection.reset();
    }
  }

private:
  std::optional<QMetaObject::Connection> m_connection;
};
}
