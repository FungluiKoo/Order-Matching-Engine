#ifndef ORDER_MATCHING_ENGINE_MESSAGE_H
#define ORDER_MATCHING_ENGINE_MESSAGE_H

#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "utils.h"

class Message {
private:
    char type = 0;
    id_type id = ID_DEFAULT;
    time_type timestamp = 0;

    side_type side = SIDE_DEFAULT;
    price_type price = PRICE_DEFAULT;
    size_type remSize = SIZE_DEFAULT;
    size_type cancSize = SIZE_DEFAULT;
    size_type execSize = SIZE_DEFAULT;

    id_type oldId = ID_DEFAULT;
    price_type oldPrice = PRICE_DEFAULT;
    size_type oldSize = SIZE_DEFAULT;
    char mpid[5] = "";
    std::string ticker;

public:
    Message() = default;
    Message(const char& type,const id_type &id,
            const time_type &timestamp);

    /**
     * Setter for the message. Transforms the Nasdaq type definitions in ours.
     *
     * - NASDAQ   --> Custom
     * - A,F      --> (A)dd
     * - D,X      --> (D)elete
     * - U        --> (R)eplace
     * - E        --> (E)xecution
     * - P        --> P, hidden execution
     * - C        --> C, execution at different price
     *
     * @param[in] _type type string: according to the definition of NASDAQ
     */
    void setType(const char &);
    void setId(const id_type &);
    void setTimeStamp(const time_type &);
    void setSide(const side_type &);
    void setPrice(const price_type &);
    void setRemSize(const size_type &);
    void setCancSize(const size_type &);
    void setMPID(const char &);
    void setTicker(const std::string &);

    char getType() const;
    id_type getId() const;
    time_type getTimeStamp() const;
    side_type getSide() const;
    price_type getPrice() const;
    size_type getRemSize() const;
    std::string getTicker() const;


    bool isEmpty() const;

    void print() const;
};


#endif //ORDER_MATCHING_ENGINE_MESSAGE_H
