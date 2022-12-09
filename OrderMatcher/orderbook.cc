#include <iterator>

#include "orderbook.hh"

// private methods:

/*
    Update last buy/sell price after a match.
*/
void OrderBook::set_last_matching_price(Order& order, unsigned price){
    if(order.isBuy()){
        last_buy_price = price;
    }else{
        last_sell_price = price;
    }
}

/*
    Get last buy matching price.
*/
unsigned OrderBook::get_buy_market_price() const{
    unsigned best_buy_price = buyprices.empty() ? 0 : *(buyprices.begin());
    return std::max(best_buy_price,last_buy_price);
}

/*
    Get last sell matching price.
*/
unsigned OrderBook::get_sell_market_price() const{ // ?
    unsigned best_sell_price = sellprices.empty() ? std::numeric_limits<unsigned>::max() : *(sellprices.begin());
    return std::min(best_sell_price,last_sell_price);
}

/*
    Add an order 'order' at price 'level' to level pool 'prices'
    and order pool 'pool'
*/
template<typename Comp>
StatusCode OrderBook::add_to_orderbook(Order& order, unsigned level, std::set<unsigned, Comp>& prices, std::unordered_map<unsigned, std::list<Order>>& pool){
    OrderSide side = order.get_side();
    if (pool.find(level) == pool.end()){ //price level not found
            prices.insert(level);
    }
    //add to relevant pool
    pool[level].push_back(order);
    //add to order map
    OrderInfo info = {side, level, order.get_type()};
    order_map.insert_or_assign(order.get_id(), info);
    return StatusCode :: OK;
}

/*
    Execute/Activate all stop orders.
*/
void OrderBook::execute_stop_orders(){
    std::cout << "Executing stop orders\n";
    //if stop price at a level <= market price, stop order is activated
    auto buy_pred = [](unsigned stop_price, unsigned sell_market_price) {
     return stop_price <= sell_market_price;
    };
    
    //execute stop buy orders if possible
    execute_stop_orders(get_sell_market_price(), stop_buy_prices, stop_buy_pool, buy_pred);

    //if stop price at a level >= market price, stop order is activated
    auto sell_pred = [](unsigned stop_price, unsigned buy_market_price) {
     return stop_price >= buy_market_price;
    };
    //execute stop sell orders if possible
    execute_stop_orders(get_buy_market_price(), stop_sell_prices, stop_sell_pool, sell_pred);
}

/*
    Execute stop orders present in 'order_pool' with price levels 'prices'
    based on predicate 'p'
*/
template<typename Pred, typename Comp>
void OrderBook::execute_stop_orders(unsigned stop_price, std::set<unsigned,Comp>& prices,
                                    std::unordered_map<unsigned, std::list<Order>>& order_pool, Pred p){
    //For every stop price satisfying predicate, delete from stop pool and activate it
     for (auto f = prices.begin(); f != prices.end();) {
        std::cout << "In loop to Executing stop orders at level " << *f <<" stop price " <<stop_price; 
        if(!p(*f, stop_price))
            break;
        std::cout << "Executing stop orders at level " << *f << "\n";
        std::list<Order> orders = order_pool[*f];
        f = prices.erase(f);
        order_pool.erase(*f);
        //iterare through all orders at a price level
        for(auto order : orders){
            execute_stop_order(order, order.get_type() == OrderType::STOP_LIMIT);
        }
        //std::cout << "End of Executing stop orders at level " << *f << "\n";
     }
}

/*
    Execute a particular stop order 'order'.
    Convert it to Limit/Market order.
*/
void OrderBook::execute_stop_order(Order& order, bool is_limit){
    if(is_limit){
        order.set_type(OrderType::LIMIT);
        std::cout << "setting to LO \n" << order;
    }else{
        // std::cout << "setting to MO \n" << order;
        order.set_type(OrderType::MARKET);
        order.set_quote(0);
    }
    match(order, !is_limit);
    std::cout << "Order qty " << order.get_quantity();
    if (order.get_quantity() > 0  && !order.isAON()){
        if(order.isBuy()){
            //buysem.acquire();
            add_to_orderbook(order, order.get_quote(), buyprices, buypool);
            //buysem.release();
        }else{
            //sellsem.acquire();
            add_to_orderbook(order, order.get_quote(), sellprices, sellpool);
            //sellsem.release();
        }
    }
}

/*
    Add stop order to the order book.
    Try to execute immediately if possible, else add it to the pool.
*/
StatusCode OrderBook::add_stop_order(Order& order, bool is_limit){
    bool can_execute;
    unsigned stop_price = order.get_stop_price();
    if(order.isBuy()){
        auto market_price = get_sell_market_price();
        std::cout << "Buy Stop price = " << market_price;
        can_execute = stop_price <= market_price;
    }else{
        auto market_price = get_buy_market_price();
        std::cout << "Sell Stop price = " << market_price;
        can_execute = stop_price >= market_price;
    }
    if(can_execute){
        std::cout << "Can execute stop order now, stop price = " << stop_price;
        //execute stop order fn - set type to MO/LO - if remaining need to add to orderbook
        execute_stop_order(order, is_limit);
    }else{
        std::cout << "Cannot execute stop order now, adding to orderbook \n";
        if(order.isBuy()){
            add_to_orderbook(order, stop_price, stop_buy_prices, stop_buy_pool);
        }else{
            add_to_orderbook(order, stop_price, stop_sell_prices, stop_sell_pool);
        }
    }
    return StatusCode::OK;
}

/*
    Fetch the OrderInfo from the order_map given order_id.
*/
std::optional<OrderInfo> OrderBook::get_order_info(unsigned int order_id){
    auto order = order_map.find(order_id);
    if (order == order_map.end()){ 
        return {};
    }
    return order->second;
}

/*
    Delete an order with ID 'order_id' at price 'price' from the level pool 'prices'
    and order pool 'pool'
*/
template<typename Comp>
void OrderBook::delete_order(unsigned order_id, unsigned price, std::set<unsigned, Comp>& prices, 
                            std::unordered_map<unsigned, std::list<Order>>& pool){
    for (auto it = pool[price].begin(); it != pool[price].end(); ++it) {
        if(it->get_id() == order_id){
            pool[price].erase(it);
            if(pool[price].empty()){ 
                // std::cout << "Del from pool \n";
                pool.erase(price);
                prices.erase(price);  
            }
            break;
        }
    }
}


// public:

/*
    Add an order to the order book.
*/
StatusCode OrderBook::add_order(Order& order){
    std::cout << "In add order \n" << order;
    unsigned order_id = order.get_id();
    if(order_map.count(order_id) != 0){
        return StatusCode :: ORDER_EXISTS;
    }
    StatusCode status = StatusCode :: OK;
    OrderType type = order.get_type();
    if(type == OrderType :: MARKET || type == OrderType :: LIMIT ){
        match(order, type == OrderType::MARKET);
        if (order.get_quantity() > 0 && !order.isAON()){
            std::cout << "Adding to book" << order;
            if(order.isBuy()){
                //buysem.acquire();
                add_to_orderbook(order, order.get_quote(), buyprices, buypool);
                //buysem.release();
            }else{
                //sellsem.acquire();
                add_to_orderbook(order, order.get_quote(), sellprices, sellpool);
                //sellsem.release();
            }
        }
        execute_stop_orders();  
    } else if(type == OrderType :: STOP){
        status = add_stop_order(order,false);
    } else{
        status = add_stop_order(order,true);
    }
    return status; 
}

/*
    Fetch an order with ID 'order_id'
*/
std::optional<Order> OrderBook::get_order(unsigned int order_id){
    //Fetch order info
    auto order_details = get_order_info(order_id);
    if (!order_details){
        return {};
    }
    auto order_info = *order_details;
    bool isbuy = order_info.side==OrderSide::BUY;
    bool isStop = (order_info.type == OrderType::STOP) || (order_info.type == OrderType::STOP_LIMIT);
    unsigned price = order_info.price;
    //Identify relevant pool to fetch order from
    std::list<Order>& order_list = (isbuy) ? ((isStop)? stop_buy_pool[price] : buypool[price]) : 
                                            ((isStop)? stop_sell_pool[price] : sellpool[price]);
    for (auto it = order_list.begin(); it != order_list.end(); ++it) {
        if(it->get_id() == order_id){
            return *it;
        }
    }
    return {};
}

/*
    Delete an order with id 'order_id'
*/
StatusCode OrderBook::delete_order(unsigned int order_id){
    auto order_details = get_order_info(order_id);
    if (!order_details){
        return StatusCode :: ORDER_NOT_EXISTS;
    }
    auto order_info = *order_details;
    order_map.erase(order_id);
    bool isBuy = order_info.side==OrderSide::BUY;
    bool isStop = (order_info.type == OrderType::STOP) || (order_info.type == OrderType::STOP_LIMIT);
    if(isBuy){
        if(isStop){
            //stop//buysem.acquire();
            delete_order(order_id, order_info.price, stop_buy_prices, stop_buy_pool);
            //stop//buysem.release();
        }else{
            //buysem.acquire();
            delete_order(order_id, order_info.price, buyprices, buypool);
            //buysem.release();
        }
    }else{
        if(isStop){
            //stop//sellsem.acquire();
            delete_order(order_id, order_info.price, stop_sell_prices, stop_sell_pool);
            //stop//sellsem.release();
        }else{
            //sellsem.acquire();
            delete_order(order_id, order_info.price, sellprices, sellpool);
            //sellsem.release();
        }
    }
    return StatusCode :: OK;  
}

/*
 For debugging
*/
void OrderBook::printBuySellPool()const{
    std::cout << "BuyPrices are:" << "\n";
    for (auto const& price : buyprices)
    {
        std::cout << price << ' ';
    }
    std::cout << "\nBuyPool" << "\n";
    for (auto const &pair: buypool) {
        std::cout << "{" << pair.first << "}\n";
        for (auto it = pair.second.begin(); it != pair.second.end(); ++it)
            std::cout << ' ' << *it <<"}\n";
    }
    std::cout << "SellPrices are:" << "\n";
    for (auto const& price : sellprices)
    {
        std::cout << price << ' ';
    }
    std::cout << "\nSellPool" << "\n";
    for (auto const &pair: sellpool) {
        std::cout << "{" << pair.first << "}\n";
        for (auto it = pair.second.begin(); it != pair.second.end(); ++it)
            std::cout << ' ' << *it << "}\n";
    }
}

