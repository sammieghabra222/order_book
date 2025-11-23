#include <gtest/gtest.h>

#include "book_test_helpers.hpp"
#include "engine/order_book.hpp"
#include "parser/order_types.hpp"

TEST(OrderBookTest, LimitBuyMatchesExistingAskAndRestsRemainder) {
    OrderBook book("AAPL");
    book.submit_limit_order({OrderType::Limit, Side::Sell, "AAPL", 101.0, 100});

    const auto trades = book.submit_limit_order({OrderType::Limit, Side::Buy, "AAPL", 102.0, 150});

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_DOUBLE_EQ(trades[0].price, 101.0);
    EXPECT_EQ(trades[0].quantity, 100);
    EXPECT_EQ(total_quantity(book.bids()), 50);
    EXPECT_EQ(total_quantity(book.asks()), 0);
}

TEST(OrderBookTest, LimitSellMatchesExistingBidWithFifo) {
    OrderBook book("MSFT");
    book.submit_limit_order({OrderType::Limit, Side::Buy, "MSFT", 250.0, 30});
    book.submit_limit_order({OrderType::Limit, Side::Buy, "MSFT", 250.0, 40});

    const auto trades = book.submit_limit_order({OrderType::Limit, Side::Sell, "MSFT", 249.0, 50});

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_DOUBLE_EQ(trades[0].price, 250.0);
    EXPECT_EQ(trades[0].quantity, 50);
    const auto& remaining_orders = book.bids().begin()->second;
    ASSERT_EQ(remaining_orders.size(), 1u);
    EXPECT_EQ(remaining_orders.front().quantity, 20);
}

TEST(OrderBookTest, MarketBuyConsumesBestAsks) {
    OrderBook book("GOOG");
    book.submit_limit_order({OrderType::Limit, Side::Sell, "GOOG", 100.0, 50});
    book.submit_limit_order({OrderType::Limit, Side::Sell, "GOOG", 101.0, 100});

    const auto trades = book.submit_market_order({OrderType::Market, Side::Buy, "GOOG", 0.0, 120});

    ASSERT_EQ(trades.size(), 2u);
    EXPECT_DOUBLE_EQ(trades[0].price, 100.0);
    EXPECT_EQ(trades[0].quantity, 50);
    EXPECT_DOUBLE_EQ(trades[1].price, 101.0);
    EXPECT_EQ(trades[1].quantity, 70);
    EXPECT_EQ(total_quantity(book.asks()), 30);
}

TEST(OrderBookTest, MarketSellConsumesBestBids) {
    OrderBook book("TSLA");
    book.submit_limit_order({OrderType::Limit, Side::Buy, "TSLA", 200.0, 60});
    book.submit_limit_order({OrderType::Limit, Side::Buy, "TSLA", 199.5, 40});

    const auto trades = book.submit_market_order({OrderType::Market, Side::Sell, "TSLA", 0.0, 80});

    ASSERT_EQ(trades.size(), 2u);
    EXPECT_DOUBLE_EQ(trades[0].price, 200.0);
    EXPECT_EQ(trades[0].quantity, 60);
    EXPECT_DOUBLE_EQ(trades[1].price, 199.5);
    EXPECT_EQ(trades[1].quantity, 20);
    EXPECT_EQ(total_quantity(book.bids()), 20);
}
