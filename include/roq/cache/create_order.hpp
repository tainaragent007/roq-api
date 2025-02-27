/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace cache {

struct ROQ_PUBLIC CreateOrder final {
  CreateOrder(roq::CreateOrder const &value)  // NOLINT
      : account{value.account}, order_id{value.order_id}, exchange{value.exchange}, symbol{value.symbol},
        side{value.side}, position_effect{value.position_effect}, time_in_force{value.time_in_force},
        execution_instructions{value.execution_instructions}, order_template{value.order_template},
        quantity{value.quantity}, price{value.price}, stop_price{value.stop_price}, routing_id{value.routing_id} {}

  CreateOrder(CreateOrder const &) = delete;
  CreateOrder(CreateOrder &&) = default;

  operator roq::CreateOrder() const {
    return {
        .account = account,
        .order_id = order_id,
        .exchange = exchange,
        .symbol = symbol,
        .side = side,
        .position_effect = position_effect,
        .time_in_force = time_in_force,
        .execution_instructions = execution_instructions,
        .order_template = order_template,
        .quantity = quantity,
        .price = price,
        .stop_price = stop_price,
        .routing_id = routing_id,
    };
  }

  Account account;
  uint32_t order_id = {};
  Exchange exchange;
  Symbol symbol;
  Side side = {};
  PositionEffect position_effect = {};
  TimeInForce time_in_force = {};
  Mask<ExecutionInstruction> execution_instructions;
  OrderTemplate order_template;
  double quantity = NaN;
  double price = NaN;
  double stop_price = NaN;
  RoutingId routing_id;
};

}  // namespace cache
}  // namespace roq
