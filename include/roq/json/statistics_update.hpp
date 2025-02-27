/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <range/v3/view.hpp>

#include "roq/statistics_update.hpp"

#include "roq/cache/statistics.hpp"

#include "roq/json/statistics.hpp"

#include "roq/json/datetime.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct StatisticsUpdate final {
  StatisticsUpdate(Context const &context, roq::StatisticsUpdate const &value) : context_{context}, value_{value} {}
  StatisticsUpdate(Context const &context, roq::StatisticsUpdate const &value, cache::Statistics const &cache)
      : context_{context}, value_{value}, cache_{&cache} {}

  template <typename Context>
  auto format_to(Context &context) const {
    if (cache_)
      return helper(context, (*cache_).statistics, UpdateType::SNAPSHOT);
    return helper(context, value_.statistics, value_.update_type);
  }

 protected:
  template <typename Context, typename T>
  auto helper(Context &context, T const &statistics, auto update_type) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("statistics":[{}],)"
        R"("update_type":{},)"
        R"("exchange_time_utc":{})"
        R"(}})"_cf,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        fmt::join(
            ranges::views::transform(
                ranges::views::remove_if(statistics, [](auto const &v) { return !std::isfinite(v.value); }),
                [this](auto const &v) {
                  return Statistics{context_, v};
                }),
            ","sv),
        String{update_type},
        DateTime{value_.exchange_time_utc});
  }

 private:
  Context const &context_;
  roq::StatisticsUpdate const &value_;
  cache::Statistics const *const cache_ = nullptr;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::StatisticsUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::StatisticsUpdate const &value, Context &context) const {
    return value.format_to(context);
  }
};
