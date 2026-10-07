#include "parse.hpp"
#include <charconv>     // std::from_chars: строка -> double без исключений
#include <cctype>       // std::isalpha: символ это буква
#include <expected>     // std::expected и std::unexpected
#include <string>       // std::string в тексте ошибки
#include <string_view>  // token не копирует строку, только смотрит на неё
#include <vector>       // std::vector<Word> для parse_line
#include <stdexcept>    // std::runtime_error для open_or_throw
#include <fstream>      // std::ifstream для open_or_throw>
#include <filesystem>   // std::filesystem::path для open_or_throw
#include <iostream>	    // std::istream для load_parse
#include <format>       // std::format для вывода ошибок>

void open_or_throw(const std::filesystem::path& path, std::ifstream& file) {
	file.open(path);
	if (!file.is_open( )) {
		throw std::runtime_error("cannot open file");
	}
}

std::string_view strip_comment(std::string_view line) {
	std::string_view strip_line = line.substr(0, line.find_first_of(";("));
	return strip_line;
}

[[nodiscard]] std::expected<Word, ParseError>
parse_word(std::string_view token, int line) {
	// пусто или первый символ не буква — это не слово
	if (token.empty( ) || !std::isalpha(static_cast<unsigned char>(token[0])))
		return std::unexpected(ParseError{ line, "not a word" });
	const char letter = token[0]; // 'G', 'X', 'Y', 'F' — буква слова
	std::size_t i = 1;        // число начинается сразу после буквы
	// идём, пока не конец, не пробел и не следующая буква
	while (i < token.size( )
		   && token[i] != ' '
		   && !std::isalpha(static_cast<unsigned char>(token[i]))) {
		++i; // '-' точка и цифры остаются внутри числа
	}
	// кусок между буквой и остановкой: "10", "-2.5" или пусто у голого "X"
	const auto num = token.substr(1, i - 1);
	if (num.empty( )) {
		return std::unexpected(ParseError{ line, std::string("missing number after ") + letter });
	}
	double value = 0.0;
	// from_chars пишет в value и говорит, где остановился
	const auto [ptr, ec] = std::from_chars(num.data( ), num.data( ) + num.size( ), value);
	// ec не ноль — не число; ptr не в конце — съели не всё ("10abc")
	if (ec != std::errc{} || ptr != num.data( ) + num.size( )) {
		return std::unexpected(ParseError{ line, "bad number" });
	}
	return Word{ letter, value }; // успех: буква и число, исключения нет
}

[[nodiscard]] std::expected<std::vector<Word>, ParseError>
parse_line(std::string_view line, int line_no) {
	line = strip_comment(line);
	std::vector<Word> words;
	if (line.empty( )) {
		return words; // пустая строка без слов — успех, пустой вектор
	}
	//===================================================================
	// поменять код под std::string_view, у него нет ++ придется идти через индексы
	std::size_t i = 0;
	while (i < line.size( )) {
		// ищем начало токена: пропускаем пробелы
		while (i < line.size( ) && line[i] == ' ') {
			++i;
		}
		if (i == line.size( )) {
			break; // дошли до конца строки, слов больше нет
		}
		const auto token_start = i;
		// ищем конец токена: до пробела или конца строки
		while (i < line.size( ) && line[i] != ' ') {
			++i;
		}
		const auto token_end = i;
		const std::string_view token(line.data( ) + token_start, token_end - token_start);
		const auto result = parse_word(token, line_no);
		if (!result) {
			return std::unexpected(result.error( )); // ошибка в слове
		}
		words.push_back(result.value( )); // успех: добавляем слово в вектор
	}
	//===================================================================
	return words;
}

void output_words(const std::vector<Word>& words, int line_number) {
	std::cout << line_number << ':';
	for (const Word& w : words) {
		std::cout << ' ' << w.letter << w.value; // G 1 даёт G1, X 10 даёт X10
	}
	std::cout << '\n';
}

void load_parse(std::istream& in) {
	std::string line;
	int line_number{ 1 };
	while (std::getline(in, line)) {
		//++line_number;
		auto result = parse_line(line, line_number);
		if (!result) {
			std::cout << std::format("{}: error: {}\n", line_number++, result.error( ).why);
			continue; // к следующей строке файла
		}
		if (result->empty( )) {
			continue; // комментарий или пустая строка, не печатать
		}
		output_words(*result, line_number++);
	}
}