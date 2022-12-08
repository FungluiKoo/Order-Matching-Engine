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
//            updatePool();
            updateBook();
//            WriteBookAndMessage();
        }
    }
}


void BookBuilder::WriteBookAndMessage() {
        std::cout << message.getString() << std::endl;
//    messageWriter.writeLine(message.getString());
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
//    std::cout << "====" << message.getType() << "=====" << std::endl;

    char typeMsg = message.getType();

    if (typeMsg == 'A')
    {
//        std::cout << "====" << message.getTicker() << "=====" << std:):endl;
//        std::cout << "====" << message.getTicker() << "=====" << std::endl

        // if the ticket is in the selected array
        if (in_array(message.getTicker(), SymbolFilters))
        {
            OrderType type = OrderType::LIMIT;
            OrderSide side = (message.getSide() == 0) ? OrderSide::BUY: OrderSide::SELL;
            Order thisOrder(message.getId(),0,
                            static_cast<unsigned int>(message.getPrice()),message.getRemSize(),
                            side ,type,0);
            centralBook.add_order(message.getTicker(), thisOrder);
            std::cout << "Add Order successfully" << std::endl;

//            centralBook.printBuySellPool(message.getTicker());
//            message.print();
        }
//        book.modifySize(message.getPrice(), message.getRemSize(), message.getSide());
    }
    else if(typeMsg == 'D')
    {
        StatusCode s = centralBook.delete_order(message.getId());
        if (s == StatusCode :: OK) {
            std::cout << "Delete Order successfully" << std::endl;
//            message.print();
        }
        // Cancel order. Totally or partially.
        // book.modifySize(message.getPrice(),-message.getCancSize(),message.getSide());
    }
    else
    {
        std::cerr << "Unexpected type! " << typeMsg << std::endl;
    }

}

