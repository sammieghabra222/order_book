#include <iostream>
#include <string>

#include "parser/order_parser.hpp"

void repl() {
    std::cout << "Enter orders: LIMIT <SYMBOL> <BUY|SELL> <PRICE> <QTY> or MARKET <SYMBOL> <BUY|SELL> <QTY>\n";
    std::cout << "> ";

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty() || line[0] == '#') {
            std::cout << "> ";
            continue;
        }

        auto parsed = parse_line(line);
        if (!parsed.order) {
            std::cout << "Parse error: " << parsed.error << "\n> ";
            continue;
        }

        const auto& order = *parsed.order;
        std::cout << "Parsed order -> type=" << (order.type == OrderType::Limit ? "LIMIT" : "MARKET")
                  << " side=" << (order.side == Side::Buy ? "BUY" : "SELL")
                  << " symbol=" << order.symbol
                  << " qty=" << order.quantity;
        if (order.type == OrderType::Limit) {
            std::cout << " price=" << order.price;
        }
        std::cout << "\n> ";
    }
}

int main() {
    repl();
    return 0;
}
