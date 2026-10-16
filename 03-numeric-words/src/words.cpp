#include "words.hpp"

#include <filesystem>
#include <fstream>
#include <cctype>
#include <expected>
#include <charconv>  // for std::from_chars
#include <iostream>  // for std::cout, std::cerr
#include <print>     // for std::print
#include <format>    // for std::format>

void open_or_throw(const std::filesystem::path& path, std::ifstream& file) {
    file.open(path);
    if (!file.is_open( )) {
        throw std::runtime_error("cannot open file: " + path.string( ));
    }
}

std::string_view strip_comment(std::string_view line) {
    return line.substr(0, line.find_first_of(";("));
}

std::expected<Word, ParseError> parse_word(char letter, std::string_view digits) {
    if (letter == 'G' || letter == 'N') {
        auto r = parse_number<int>(digits);
        if (!r) {                        
			return std::unexpected(ParseError{ ParseErrorKind::WrongType, 0, std::format("{} wants int", letter) });
        }
        return Word{ letter, *r };
    }
    else if (letter == 'X' || letter == 'Y' || letter == 'Z' || letter == 'F') {
        auto r = parse_number<double>(digits);
        if (!r) {
            return std::unexpected(ParseError{ ParseErrorKind::WrongType, 0, std::format("{} wants double", letter) });
        }
        return Word{ letter, *r };
    }
    else {
		return std::unexpected(ParseError{ ParseErrorKind::WrongType, 0, std::string("is not a word") });
    }
}

[[nodiscard]] std::expected<std::vector<Word>, ParseError> parse_line(std::string_view line, int line_number) {
    std::vector<Word> words;
                                                                         
    line = strip_comment(line);          // результат обязательно присвоить
    if (line.empty( )) {
        return words;                    // пусто или только комментарий
    }

    size_t pos = 0;
    while (pos < line.size( )) {
        // пропуск пробелов, \t, \r
        while (pos < line.size( ) && std::isspace(static_cast<unsigned char>(line[pos]))) {
            ++pos;
        }
        if (pos >= line.size( )) {
            break;
        }

        char letter = line[pos];
        if (!std::isalpha(static_cast<unsigned char>(letter))) {
            return std::unexpected(ParseError{ ParseErrorKind::UnexpectedChar, line_number, "unexpected char" });
        }

        // конец числа: до пробела или до следующей буквы
        size_t end = pos + 1;
        while (end < line.size( )
               && !std::isspace(static_cast<unsigned char>(line[end]))
               && !std::isalpha(static_cast<unsigned char>(line[end]))) {
            ++end;
        }

        // пустой кусок ("X" без числа) отловит parse_number -> MissingNumber
        auto result = parse_word(letter, line.substr(pos + 1, end - pos - 1));
        if (!result) {
            ParseError err = result.error( );
            err.line_number = line_number;   // номер строки подставляем здесь
            return std::unexpected(err);
        }
        words.push_back(*result);
        pos = end;                           // к следующему слову
    }

    return words;
}

void output_words(const std::vector<Word>& words, int line_number) {
    std::cout << line_number << ':';
    for (const Word& w : words) {
        std::cout << ' ' << w.letter << '=';
        std::visit([ ](auto v) { std::cout << v; }, w.value);
    }
    std::cout << '\n';
}

void load_parse(std::istream& in) { 
    std::string line;
    int line_number{ 0 };
    while (std::getline(in, line)) {
        ++line_number;
        auto result = parse_line(line, line_number);
        if (!result) {
            std::print("{}: error: {}\n", line_number, result.error( ).message);
            continue;
        }
        if (result->empty( )) {
            continue;
        }
        output_words(*result, line_number);
    }
}