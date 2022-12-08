#pragma once

#include <chrono>
#include <iostream>

enum class OrderSide : unsigned char {
    BUY,
    SELL,
    SELL_SHORT,
    SELL_SHORT_EXEMPT,
    CROSS,
    CROSS_SHORT,
    CROSS_SHORT_EXEMPT
};

enum class OrderType : unsigned char {
    LIMIT,
    MARKET,
    STOP, // a.k.a. STOPLOSS
    STOP_LIMIT // a.k.a. STOPLOSS_LIMIT
};

/*
    The Order class.
*/
class Order{
private:
    unsigned order_id;
    unsigned owner_id;
    unsigned quantity; 
    unsigned quote;
    unsigned stop_price;
    OrderSide order_side;
    OrderType order_type;
    char all_or_none; // aon=1, partial order allowed=0
    std::chrono::time_point<std::chrono::system_clock> timestamp;
public:
    Order(unsigned order, unsigned owner, unsigned quote, unsigned stop_price,unsigned qty, OrderSide sd, OrderType tp, char aon = 0, std::chrono::time_point<std::chrono::system_clock> tmstmp = std::chrono::system_clock::now()):
        order_id(order),
        owner_id(owner),
        quote(quote),
        stop_price(stop_price),
        quantity(qty),
        order_side(sd),
        order_type(tp),
        all_or_none(aon),
        timestamp(tmstmp){}
    Order(unsigned order, unsigned owner, unsigned quote, unsigned qty, OrderSide sd, OrderType tp, char aon = 0, std::chrono::time_point<std::chrono::system_clock> tmstmp = std::chrono::system_clock::now()):
        Order(order,owner,quote,0,qty,sd,tp,aon,tmstmp)
        {}
    unsigned get_id()const{return order_id;}
    unsigned get_owner()const{return owner_id;}
    unsigned get_quantity()const{return quantity;}
    void reduce_quantity(unsigned x){quantity-=x;} // only if aon=0
    unsigned get_quote()const{return quote;}
    void set_quote(unsigned x){quote=x;} // only for market orders
    unsigned get_stop_price()const{return stop_price;}
    OrderSide get_side()const{return order_side;}
    OrderType get_type()const{return order_type;}
    void set_type(OrderType type){order_type = type;}
    char isAON()const{return all_or_none;}
    bool isBuy()const{return order_side == OrderSide::BUY;}
    std::chrono::time_point<std::chrono::system_clock> get_time()const{return timestamp;}
    friend std::ostream& operator<<(std::ostream &s, const Order &order);
};

// class Transaction{
// public:
//     unsigned buyside_order_id;
//     unsigned sellside_order_id;
//     unsigned final_price; // price at which a transaction was done
//     unsigned quantity; // In case of partial order
//     std::chrono::time_point<std::chrono::system_clock> timestamp; // when the transaction was completed; -- not sure if useful
//     Transaction(unsigned buyer_order_id, unsigned seller_order_id, unsigned price, unsigned quantity):
//         buyside_order_id(buyer_order_id),
//         sellside_order_id(seller_order_id),
//         final_price(price),
//         quantity(quantity),
//         timestamp(std::chrono::system_clock::now()){}
// };
