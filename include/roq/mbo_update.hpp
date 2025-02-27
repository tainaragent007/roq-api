/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/numbers.hpp"
#include "roq/string_types.hpp"
#include "roq/update_action.hpp"

namespace roq {

//! Represents the update status of a single order in the order book
struct ROQ_PUBLIC MBOUpdate final {
  double price = NaN;              //!< Order price
  double remaining_quantity = {};  //!< Remaining order quantity
  UpdateAction action = {};        //!< Order update action
  uint32_t priority = {};          //!< Queue priority
  ExternalTradeId order_id;        //!< Order identifier
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBOUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::MBOUpdate const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(price={}, )"
        R"(remaining_quantity={}, )"
        R"(action={}, )"
        R"(priority={}, )"
        R"(order_id="{}")"
        R"(}})"_cf,
        value.price,
        value.remaining_quantity,
        value.action,
        value.priority,
        value.order_id);
  }
};
