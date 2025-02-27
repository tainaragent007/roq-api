/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace cache {

struct ROQ_PUBLIC ModifyOrder final {
  ModifyOrder(roq::ModifyOrder const &value)  // NOLINT
      : account{value.account}, order_id{value.order_id}, quantity{value.quantity}, price{value.price},
        routing_id{value.routing_id}, version{value.version}, conditional_on_version{value.conditional_on_version} {}

  ModifyOrder(ModifyOrder const &) = delete;
  ModifyOrder(ModifyOrder &&) = default;

  operator roq::ModifyOrder() const {
    return {
        .account = account,
        .order_id = order_id,
        .quantity = quantity,
        .price = price,
        .routing_id = routing_id,
        .version = version,
        .conditional_on_version = conditional_on_version,
    };
  }

  Account account;
  uint32_t order_id = {};
  double quantity = NaN;
  double price = NaN;
  RoutingId routing_id;
  uint32_t version = {};
  uint32_t conditional_on_version = {};
};

}  // namespace cache
}  // namespace roq
