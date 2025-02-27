/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/mbp_update.hpp"

#include "roq/json/context.hpp"
#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct MBPUpdate final {
  MBPUpdate(Context const &context, roq::MBPUpdate const &value) : context_{context}, value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("price":{},)"
        R"("quantity":{},)"
        R"("implied_quantity":{},)"
        R"("number_of_orders":{},)"
        R"("update_action":{},)"
        R"("price_level":{})"
        R"(}})"_cf,
        Number{value_.price, context_.price_decimals},
        Number{value_.quantity, context_.quantity_decimals},
        Number{value_.implied_quantity},
        value_.number_of_orders,
        String{value_.update_action},
        value_.price_level);
  }

 private:
  Context const &context_;
  roq::MBPUpdate const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::MBPUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::MBPUpdate const &value, Context &context) const {
    return value.format_to(context);
  }
};
