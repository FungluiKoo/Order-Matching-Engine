#ifndef ORDER_MATCHING_ENGINE_READER_H
#define ORDER_MATCHING_ENGINE_READER_H


#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <cinttypes>
#include <cstring>
#include "message.h"

class Reader{
private:
    std::string fileName;
    std::ifstream file;
//     std::string messageToFilter = "AFEXDU"; // QC
    std::string stock;
    unsigned count = 0;
    char message[64];
    bool validFile = false;
    time_t start;

public:

    Reader(std::string fileName);

    bool isValid() const;
    virtual ~Reader();

    Message createMessage();
    bool eof();
    void printProgress();
    virtual void readBytesIntoMessage(const long &);
    virtual void skipBytes(const long &);
    void setMessage(const char*);
    virtual char getKey();
    std::string getFileName() const;
    std::string getStock() const;
};





#endif //ORDER_MATCHING_ENGINE_READER_H
