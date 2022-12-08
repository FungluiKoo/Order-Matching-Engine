#ifndef ORDER_MATCHING_ENGINE_BOOK_BUILDER_H
#define ORDER_MATCHING_ENGINE_BOOK_BUILDER_H

#include "Parser/message.h"
#include "Parser/reader.h"
#include "Parser/writer.h"
#include "OrderMatcher/order.hh"
#include "OrderMatcher/central_order_book.hh"
#include <algorithm>

class BookBuilder{
private:
    Message message;
    CentralOrderBook centralBook;
    Reader message_reader;
    Writer messageWriter;
    Writer bookWriter;
    Writer parserWriter;
    time_t totalTime;
    std::vector<std::string> SymbolFilters =
            { "AAPL", "MSFT", "TSLA", "AMZN"};

public:
    BookBuilder(const std::string &inputMessagePath,
                const std::string &outputMessageCSV
                );

    ~BookBuilder();

    void start();

    void next();

    bool updateMessage();

    void updateBook();

//    void updatePool();

    void WriteBookAndMessage();

    bool in_array(const std::string &value, const std::vector<std::string> &array)
    {
        return std::find(array.begin(), array.end(), value) != array.end();
    }
};


#endif //ORDER_MATCHING_ENGINE_BOOK_BUILDER_H
