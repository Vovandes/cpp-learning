#include "words.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>

// Проверка concept: обе строки должны НЕ компилироваться.
// auto bad1 = parse_number<std::string>("1"); // должно падать: не число
// auto bad2 = parse_number<bool>("1");        // должно падать: bool не Number


int main(int argc, char* argv[]) {
    try {
        std::istream* in = &std::cin;
        std::ifstream file;
        if (argc > 1) {
            open_or_throw(argv[1], file);
            in = &file;                                                       
        }
        load_parse(*in);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what( ) << "\n";
        return 1;
    }
    return 0;
}