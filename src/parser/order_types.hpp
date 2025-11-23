#pragma once

#include <cstdint>
#include <string>

enum class Side { Buy, Sell };
enum class OrderType { Limit, Market };

struct OrderInput {
    OrderType type{};
    Side side{};
    std::string symbol;
    double price{};            // only used for limit orders
    std::int64_t quantity{};
};
