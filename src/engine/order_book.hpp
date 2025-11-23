#pragma once

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "parser/order_types.hpp"

struct Trade {
    std::string symbol;
    double price{};
    std::int64_t quantity{};
};

class OrderBook {
public:
    explicit OrderBook(std::string symbol);

    std::vector<Trade> submit_limit_order(const OrderInput& order);
    std::vector<Trade> submit_market_order(const OrderInput& order);

    const std::string& symbol() const noexcept { return symbol_; }
    const std::map<double, std::deque<OrderInput>, std::greater<>>& bids() const noexcept { return bids_; }
    const std::map<double, std::deque<OrderInput>>& asks() const noexcept { return asks_; }

private:
    std::string symbol_;
    // price -> FIFO queue of resting orders on each side
    std::map<double, std::deque<OrderInput>, std::greater<>> bids_;  // highest price first
    std::map<double, std::deque<OrderInput>> asks_;                  // lowest price first
    bool can_match_bid(const OrderInput& order) const noexcept;
    bool can_match_ask(const OrderInput& order) const noexcept;
};
