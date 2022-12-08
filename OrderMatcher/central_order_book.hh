#include <memory>

#include "orderbook.hh"

/*
    The main Order Book class that maintains the order book for different
    stocks. It provides operations on the order book.
*/
class CentralOrderBook {
    private:
        // map of stock symbol to its order book
        std::unordered_map<std::string, std::unique_ptr<OrderBook>> order_book_map;
        // store a hash map of orderID to symbols
        std::unordered_map<unsigned int, std::string> order_ticket_map;
public:
        
    public:
        StatusCode add_symbol(std::string);
        
        StatusCode add_order(std::string, Order&);
        
        StatusCode delete_order(unsigned int);

        std::optional<Order> get_order(std::string, unsigned int);

        std::pair<StatusCode, unsigned> best_ask(std::string) const;

        std::pair<StatusCode, unsigned> best_bid(std::string) const;

        void printBuySellPool(std::string) const;
};