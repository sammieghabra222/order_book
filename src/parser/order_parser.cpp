#include "parser/order_parser.hpp"

#include <cctype>
#include <sstream>

namespace {
std::string to_upper(std::string text) {
    for (char& c : text) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return text;
}
}  // namespace

ParseResult parse_line(const std::string& line) {
    std::istringstream iss(line);
    std::string type_str;
    std::string symbol;
    std::string side_str;

    if (!(iss >> type_str >> symbol >> side_str)) {
        return {std::nullopt, "Expected: <LIMIT|MARKET> <SYMBOL> <BUY|SELL> ..."};
    }

    OrderInput order{};
    order.symbol = symbol;

    type_str = to_upper(type_str);
    side_str = to_upper(side_str);

    if (type_str == "LIMIT") {
        order.type = OrderType::Limit;
    } else if (type_str == "MARKET") {
        order.type = OrderType::Market;
    } else {
        return {std::nullopt, "Order type must be LIMIT or MARKET"};
    }

    if (side_str == "BUY" || side_str == "B") {
        order.side = Side::Buy;
    } else if (side_str == "SELL" || side_str == "S") {
        order.side = Side::Sell;
    } else {
        return {std::nullopt, "Side must be BUY or SELL"};
    }

    if (order.type == OrderType::Limit) {
        if (!(iss >> order.price >> order.quantity)) {
            return {std::nullopt, "Limit order needs price and quantity"};
        }
        if (order.price <= 0.0) {
            return {std::nullopt, "Price must be positive"};
        }
    } else {
        if (!(iss >> order.quantity)) {
            return {std::nullopt, "Market order needs quantity"};
        }
    }

    if (order.quantity <= 0) {
        return {std::nullopt, "Quantity must be positive"};
    }

    return {order, {}};
}
