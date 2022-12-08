#include "reader.h"

Reader::Reader(std::string _fileName) :
        fileName(std::move(_fileName)) {
    file.open(fileName);
    if (!file.is_open()) {
        std::cerr << "The input file: " << fileName << " cannot be open! " << std::endl;
    } else {
        std::cout << "Opened " << fileName << " to read ITCH 5.0. messages." << std::endl;
        validFile = true;
    }
    start = time(0);
}


void Reader::printProgress(){
    count ++;
    if(count % 10000000 == 0) {
        std::cout << "Processed " << count / 1000000 << "Million messages. "
                  << int(count / difftime(time(0), start) / 10000) / 100. << " Mio messages per sec." << std::endl;
    }
}

void Reader::readBytesIntoMessage(const long &size){
    file.read(message, size);
}

void Reader::skipBytes(const long &size){
    file.ignore(size);
}

char Reader::getKey(){
    char key;
    file.get(key);
    return key;
}

Message Reader::createMessage(){
    printProgress();
    Message msg;
    skipBytes(2);
    char key = getKey();
//    std::cout << "key: " << key << "   Pos: "<<  this->file.tellg() << std::endl;
    char ticker[9];
    strncpy(ticker, stock.c_str(), 8); ticker[8] = 0;
    int debug = 0;

    switch(key){
        uint64_t timeStamp, orderId, oldOrderId, newOrderId;
        uint32_t size, price, execSize, cancSize, newSize, newPrice;

        // debug
        uint64_t locateCode, trackingNumb;
        char eventCode;
        char direction;
        char mpid[5];


        case 'S':
            readBytesIntoMessage(11);
            break;
        case 'R':
            readBytesIntoMessage(38);
            break;
        case 'H':
            readBytesIntoMessage(24);
            break;
        case 'T':
            readBytesIntoMessage(19);
            break;
        case 'z':
            readBytesIntoMessage(19);
            break;
        case 'Y':
            readBytesIntoMessage(19);
            break;
        case 'L':
            readBytesIntoMessage(25);
            break;
        case 'V':
            readBytesIntoMessage(34);
            break;
        case 'W':
            readBytesIntoMessage(11);
            break;
        case 'K':
            readBytesIntoMessage(27);
            break;
        case 'J':
            readBytesIntoMessage(34);
            break;
        case 'h':
            readBytesIntoMessage(20);
            break;
        case 'A':
            readBytesIntoMessage(35);
            timeStamp = parse_ts(message+4);
            orderId = parse_uint64(message+10);
            direction = message[18];
            size = parse_uint32(message+19);
            strncpy(ticker, message+23, 8); ticker[8] = 0;
            price = parse_uint32(message+31);
            msg.setType(key);
            msg.setTimeStamp(static_cast<time_type>(timeStamp));
            msg.setId(static_cast<id_type>(orderId));
            msg.setSide(static_cast<side_type>(direction == 'S'));
            msg.setRemSize(static_cast<size_type>(size));
            msg.setPrice(static_cast<price_type>(price));
            msg.setTicker(std::string(ticker));
            if (debug == 1)
            {
                std::cout << "ticker:       " << ticker << std::endl;
                msg.print();
            }
            break;
        case 'F':
            readBytesIntoMessage(39);
            timeStamp = parse_ts(message+4);
            orderId = parse_uint64(message+10);
            direction = message[18];
            size = parse_uint32(message+19);
            strncpy(ticker, message+23, 8); ticker[8] = 0;
            price = parse_uint32(message+31);
            strncpy(mpid, message+35, 4); mpid[4] = 0;
            msg.setType(key);
            msg.setTimeStamp(static_cast<time_type>(timeStamp));
            msg.setId(static_cast<id_type>(orderId));
            msg.setSide(static_cast<side_type>(direction == 'S'));
            msg.setRemSize(static_cast<size_type>(size));
            msg.setPrice(static_cast<price_type>(price));
            msg.setMPID(*mpid);
            msg.setTicker(std::string(ticker));
            if (debug == 1)
            {
                std::cout << "ticker:       " << ticker << std::endl;
                msg.print();
            }
            break;
        case 'E':
            readBytesIntoMessage(30);
            break;
        case 'C':
            readBytesIntoMessage(35);
            break;
        case 'X':
            readBytesIntoMessage(22);
            timeStamp = parse_ts(message+4);
            orderId = parse_uint64(message+10);
            cancSize = parse_uint32(message+18);
            msg.setType(key);
            msg.setTimeStamp(static_cast<time_type>(timeStamp));
            msg.setId(static_cast<id_type>(orderId));
            msg.setCancSize(static_cast<size_type>(cancSize));
            if (debug == 1)
                msg.print();
            break;
        case 'D':
            readBytesIntoMessage(18);
            timeStamp = parse_ts(message+4);
            orderId = parse_uint64(message+10);
            msg.setType(key);
            msg.setTimeStamp(static_cast<time_type>(timeStamp));
            msg.setId(static_cast<id_type>(orderId));
            if (debug == 1)
                msg.print();
            break;
        case 'U':
            readBytesIntoMessage(34);
//            timeStamp = parse_ts(message+4);
//            oldOrderId = parse_uint64(message+10);
//            newOrderId = parse_uint64(message+18);
//            newSize = parse_uint32(message+26);
//            newPrice = parse_uint32(message+30);
//            msg.setType(key);
//            msg.setTimeStamp(static_cast<time_type>(timeStamp));
//            msg.setId(static_cast<id_type>(newOrderId));
//            msg.setOldId(static_cast<id_type>(oldOrderId));
//            msg.setRemSize(static_cast<size_type>(newSize));
//            msg.setPrice(static_cast<price_type>(newPrice));
            break;
        case 'P':
            readBytesIntoMessage(43);
//            timeStamp = parse_ts(message+4);
//            orderId = parse_uint64(message+10);
//            direction = message[18];
//            size = parse_uint32(message+19);
//            strncpy(ticker, message+23, 8); ticker[8] = 0;
//            price = parse_uint32(message+31);
//            msg.setType(key);
//            msg.setTimeStamp(static_cast<time_type>(timeStamp));
//            msg.setId(static_cast<id_type>(orderId));
//            msg.setSide(static_cast<side_type>(direction == 'S'));
//            msg.setExecSize(static_cast<size_type>(size));
//            msg.setPrice(static_cast<price_type>(price));
            break;
        case 'Q':
            readBytesIntoMessage(39);
            break;
        case 'B':
            readBytesIntoMessage(18);
            break;
        case 'I':
            readBytesIntoMessage(49);
            break;
        case 'N':
            readBytesIntoMessage(19);
            break;
        default:
            if (!eof()) {
                std::cerr << "Type " << key << " not found: abort" << std::endl;
                validFile = false;
            }
            break;
    }

//    if (strcmp(ticker, stock.c_str()) != 0) {
//        return {};
//    }
    return msg;
}

bool Reader::eof(){
    return file.eof();
}

bool Reader::isValid() const{
    return validFile;
}

std::string Reader::getFileName() const{
    return fileName;
}

std::string Reader::getStock() const{
    return stock;
}

void Reader::setMessage(const char* str){
    memcpy(message, str, 64);
}

Reader::~Reader(){
    if (file.is_open()){
        file.close();
        std::cout << "File " << fileName << " has been closed" << std::endl;
        std::cout << "Finished, processed " << count << " messages in " << difftime(time(0),start) << "seconds."  << std::endl;
    }
}
