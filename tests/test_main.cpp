#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

#include "engine/order_router.hpp"
#include "repl.hpp"

TEST(MainTest, ReplProcessesInputStream) {
    OrderRouter router;
    std::istringstream input(
        "# comment line\n"
        "LIMIT AAPL BUY 10 5\n"
        "MARKET AAPL SELL 3\n"
        "MARKET AAPL SELL\n"  // parse error
    );

    std::ostringstream buffer;
    auto* old_buf = std::cout.rdbuf(buffer.rdbuf());
    repl(input, router, false);
    std::cout.rdbuf(old_buf);
    const auto text = buffer.str();

    EXPECT_NE(text.find("Accepted order"), std::string::npos);
    EXPECT_NE(text.find("TRADE AAPL price=10 qty=3"), std::string::npos);
    EXPECT_NE(text.find("Parse error"), std::string::npos);
}
