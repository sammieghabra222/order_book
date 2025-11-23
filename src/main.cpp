#include <fstream>
#include <iostream>
#include <string>

#include "engine/order_router.hpp"
#include "parser/order_parser.hpp"

void repl(std::istream& input, OrderRouter& router, bool interactive) {
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty() || line[0] == '#') {
            if (interactive) std::cout << "> ";
            continue;
        }

        auto parsed = parse_line(line);
        if (!parsed.order) {
            std::cout << "Parse error: " << parsed.error << "\n";
            if (interactive) std::cout << "> ";
            continue;
        }

        const auto& order = *parsed.order;
        auto trades = router.submit(order);

        std::cout << "Accepted order -> type=" << (order.type == OrderType::Limit ? "LIMIT" : "MARKET")
                  << " side=" << (order.side == Side::Buy ? "BUY" : "SELL")
                  << " symbol=" << order.symbol
                  << " qty=" << order.quantity;
        if (order.type == OrderType::Limit) std::cout << " price=" << order.price;
        std::cout << "\n";

        if (!trades.empty()) {
            for (const auto& t : trades) {
                std::cout << "TRADE " << t.symbol << " price=" << t.price << " qty=" << t.quantity
                         << "\n";
            }
        }
        if (interactive) std::cout << "> ";
    }
}

int main(int argc, char* argv[]) {
    OrderRouter router;

    if (argc > 1) {
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
        // text file mode - non interactive mode.
        std::cout << "Reading from file.\n";
        repl(file, router, false);
    } else {
        // interactive mode.
        std::cout << "Enter orders: LIMIT <SYMBOL> <BUY|SELL> <PRICE> <QTY> or MARKET <SYMBOL> <BUY|SELL> <QTY>\n";
        std::cout << "> ";
        repl(std::cin, router, true);
    }
    return 0;
}
