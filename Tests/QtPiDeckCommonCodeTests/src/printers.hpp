#include <optional>
#include <ostream>

#include "Network/MessageHeader.hpp"
#include "Network/Messages.hpp"

namespace std {
template<class T>
inline auto operator<<(std::ostream& ostr, const std::optional<T>& right) -> std::ostream& {
  if (right.has_value()) {
    ostr << right.value();
  } else {
    ostr << "(nil)";
  }
  return ostr;
}

inline auto operator<<(std::ostream& ostr, const std::nullopt_t& /*right*/) -> std::ostream& {
  ostr << "(nil)";
  return ostr;
}
}

namespace QtPiDeck {
namespace Network {
inline auto operator<<(std::ostream& ostr, const MessageType& right) -> std::ostream& {
  ostr << static_cast<uint32_t>(right);
  return ostr;
}

inline auto operator<<(std::ostream& ostr, const MessageHeader& right) -> std::ostream& {
  ostr << "MessageHeader{ dataSize: " << right.dataSize() << ", messageType: " << right.messageType()
       << ", RequestId: " << right.requestId().toStdString().c_str() << " }";
  return ostr;
}

inline namespace Messages {
inline auto operator<<(std::ostream& ostr, const Hello& right) -> std::ostream& {
  ostr << "Hello{ interfaceVersion: " << right.interfaceVersion << ", qcharSize: " << right.qcharSize << " }";
  return ostr;
}
}
}
}
