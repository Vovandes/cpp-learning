// I/O: исключение. Файла нет — продолжать нечего.
// Разбор слова: expected. Битая строка не отменяет следующие.
// Не наоборот: expected на «нет файла» заставляет каждый вызов
// проверять диск, а throw на плохое слово роняет весь файл.
#include "parse.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    try {
        std::istream* in = &std::cin;
        std::ifstream file;
        if (argc > 1) {
            open_or_throw(argv[1], file);
            in = &file;
        }
		load_parse(*in);
        
		std::system("pause");
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what( ) << '\n';
        return 1;
    }
    return 0;
}