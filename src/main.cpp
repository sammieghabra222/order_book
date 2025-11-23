#include <iostream>
#include <fstream>
#include <string>

#include "parser/order_parser.hpp"

void repl(std::istream& input, bool interactive) {

    if (interactive) {
        std::cout << "Enter orders: LIMIT <SYMBOL> <BUY|SELL> <PRICE> <QTY> or MARKET <SYMBOL> <BUY|SELL> <QTY>\n";
        std::cout << "> ";
    } else {
        std::cout << "Reading from file." << std::endl;
    }

    std::string line;
    while (std::getline(input, line)) {
        if (line.empty() || line[0] == '#') {
            if (interactive) std::cout << "> ";
            continue;
        }

        auto parsed = parse_line(line);
        if (!parsed.order) {
            std::cout << "Parse error: " << parsed.error << "\n> ";
            if (interactive) std::cout << "> ";
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
        if (interactive) std::cout << "> ";
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
        // text file mode - non interactive mode.
        repl(file, false);
    } else {
        // interactive mode.
        repl(std::cin, true);
    }
    return 0;
}
