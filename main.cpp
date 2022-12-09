#include <iostream>
#include "book_builder.h"

using namespace std;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout << "Welcome to the Nasdaq ITCH order matching engine" << std::endl;

    string file_path = "./data/03272019.PSX_ITCH50";
    string outputMessageCSV = "./data/test.log";


    std::cout << "---------------------start------------------------" << std::endl;

    BookBuilder builder(file_path, outputMessageCSV);
    builder.start();

    std::cout << "---------------------end------------------------" << std::endl;


    return 0;
}