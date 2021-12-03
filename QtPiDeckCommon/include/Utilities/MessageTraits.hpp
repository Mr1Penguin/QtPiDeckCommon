#pragma once

#include <cstdint>

#include "boost/tti/has_member_function.hpp"

#include <QString>

#include "Network/MessageHeader.hpp"

namespace QtPiDeck::Network {
BOOST_TTI_HAS_MEMBER_FUNCTION(messageSize)
template<class T>
constexpr bool has_messageSize = has_member_function_messageSize<T, uint64_t>::value;
BOOST_TTI_HAS_MEMBER_FUNCTION(messageHeader)
template<class T>
constexpr bool has_messageHeader =
    has_member_function_messageHeader<T, MessageHeader, boost::mpl::vector<const QString&>>::value;

template<class T>
constexpr bool is_valid_message = has_messageSize<T>&& has_messageHeader<T>;
}