#include "engine/order_book.hpp"
#include "parser/order_types.hpp"

OrderBook::OrderBook(std::string symbol) : symbol_(std::move(symbol)) {}

std::vector<Trade> OrderBook::submit_limit_order(const OrderInput& in) {
    OrderInput order = in;
    std::vector<Trade> trades;
    if (order.side == Side::Buy) {
        while (can_match_ask(order)) {
            auto head = asks_.begin();
            auto price = head->first;
            auto& orders = head->second;
            auto quantity = 0;
            while (!orders.empty() && order.quantity > 0) {
                auto& existing_bid = orders.front();
                if (existing_bid.quantity > order.quantity) {
                    existing_bid.quantity -= order.quantity;
                    quantity = quantity + order.quantity;
                    order.quantity = 0;
                } else {
                    order.quantity -= existing_bid.quantity;
                    quantity = quantity + existing_bid.quantity;
                    existing_bid.quantity = 0;
                    orders.pop_front();
                }
            }

            if (quantity > 0) {
                trades.push_back({symbol_, price, quantity});
            }

            if (orders.empty()) {
                asks_.erase(head);
            }
        }

        if (order.quantity > 0) {
            bids_[order.price].push_back(order);
        }
    } else {
        // match the sell order.
        while (can_match_bid(order)) {
            auto head = bids_.begin();
            auto price = head->first;
            auto& orders = head->second;
            auto quantity = 0;
            while (!orders.empty() && order.quantity > 0) {
                auto& existing_ask = orders.front();

                if (existing_ask.quantity > order.quantity) {
                    existing_ask.quantity -= order.quantity;
                    quantity = quantity + order.quantity;
                    order.quantity = 0;
                } else {
                    order.quantity -= existing_ask.quantity;
                    quantity = quantity + existing_ask.quantity;
                    existing_ask.quantity = 0;
                    orders.pop_front();
                }
            }

            if (quantity > 0) {
                trades.push_back({symbol_, price, quantity});
            }

            if (orders.empty()) {
                bids_.erase(head);
            }
        }

        if (order.quantity > 0) {
            asks_[order.price].push_back(order);
        }
    }
    return trades;
}

std::vector<Trade> OrderBook::submit_market_order(const OrderInput& in) {
    OrderInput order = in;
    std::vector<Trade> trades;
    if (order.side == Side::Buy) {
        while (!asks_.empty() && order.quantity > 0) {
            auto ask = asks_.begin();
            auto ask_price = ask->first;
            auto& orders = ask->second;
            int quantity = 0;
            while (!orders.empty() && order.quantity > 0) {
                auto& first = orders.front();

                if (order.quantity < first.quantity) {
                    quantity = quantity + order.quantity;
                    first.quantity = first.quantity - order.quantity;
                    order.quantity = 0;
                } else {
                    quantity = quantity + first.quantity;
                    order.quantity = order.quantity - first.quantity;
                    orders.pop_front();
                }
            }

            if (quantity > 0) {
                trades.push_back({symbol_, ask_price, quantity});
            }

            if (orders.empty()) {
                asks_.erase(ask);
            }
        }
        
    } else {
        while (!bids_.empty() && order.quantity > 0) {
            auto bid = bids_.begin();
            auto bid_price = bid->first;
            auto& orders = bid->second;
            int quantity = 0;

            while (!orders.empty() && order.quantity > 0) {
                auto& first = orders.front();
                if (order.quantity < first.quantity) {
                    quantity = quantity + order.quantity;
                    first.quantity = first.quantity - order.quantity;
                    order.quantity = 0;
                } else {
                    quantity = quantity + first.quantity;
                    order.quantity = order.quantity - first.quantity;
                    orders.pop_front();
                }
            }
            if (quantity > 0) {
                trades.push_back({symbol_, bid_price, quantity});
            }
            if (orders.empty()) {
                bids_.erase(bid);
            }
        }
    }
    return trades;
}

bool OrderBook::can_match_bid(const OrderInput& order) const noexcept {
    auto head = bids_.begin();
    return head != bids_.end() && head->first >= order.price && order.quantity > 0;
}

bool OrderBook::can_match_ask(const OrderInput& order) const noexcept {
    auto head = asks_.begin();
    return head != asks_.end() && head->first <= order.price && order.quantity > 0;
}
