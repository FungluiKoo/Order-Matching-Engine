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
    std::cout << "====" << message.getType() << "=====" << std::endl;

    char typeMsg = message.getType();

    if (typeMsg == 'A')
    {
        std::cout << "====" << message.getTicker() << "=====" << std::endl;
//        if (!in_array(message.getString(), SymbolFilters))

//        OrderType type = OrderType::LIMIT;
//        OrderSide side = OrderSide::BUY;
//        Order thisOrder(message.getId(),0,
//                        message.getPrice(),message.getRemSize(),
//                        side,type,0);
//        book.add_order(message.getMPID(), thisOrder);

//        book.modifySize(message.getPrice(), message.getRemSize(), message.getSide());
    }
    else if(typeMsg == 'D')
    {



        // Cancel order. Totally or partially.
//        book.modifySize(message.getPrice(),-message.getCancSize(),message.getSide());
    }
    else
    {
        std::cerr << "Unexpected type! " << typeMsg << std::endl;
    }

    std::cout << std::endl;
}

