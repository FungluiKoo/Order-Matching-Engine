#include <algorithm>

#include "orderbook.hh"

// private:
void OrderBook::match_order(Order& order){ // assume limit order
    bool isbuy = order.get_side()==OrderSide::BUY;
    if(order.isAON()){
        auto qty = order.get_quantity();
        decltype(qty) fulfillment = 0;
        auto quote = order.get_quote();
        auto iter = isbuy ? sellprices.begin() : buyprices.begin();
        auto iterend = isbuy ? std::upper_bound(sellprices.begin(), sellprices.end(), quote) : std::upper_bound(buyprices.begin(), buyprices.end(), quote, std::greater<unsigned>());
        for(; qty<fulfillment && iter!=iterend; ++iter){
            auto &nowlist = isbuy ? sellpool[*iter] : buypool[*iter];
            for(auto &noworder : nowlist){
                fulfillment += noworder.get_quantity();
            }
        }
        if(qty < fulfillment){return;}
    }
    while(!(isbuy ? sellprices.empty() : buyprices.empty())){
        auto level = isbuy ? best_ask() : best_bid();
        if(isbuy ? order.get_quote() < level : order.get_quote() > level){
            return;
        }
        auto &nowlist = isbuy ? sellpool[level] : buypool[level];
        while(!nowlist.empty()){
            auto &noworder = nowlist.front();
            auto quantity = std::min(noworder.get_quantity(), order.get_quantity());
            if(noworder.isAON() && noworder.get_quantity() > order.get_quantity()){
                return;
            }
            
            // execute the order
            ostrm << (isbuy?order.get_id():noworder.get_id()) << ";" <<
                (isbuy?noworder.get_id():order.get_id()) << ";" <<
                level << ";" << quantity << "\n";
            // result.push_back(Transaction(isbuy?order.get_id():noworder.get_id(), isbuy?noworder.get_id():order.get_id(), level, quantity));
            noworder.reduce_quantity(quantity);
            //update matching price
            set_last_matching_price(noworder, level);
            if(noworder.get_quantity()==0){
                order_map.erase(noworder.get_id());
                nowlist.pop_front();
                if(nowlist.empty()){
                    if(isbuy){
                        sellpool.erase(level);
                        sellprices.erase(level);
                    }else{
                        buypool.erase(level);
                        buyprices.erase(level);
                    }
                }
            }
            order.reduce_quantity(quantity);
            if(order.get_quantity()==0){
                // the caller needs to delete this entry from pool similar to noworder
                return;
            }
        }
    }
    return;
}


void OrderBook::match(Order& order, bool isMarket){
    // Made minor change of passing boolean to prevent repeating same code in multiple places
    auto isbuy = order.get_side()==OrderSide::BUY;
    // if(isbuy){sellsem.acquire();}
    // else{buysem.acquire();}

    if(isMarket){
        if(isbuy){
            order.set_quote(best_ask());
        }else{
            order.set_quote(best_bid());
        }
    }
    match_order(order);
    // if(isbuy){sellsem.release();}
    // else{buysem.release();}
}