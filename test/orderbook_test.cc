#include "../OrderMatcher/central_order_book.hh"

// #include "gmock/gmock-matchers.h"
#include <gtest/gtest.h>

// using testing::Matches;

// Demonstrate some basic assertions.
TEST(OrderBook, AddSymbols) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
}

TEST(OrderBook, AddSymbolsDuplicate) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
  EXPECT_EQ(StatusCode::SYMBOL_EXISTS, book.add_symbol(s));
}

// add 2 buy - check besk bid
TEST(OrderBook, AddBuyLimitOrders) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
  Order buy1(1,2,1000,15,OrderSide::BUY,OrderType::LIMIT,0);
  Order buy2(2,2,800,5,OrderSide::BUY,OrderType::LIMIT,0);
  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy1));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy2));

  std::pair<StatusCode, unsigned> best_bid = book.best_bid(s);
  EXPECT_EQ(StatusCode::OK, best_bid.first);
  EXPECT_EQ(1000, best_bid.second);
}

// add 2 sell - check best ask
TEST(OrderBook, AddSellLimitOrders) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
  Order sell1(1,2,1000,15,OrderSide::SELL,OrderType::LIMIT,0);
  Order sell2(2,2,800,5,OrderSide::SELL,OrderType::LIMIT,0);
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell1));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell2));

  std::pair<StatusCode, unsigned> best_ask = book.best_ask(s);
  EXPECT_EQ(StatusCode::OK, best_ask.first);
  EXPECT_EQ(800, best_ask.second);
}

TEST(OrderBook, MatchLimitOrdersBasic) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
  Order buy1(1,2,1000,15,OrderSide::BUY,OrderType::LIMIT,0);
  Order buy2(2,2,999,5,OrderSide::BUY,OrderType::LIMIT,0);
  Order sell1(3,2,999,10,OrderSide::SELL,OrderType::LIMIT,0);
  Order sell2(4,2,998,5,OrderSide::SELL,OrderType::LIMIT,0);

  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy1));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy2));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell1));
  
  //sell1 should have matched with buy1 - hence sell1 should be deleted from order book
  auto sell_order_obj = book.get_order(s,3);
  EXPECT_FALSE(sell_order_obj);

  //buy1 should be partially filled with 5 qty still remaining
  auto buy_order_obj = book.get_order(s,1);
  EXPECT_FALSE(!buy_order_obj);
  Order order = *buy_order_obj; 
  EXPECT_EQ(buy1.get_id(), order.get_id());
  EXPECT_EQ(5, order.get_quantity());

}

TEST(OrderBook, MatchLimitOrdersAcrossLevels) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
  Order buy1(1,2,1000,15,OrderSide::BUY,OrderType::LIMIT,0);
  Order buy2(2,2,999,10,OrderSide::BUY,OrderType::LIMIT,0);
  Order sell1(3,2,999,20,OrderSide::SELL,OrderType::LIMIT,0);
  Order sell2(4,2,998,15,OrderSide::SELL,OrderType::LIMIT,0);

  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy1));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy2));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell1));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell2));
  
  book.printBuySellPool(s);
  //sell1 should have matched with buy1 and buy2
  //sell2 should have matched with buy2

  //buy2 should be partially filled with 10 qty still remaining
  auto sell_order_obj = book.get_order(s,4);
  EXPECT_FALSE(!sell_order_obj);
  Order order = *sell_order_obj; 
  EXPECT_EQ(sell2.get_id(), order.get_id());
  EXPECT_EQ(10, order.get_quantity());
}

TEST(OrderBook, MatchMarketOrders) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
  Order buy1(1,2,1000,5,OrderSide::BUY,OrderType::LIMIT,0);
  Order buy2(2,2,1000,5,OrderSide::BUY,OrderType::LIMIT,0);
  Order buy3(3,2,1000,10,OrderSide::BUY,OrderType::LIMIT,0);
  Order sell1(4,2,0,15,OrderSide::SELL,OrderType::MARKET,0);

  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy1));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy2));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy3));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell1));
  
  book.printBuySellPool(s);
  //sell1 should be matched with buy1, buy2, and buy3.
  //buy3 would be partially filled
  std::pair<StatusCode, unsigned> best_bid = book.best_bid(s);
  EXPECT_EQ(StatusCode::OK, best_bid.first);
  EXPECT_EQ(1000, best_bid.second);

  auto buy_order_obj = book.get_order(s,3);
  EXPECT_FALSE(!buy_order_obj);
  Order order = *buy_order_obj; 
  EXPECT_EQ(buy3.get_id(), order.get_id());
  EXPECT_EQ(5, order.get_quantity());
  std::cout << buy3;
  

}

TEST(OrderBook, AddStopOrder) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
  Order buy1(1,2,702,15,OrderSide::BUY,OrderType::LIMIT,0);
  Order stop_sell1(9,2,700,700,15,OrderSide::SELL,OrderType::STOP_LIMIT,0);

  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy1)); //B:702
  EXPECT_EQ(StatusCode::OK, book.add_order(s, stop_sell1));
  
  //stop order won't be executed yet, verify that
  auto sell_order_obj = book.get_order(s,9);
  EXPECT_FALSE(!sell_order_obj);
  Order order = *sell_order_obj; 
  EXPECT_EQ(OrderType::STOP_LIMIT, order.get_type());

  Order buy2(3,2,700,5,OrderSide::BUY,OrderType::LIMIT,0);
  Order sell1(5,2,701,15,OrderSide::SELL,OrderType::LIMIT,0);
  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy2)); //B:702,700
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell1));//s:701

  // book.printBuySellPool(s);
  //buy1 and sell1 matched - stop order won't be executed yet
  sell_order_obj = book.get_order(s,9);
  EXPECT_FALSE(!sell_order_obj);
  order = *sell_order_obj; 
  EXPECT_EQ(OrderType::STOP_LIMIT, order.get_type());

  Order sell2(6,2,699,5,OrderSide::SELL,OrderType::LIMIT,0);
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell2));
  // book.printBuySellPool(s);
  //buy2 and sell2 matched - stop limit order will be converted to limit order now
  sell_order_obj = book.get_order(s,9);
  EXPECT_FALSE(!sell_order_obj);
  order = *sell_order_obj; 
  EXPECT_EQ(OrderType::LIMIT, order.get_type());

}

TEST(OrderBook, DeleteLimitOrder) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
  Order buy1(1,2,1000,15,OrderSide::BUY,OrderType::LIMIT,0);
  Order sell1(3,2,999,10,OrderSide::SELL,OrderType::LIMIT,0);

  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy1));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell1));

  EXPECT_EQ(StatusCode::OK, book.delete_order(1));
  EXPECT_EQ(StatusCode::OK, book.delete_order(3));
  
  //order should be deleted
  auto order_obj = book.get_order(s,1);
  EXPECT_FALSE(order_obj);

  order_obj = book.get_order(s,3);
  EXPECT_FALSE(order_obj);
}

TEST(OrderBook, DeleteStopOrder) {
  CentralOrderBook book;
  std::string s = "APPLE";
  
  EXPECT_EQ(StatusCode::OK, book.add_symbol(s));
  Order buy1(1,2,1000,1000,15,OrderSide::BUY,OrderType::STOP_LIMIT,0);
  Order sell1(3,2,999,999,10,OrderSide::SELL,OrderType::STOP_LIMIT,0);

  EXPECT_EQ(StatusCode::OK, book.add_order(s, buy1));
  EXPECT_EQ(StatusCode::OK, book.add_order(s, sell1));

  EXPECT_EQ(StatusCode::OK, book.delete_order(1));
  EXPECT_EQ(StatusCode::OK, book.delete_order(3));
  
  //order should be deleted
  auto order_obj = book.get_order(s,1);
  EXPECT_FALSE(order_obj);

  order_obj = book.get_order(s,3);
  EXPECT_FALSE(order_obj);
}

TEST(OrderBook, DeleteOrderNotExists) {
  CentralOrderBook book;
  std::string s = "APPLE";
  EXPECT_EQ(StatusCode::SYMBOL_NOT_EXISTS, book.delete_order(1));
}
