#pragma once

#include <boost/log/attributes/clock.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/core/record_view.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/formatting_ostream_fwd.hpp>

#include <string>

#include "QtPiDeckCommon_global.hpp"

namespace QtPiDeck::Utilities {
using severity =  boost::log::trivial::severity_level;

inline const std::string categoryAttribute{"category"};

inline auto getConstantAttributeValue(std::string value) -> boost::log::attributes::constant<std::string> {
  return boost::log::attributes::constant{std::move(value)};
}

inline void initLogger(boost::log::sources::severity_logger<boost::log::trivial::severity_level>& slg,
                       std::string_view sv) {
  slg.add_attribute(categoryAttribute, getConstantAttributeValue(std::string{sv}));
  slg.add_attribute("TimeStamp", boost::log::attributes::local_clock());
}

void QTPIDECKCOMMON_EXPORT initLogging(std::string_view execName);
}