//
// Created by jinzh on 2022/12/6.
//

#include "message.h"

Message::Message(const char& _type,
                 const id_type& _id,
                 const time_type& _timestamp):
        type(_type),
        id(_id),
        timestamp(_timestamp){};

void Message::setType(const char& _type){
    if(_type =='A' || _type == 'F'){
        type = 'A'; // (A)dd
    }
    else if (_type == 'D' || _type == 'X'){
        type = 'D'; // (D)elete
    }
    else if (_type == 'U'){
        type = 'R'; // (R)eplace
    }
    else if (_type == 'E'){
        type = _type; // (E)xecute
    }
    else if (_type == 'P'){
        type = _type; // execute hidden message
    }
    else if (_type == 'C'){
        type = _type;
    }
    else{
        std::cerr << "Message with wrong type (" << _type << ") !"<< std::endl;
    }
}

void Message::setId(const id_type& _id){
    id = _id;
}

void Message::setTimeStamp(const time_type& _timestamp){
    timestamp = _timestamp;
}

void Message::setSide(const side_type& _side){
    side = _side;
}

void Message::setPrice(const price_type& _price){
    price = _price;
}

void Message::setRemSize(const size_type& _size){
    remSize = _size;
}

void Message::setCancSize(const size_type& _size){
    cancSize = _size;
}



void Message::setMPID(const char& _mpid){
    strncpy(mpid, &_mpid, 4); mpid[4] = 0;
}

void Message::setTicker(const std::string & _ticker) {
    ticker = _ticker;
    // remove the whitespaces
    ticker.erase(std::remove_if(ticker.begin(), ticker.end(), ::isspace),ticker.end());
}



// getters
char Message::getType() const{
    return type;
}

id_type Message::getId() const{
    return id;
}

time_type Message::getTimeStamp()const{
    return timestamp;
}

side_type Message::getSide()const{
    return side;
}

price_type Message::getPrice()const{
    return price;
}

size_type Message::getRemSize()const{
    return remSize;
}


std::string Message::getTicker() const {
    return ticker;
}


bool Message::isEmpty()const{
    return (id==ID_DEFAULT);
}


void Message::print() const {
    std::cout << "Message type   :" << type <<std::endl;
    std::cout << "Id             :"<< id << std::endl;
    std::cout << "timestamp      :" << timestamp << std::endl;
    std::cout << "Side           :" << side << std::endl;
    std::cout << "Price          :"<< price << std::endl;
    std::cout << "Remaining size :" << remSize << std::endl;
    if (type == 'D'){
        std::cout << "Deletion size  :" << cancSize << std::endl;
    }
    if (type == 'E'){
        std::cout << "Execution size :" << execSize << std::endl;
    }
    if (type == 'U'){
        std::cout << "Old Id         :" << oldId << std::endl;
        std::cout << "Old size       :"<< oldSize << std::endl;
        std::cout << "Old price      :" << oldPrice << std::endl;
    }
    if (type == 'F'){
        std::cout << "MPID :" << mpid << std::endl;
    }
    std::cout << " " << std::endl;
}