/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <magic_enum.hpp>

#include <fmt/compile.h>
#include <fmt/format.h>

namespace roq {

//! Enumeration of error types
enum class Error : uint8_t {
  UNDEFINED = 0,
  UNKNOWN,
  NOT_SUPPORTED,  //!< Request type not supported
  INVALID_ACCOUNT,
  INVALID_ORDER_ID,
  INVALID_EXCHANGE,
  INVALID_SYMBOL,
  INVALID_SIDE,
  INVALID_POSITION_EFFECT,
  INVALID_QUANTITY,
  INVALID_MAX_SHOW_QUANTITY,
  INVALID_ORDER_TYPE,
  INVALID_TIME_IN_FORCE,
  INVALID_EXECUTION_INSTRUCTION,
  INVALID_ORDER_TEMPLATE,
  INVALID_PRICE,
  INVALID_STOP_PRICE,
  INVALID_ROUTING_ID,
  INVALID_REQUEST_VERSION,
  INVALID_REQUEST_ID,
  INVALID_REQUEST_TYPE,
  INVALID_REQUEST_STATUS,
  INVALID_REQUEST_ARGS,            //!< Request arguments did not meet validation rules
  UNKNOWN_EXTERNAL_ORDER_ID,       //!< An external order identifier has not yet been received
  NOT_AUTHORIZED,                  //!< User is not authorized
  REQUEST_RATE_LIMIT_REACHED,      //!< Request rate limit has been reached
  GATEWAY_NOT_READY,               //!< Gateway not ready (could be disconnected or currently downloading)
  NETWORK_ERROR,                   //!< Network disconnect event (unknown request/order status)
  TIMEOUT,                         //!< Response has not been received (unknown request/order status)
  PARSE_ERROR,                     //!< Message parse error (unknown request/order status)
  MODIFY_HAS_NO_EFFECT,            //!< Modify request has no effect on order
  TOO_LATE_TO_MODIFY_OR_CANCEL,    //!< Too late to modify or cancel (order has been filled or is already canceled)
  CONDITIONAL_REQUEST_HAS_FAILED,  //!< Happens when gateway detects chain failure
  UNKNOWN_ORDER_ID,
  INSUFFICIENT_FUNDS,  //!< Insufficient funds for order action
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Error> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Error const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, magic_enum::enum_name(value));
  }
};
