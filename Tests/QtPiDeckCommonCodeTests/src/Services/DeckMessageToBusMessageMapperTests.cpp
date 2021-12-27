#include "BoostUnitTest.hpp"

#include "Services/DeckMessageToBusMessageMapper.hpp"

#include "printers.hpp"

CT_BOOST_AUTO_TEST_SUITE(DeckMessageToBusMessageMapperTests)

using namespace QtPiDeck::Bus;
using namespace QtPiDeck::Services;
using namespace QtPiDeck::Network;

CT_BOOST_AUTO_TEST_CASE(returnNulloptForUnknownType) {
  const auto mapper = DeckMessageToBusMessageMapper{};
  constexpr auto type = MessageType::Reserved;
  CT_BOOST_TEST(mapper.getBusMessageType(type) == std::nullopt);
}

CT_BOOST_AUTO_TEST_CASE(returnValueForKnownType) {
  const auto mapper   = DeckMessageToBusMessageMapper{};
  constexpr auto type = MessageType::Dummy;
  CT_BOOST_TEST(mapper.getBusMessageType(type) == Dummy::DummyId);
}

template<bool hasValue>
class derived final : public DeckMessageToBusMessageMapper {
protected:
  auto getType(MessageType /*type*/) const -> std::optional<decltype(QtPiDeck::Bus::Message::messageType)> final {
    if (hasValue) {
      return uint64_t{1};
    }
    
    return std::nullopt;
  }
};

CT_BOOST_AUTO_TEST_CASE(returnNulloptForUnknownTypeDerived) {
  const auto mapper   = derived<false>{};
  constexpr auto type = MessageType::Reserved;
  CT_BOOST_TEST(mapper.getBusMessageType(type) == std::nullopt);
}

CT_BOOST_AUTO_TEST_CASE(returnValueForKnownTypeByParent) {
  const auto mapper   = derived<false>{};
  constexpr auto type = MessageType::Dummy;
  CT_BOOST_TEST(mapper.getBusMessageType(type) == Dummy::DummyId);
}

CT_BOOST_AUTO_TEST_CASE(returnValueForKnownTypeDerived) {
  const auto mapper   = derived<true>{};
  constexpr auto type = MessageType::Reserved;
  CT_BOOST_TEST(mapper.getBusMessageType(type) == uint64_t{1});
}

CT_BOOST_AUTO_TEST_SUITE_END()