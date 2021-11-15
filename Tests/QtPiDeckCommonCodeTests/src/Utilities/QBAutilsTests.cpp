// NOLINTNEXTLINE
#define BOOST_TEST_MODULE LiteralTests
#include "BoostUnitTest.hpp"

#include <QString>
#include <QIODevice>

#include "Utilities/QBAutils.hpp"
#include "Utilities/Logging.hpp"

auto main(int argc, char* argv[]) -> int {
  QtPiDeck::Utilities::initLogging("qbautilsTests");
  return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

CT_BOOST_AUTO_TEST_SUITE(qbautilsTests)
CT_BOOST_AUTO_TEST_CASE(unpackQString) {
  const auto data = QString{"example"};
  auto byteArray  = QByteArray{};
  QDataStream ds{&byteArray, QIODevice::WriteOnly};
  ds << data;
  const auto result = QtPiDeck::Utilities::convert<QString>(byteArray);
  CT_BOOST_TEST(data == result);
}

CT_BOOST_AUTO_TEST_CASE(unpackInt32) {
  const auto data = int32_t{-25};
  auto byteArray  = QByteArray{};
  QDataStream ds{&byteArray, QIODevice::WriteOnly};
  ds << data;
  const auto result = QtPiDeck::Utilities::convert<int32_t>(byteArray);
  CT_BOOST_TEST(data == result);
}

CT_BOOST_AUTO_TEST_CASE(unpackUint32) {
  const auto data = uint32_t{12};
  auto byteArray  = QByteArray{};
  QDataStream ds{&byteArray, QIODevice::WriteOnly};
  ds << data;
  const auto result = QtPiDeck::Utilities::convert<uint32_t>(byteArray);
  CT_BOOST_TEST(data == result);
}

CT_BOOST_AUTO_TEST_SUITE_END()