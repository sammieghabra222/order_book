#include "engine/order_router.hpp"

std::vector<Trade> OrderRouter::submit(const OrderInput& order) {
    if (order.type == OrderType::Limit) {
        return book_for(order.symbol).submit_limit_order(order);
    } 
    return book_for(order.symbol).submit_market_order(order);
    
}

OrderBook& OrderRouter::book_for(const std::string& symbol) {
    auto it = books_.find(symbol);
    if (it == books_.end()) {
        it = books_.emplace(symbol, OrderBook(symbol)).first;
    }
    return it->second;
}
