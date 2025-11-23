#pragma once

#include <deque>
#include <map>

#include "parser/order_types.hpp"

inline std::int64_t total_quantity(const std::map<double, std::deque<OrderInput>, std::greater<>>& book_side) {
    std::int64_t total = 0;
    for (const auto& [_, orders] : book_side) {
        for (const auto& order : orders) {
            total += order.quantity;
        }
    }
    return total;
}

inline std::int64_t total_quantity(const std::map<double, std::deque<OrderInput>>& book_side) {
    std::int64_t total = 0;
    for (const auto& [_, orders] : book_side) {
        for (const auto& order : orders) {
            total += order.quantity;
        }
    }
    return total;
}
