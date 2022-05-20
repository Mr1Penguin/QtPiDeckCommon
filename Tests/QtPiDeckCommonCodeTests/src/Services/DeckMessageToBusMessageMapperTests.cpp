#include "BoostUnitTest.hpp"

#include "Services/DeckMessageToBusMessageMapper.hpp"

#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(DeckMessageToBusMessageMapperTests) // NOLINT

using namespace QtPiDeck::Bus;
using namespace QtPiDeck::Services;
using namespace QtPiDeck::Network;

BOOST_AUTO_TEST_CASE(returnNulloptForUnknownType) // NOLINT
{
  const auto mapper   = DeckMessageToBusMessageMapper{};
  constexpr auto type = MessageType::Reserved;
  BOOST_TEST(mapper.getBusMessageType(type) == std::nullopt); // NOLINT
}

BOOST_AUTO_TEST_CASE(returnValueForKnownType) // NOLINT
{
  const auto mapper   = DeckMessageToBusMessageMapper{};
  constexpr auto type = MessageType::Dummy;
  BOOST_TEST(mapper.getBusMessageType(type) == Dummy::DummyId); // NOLINT
}

template<bool hasValue>
class derived final : public DeckMessageToBusMessageMapper {
protected:
  [[nodiscard]] auto getType(MessageType /*type*/) const
      -> std::optional<decltype(QtPiDeck::Bus::Message::messageType)> final {
    if (hasValue) {
      return uint64_t{1};
    }

    return std::nullopt;
  }
};

BOOST_AUTO_TEST_CASE(returnNulloptForUnknownTypeDerived) // NOLINT
{
  const auto mapper   = derived<false>{};
  constexpr auto type = MessageType::Reserved;
  BOOST_TEST(mapper.getBusMessageType(type) == std::nullopt); // NOLINT
}

BOOST_AUTO_TEST_CASE(returnValueForKnownTypeByParent) // NOLINT
{
  const auto mapper   = derived<false>{};
  constexpr auto type = MessageType::Dummy;
  BOOST_TEST(mapper.getBusMessageType(type) == Dummy::DummyId); // NOLINT
}

BOOST_AUTO_TEST_CASE(returnValueForKnownTypeDerived) // NOLINT
{
  const auto mapper   = derived<true>{};
  constexpr auto type = MessageType::Reserved;
  BOOST_TEST(mapper.getBusMessageType(type) == uint64_t{1}); // NOLINT
}

BOOST_AUTO_TEST_SUITE_END() // NOLINT
