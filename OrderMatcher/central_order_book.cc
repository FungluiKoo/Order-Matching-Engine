#include "central_order_book.hh"

/*
    Add a stock symbol 'symbol' to the Central Order Book.
*/
StatusCode CentralOrderBook::add_symbol(std::string symbol){
    StatusCode status;
    if (order_book_map.count(symbol) != 0){
        status = StatusCode :: SYMBOL_EXISTS;
    } else{
        order_book_map[symbol] = OrderBook(symbol);
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

    // if not found then create an orderbook
    if (order_book_ptr == order_book_map.end()){
        // std::cout << "symbol not found";
        status = add_symbol(symbol);
    } else{
        // std::cout << "symbol found";
        status = (order_book_ptr->second).add_order(order);
    }
    if (status == StatusCode::OK)
    {
        order_ticket_map[order.get_id()] = symbol;
    }
    return status;
}

/*
    Fetch an order of a particular symbol and order ID from the order book. 
*/
std::optional<Order> CentralOrderBook::get_order(unsigned int order_id){
    auto order_ticket_ptr = order_ticket_map.find(order_id);
    if (order_ticket_ptr == order_ticket_map.end()){
        return {};
    }
    std::string symbol = order_ticket_ptr->second;
    auto order_book_ptr = order_book_map.find(symbol);
    return (order_book_ptr->second).get_order(order_id);
}

/*
    Delete an order of an order ID from the order book.
*/
StatusCode CentralOrderBook::delete_order(unsigned int order_id){
    StatusCode status;
    // first check the order ticket map
    auto order_ticket_ptr = order_ticket_map.find(order_id);
    if (order_ticket_ptr == order_ticket_map.end()){
        status = StatusCode :: ORDER_NOT_EXISTS;
    }
    else {
        std::string sym = order_ticket_ptr->second;
        // then go to the order book
        auto order_book_ptr = order_book_map.find(sym);
        // if (order_book_ptr == order_book_map.end()){
        //     status = StatusCode :: SYMBOL_NOT_EXISTS;
        // } else{
            status = (order_book_ptr->second).delete_order(order_id);
//            status = StatusCode :: OK;
        // }
        order_ticket_map.erase(order_id);
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
        price = (order_book_ptr->second).best_ask();
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
        price = (order_book_ptr->second).best_bid();
        status = StatusCode :: OK;
    }
    return std::make_pair(status, price);
}

// Print the order book contents - internally used for debugging
void CentralOrderBook::printBuySellPool(std::string symbol)const{
    order_book_map.at(symbol).printBuySellPool();
}