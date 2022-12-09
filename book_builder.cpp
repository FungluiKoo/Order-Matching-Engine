#include "book_builder.h"


BookBuilder::BookBuilder(const std::string &inputMessagePath,
                         const std::string &outputMessageCSV
                         ):
        message_reader(inputMessagePath),
        messageWriter(outputMessageCSV)
{
    std::cout << "Begin building book and matching orders" << std::endl;
    totalTime = time(0);
}

BookBuilder::~BookBuilder()
{
    std::cout << "Finish building book and matching orders in "
    << difftime(time(0),totalTime) << "seconds."  << std::endl;
    std::cout << "Total Add Order is " << totalAdd << " and "
    << "Total Delete Order is " << totalDelete << std::endl;
}

void BookBuilder::start(){
    while(!message_reader.eof() and message_reader.isValid()){
        next();
    }
}

void BookBuilder::next(){
    message = message_reader.createMessage();
    if(!message.isEmpty()){
        bool validMessage = updateMessage();
        if(validMessage){
            updateBook();
        }
    }
}



bool BookBuilder::updateMessage() {
//    char typeMsg = message.getType();
//
//    if (typeMsg == 'A' or typeMsg == 'P') {
//        return true;
//    }

    return true;

}

void BookBuilder::updateBook() {
    char typeMsg = message.getType();

    if (typeMsg == 'A')
    {
        // if the ticket is in the selected array
        if (in_array(message.getTicker(), SymbolFilters))
        {
            OrderType type = OrderType::LIMIT;
            OrderSide side = (message.getSide() == 0) ? OrderSide::BUY: OrderSide::SELL;
            Order thisOrder(message.getId(),0,
                            static_cast<unsigned int>(message.getPrice()),message.getRemSize(),
                            side ,type,0);
            centralBook.add_order(message.getTicker(), thisOrder);
            // std::cout << "Thread detached." << std::endl;

//            centralBook.printBuySellPool(message.getTicker());
//            message.print();
            totalAdd += 1;
        }
    }
    else if(typeMsg == 'D')
    {
        centralBook.delete_order(message.getId());
        // if (s == StatusCode :: OK) {
            totalDelete += 1;

        // }

    }
    else
    {
        std::cerr << "Unexpected type! " << typeMsg << std::endl;
    }

}

