#include "central_order_book.hh"

/*
    Add a stock symbol 'symbol' to the Central Order Book.
*/
StatusCode CentralOrderBook::add_symbol(std::string symbol){
    StatusCode status;
    if (order_book_map.count(symbol) != 0){
        status = StatusCode :: SYMBOL_EXISTS;
    } else{
        order_book_map[symbol] = std::make_unique<OrderBook>();
        status = StatusCode :: OK;
    }
    return status;
}

/*
    Adds an order of a particular symbol to the order book. 
*/
StatusCode CentralOrderBook::add_order(std::string symbol, Order& order){
    StatusCode status;
    auto order_book_ptr = order_book_map.find(symbol);
    
    if (order_book_ptr == order_book_map.end()){
        status = StatusCode :: SYMBOL_NOT_EXISTS;
        // std::cout << "symbol not found";
    } else{
        // std::cout << "symbol found";
        status = order_book_ptr->second->add_order(order);
    }
    return status;
}

/*
    Fetch an order of a particular symbol and order ID from the order book. 
*/
std::optional<Order> CentralOrderBook::get_order(std::string symbol, unsigned int order_id){
    auto order_book_ptr = order_book_map.find(symbol);
    
    if (order_book_ptr == order_book_map.end()){
        return {};
    }
    return order_book_ptr->second->get_order(order_id);
}

/*
    Delete an order of a particular symbol and order ID from the order book. 
*/
StatusCode CentralOrderBook::delete_order(std::string symbol, unsigned int order_id){
    StatusCode status;
    auto order_book_ptr = order_book_map.find(symbol);
    if (order_book_ptr == order_book_map.end()){
        status = StatusCode :: SYMBOL_NOT_EXISTS;
    } else{
        order_book_ptr->second->delete_order(order_id);
        status = StatusCode :: OK;
    }
    return status;
}

/*
    Return the best ask/sell price of a symbol.
*/
std::pair<StatusCode, unsigned> CentralOrderBook::best_ask(std::string symbol) const{
    StatusCode status;
    unsigned price = std::numeric_limits<unsigned>::max();
    auto order_book_ptr = order_book_map.find(symbol);
    if (order_book_ptr == order_book_map.end()){
        status = StatusCode :: SYMBOL_NOT_EXISTS;
    } else{
        price = order_book_ptr->second->best_ask();
        status = StatusCode :: OK;
    }
    return std::make_pair(status, price);
}

/*
    Return the best bid/buy price of a symbol.
*/
std::pair<StatusCode, unsigned> CentralOrderBook::best_bid(std::string symbol) const{
    StatusCode status;
    unsigned price = 0;
    auto order_book_ptr = order_book_map.find(symbol);
    if (order_book_ptr == order_book_map.end()){
        status = StatusCode :: SYMBOL_NOT_EXISTS;
    } else{
        price = order_book_ptr->second->best_bid();
        status = StatusCode :: OK;
    }
    return std::make_pair(status, price);
}

// Print the order book contents - internally used for debugging
void CentralOrderBook::printBuySellPool(std::string symbol)const{
    order_book_map.at(symbol)->printBuySellPool();
}