#include <iostream>
#include "book_builder.h"

using namespace std;

int main() {
    std::cout << "Welcome to the Nasdaq ITCH order matching engine" << std::endl;

    string file_path = "./data/03272019.PSX_ITCH50";
    string outputMessageCSV = "./data/test.log";


    std::cout << "---------------------start------------------------" << std::endl;

    BookBuilder builder(file_path, outputMessageCSV);
    builder.start();

    std::cout << "---------------------end------------------------" << std::endl;


    return 0;
}