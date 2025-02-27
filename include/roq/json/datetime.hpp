/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/utils/datetime.hpp"

namespace roq {
namespace json {

template <typename T>
struct DateTime final {
  explicit DateTime(T value) : value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace fmt::literals;
    if (value_.count())
      return fmt::format_to(context.out(), R"("{}")"_cf, utils::DateTime_iso8601{value_});
    return fmt::format_to(context.out(), "null"_cf);
  }

 private:
  T const value_;
};

}  // namespace json
}  // namespace roq

template <typename T>
struct fmt::formatter<roq::json::DateTime<T>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::DateTime<T> const &value, Context &context) const {
    return value.format_to(context);
  }
};
