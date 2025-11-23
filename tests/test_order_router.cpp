#include <gtest/gtest.h>

#include "book_test_helpers.hpp"
#include "engine/order_router.hpp"

TEST(OrderRouterTest, CreatesBooksAndRoutesByType) {
    OrderRouter router;
    const auto no_trade = router.submit({OrderType::Limit, Side::Buy, "IBM", 140.0, 10});
    EXPECT_TRUE(no_trade.empty());
    EXPECT_EQ(router.books().size(), 1u);

    const auto trades = router.submit({OrderType::Market, Side::Sell, "IBM", 0.0, 5});
    ASSERT_EQ(trades.size(), 1u);
    EXPECT_DOUBLE_EQ(trades[0].price, 140.0);
    EXPECT_EQ(trades[0].quantity, 5);
}

TEST(OrderRouterTest, KeepsBooksIsolatedBySymbol) {
    OrderRouter router;
    router.submit({OrderType::Limit, Side::Buy, "AAA", 10.0, 10});
    router.submit({OrderType::Limit, Side::Sell, "BBB", 20.0, 5});

    const auto trades = router.submit({OrderType::Market, Side::Sell, "AAA", 0.0, 5});

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(router.books().size(), 2u);
    EXPECT_EQ(total_quantity(router.book_for("BBB").asks()), 5);
}

TEST(OrderRouterTest, CreatesBookForMarketOrdersWithoutLiquidity) {
    OrderRouter router;
    const auto trades = router.submit({OrderType::Market, Side::Buy, "NEW", 0.0, 10});
    EXPECT_TRUE(trades.empty());
    EXPECT_EQ(router.books().size(), 1u);
}
