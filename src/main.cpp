#include <fstream>
#include <iostream>
#include <string>

#include "engine/order_router.hpp"
#include "parser/order_parser.hpp"
#include "repl.hpp"

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
