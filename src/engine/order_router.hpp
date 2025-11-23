#pragma once

#include <map>
#include <string>
#include <vector>

#include "engine/order_book.hpp"

class OrderRouter {
public:
    // Route the order to the appropriate symbol book and return resulting trades.
    std::vector<Trade> submit(const OrderInput& order);

    OrderBook& book_for(const std::string& symbol);
    const std::map<std::string, OrderBook>& books() const noexcept { return books_; }

private:
    std::map<std::string, OrderBook> books_;
};
