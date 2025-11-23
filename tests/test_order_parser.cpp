#include <gtest/gtest.h>

#include "parser/order_parser.hpp"
#include "parser/order_types.hpp"

TEST(ParserTest, ParsesLimitOrder) {
    const auto result = parse_line("LIMIT aapl buy 101.5 200");
    ASSERT_TRUE(result.order);

    const auto& order = *result.order;
    EXPECT_EQ(order.type, OrderType::Limit);
    EXPECT_EQ(order.side, Side::Buy);
    EXPECT_EQ(order.symbol, "AAPL");
    EXPECT_DOUBLE_EQ(order.price, 101.5);
    EXPECT_EQ(order.quantity, 200);
}

TEST(ParserTest, RejectsInvalidOrder) {
    const auto result = parse_line("LIMIT AAPL ???");
    EXPECT_FALSE(result.order);
    EXPECT_FALSE(result.error.empty());
}

TEST(ParserTest, RejectsMissingTokens) {
    const auto result = parse_line("LIMIT");
    EXPECT_FALSE(result.order);
}

TEST(ParserTest, RejectsInvalidType) {
    const auto result = parse_line("STOP AAPL BUY 101 10");
    EXPECT_FALSE(result.order);
}

TEST(ParserTest, RejectsInvalidSide) {
    const auto result = parse_line("LIMIT AAPL HOLD 101 10");
    EXPECT_FALSE(result.order);
}

TEST(ParserTest, RejectsNonPositivePrice) {
    const auto result = parse_line("LIMIT AAPL BUY 0 10");
    EXPECT_FALSE(result.order);
}

TEST(ParserTest, RejectsNonPositiveQuantityLimit) {
    const auto result = parse_line("LIMIT AAPL BUY 10 -5");
    EXPECT_FALSE(result.order);
}

TEST(ParserTest, RejectsMarketMissingQuantity) {
    const auto result = parse_line("MARKET AAPL BUY");
    EXPECT_FALSE(result.order);
}

TEST(ParserTest, RejectsMarketNonPositiveQuantity) {
    const auto result = parse_line("MARKET AAPL BUY 0");
    EXPECT_FALSE(result.order);
}
