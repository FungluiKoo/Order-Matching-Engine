#pragma once

#include <array>
#include <fstream>
#include <list>
#include <optional>
#include <set>
#include <semaphore>
#include <string>
#include <unordered_map>
#include <vector>

#include "order.hh"

enum StatusCode {
    OK,
    SYMBOL_EXISTS,
    SYMBOL_NOT_EXISTS,
    ORDER_EXISTS,
    ORDER_NOT_EXISTS

};

struct OrderInfo{
    OrderSide side;
    unsigned price;
    OrderType type;
};

/*
    The Order Book for a particular stock symbol.
*/
class OrderBook{

    private:
        unsigned last_buy_price = 0;
        unsigned last_sell_price = std::numeric_limits<unsigned>::max();
        std::string company; // also the filename for output
        std::ofstream ostrm;

        std::binary_semaphore buysem{0}, sellsem{0}, stopbuysem{0}, stopsellsem{0};
        // key=price level; value=a list of Order
        std::unordered_map<unsigned, std::list<Order>> buypool, sellpool, stop_buy_pool, stop_sell_pool; 
        // stores current levels of the hashmaps (sellpool and stop_buy_pool)
        std::set<unsigned, std::less<unsigned>> sellprices, stop_buy_prices;
        // stores current levels of the hashmaps (buypool and stop_sell_pool)
        std::set<unsigned, std::greater<unsigned>> buyprices, stop_sell_prices;
        // key=order ID, value=(orderside, price level, ordertype)
        std::unordered_map<unsigned, OrderInfo> order_map; 

        unsigned get_sell_market_price() const;
        unsigned get_buy_market_price() const;
        void execute_stop_orders();
        
        template<typename Pred, typename Comp>
        void execute_stop_orders(unsigned, std::set<unsigned, Comp>&, std::unordered_map<unsigned, std::list<Order>>&, Pred);
        
        void execute_stop_order(Order&, bool);
        void match_order(Order& order);
        void match(Order& order, bool isMarket);
        StatusCode add_stop_order(Order&, bool);
        std::optional<OrderInfo> get_order_info(unsigned int);
        
        template<typename Comp>
        StatusCode add_to_orderbook(Order& order, unsigned level, std::set<unsigned, Comp>& prices, std::unordered_map<unsigned, std::list<Order>>& pool);
        
        template<typename Comp>
        void delete_order(unsigned, unsigned, std::set<unsigned, Comp>& prices, std::unordered_map<unsigned, std::list<Order>>& pool);
        
        void set_last_matching_price(Order& order, unsigned price);

    public:
        OrderBook(std::string company = "MUDD") :
            company(company),
            ostrm(company, std::ios_base::app)
            {
                buysem.release();
                sellsem.release();
                stopbuysem.release();
                stopsellsem.release();
            }
        StatusCode add_order(Order&);
        std::optional<Order> get_order(unsigned int);
        StatusCode delete_order(unsigned int);
        unsigned best_ask()const{
            return sellprices.empty() ? std::numeric_limits<unsigned>::max() : *(sellprices.begin()); 
        }
        unsigned best_bid()const{
            return buyprices.empty() ? 0 : *(buyprices.begin());
        }
        void printBuySellPool()const;
};
