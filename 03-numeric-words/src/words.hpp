#pragma once
#include <concepts>
#include <expected>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <fstream>
#include <filesystem>

void open_or_throw(const std::filesystem::path& path, std::ifstream& file);

std::string_view strip_comment(std::string_view line);

enum class ParseErrorKind {
    UnexpectedChar,
    MissingNumber,
    BadNumber,
    WrongType,
};

struct ParseError {
    ParseErrorKind kind;
	int line_number;
    std::string message;
};

template<typename T>
concept Number = (std::integral<T> || std::floating_point<T>) && !std::same_as<T, bool>; 

// Значение слова: int для G/N, double для X/Y/Z/F.
struct Word {
    char letter;
    std::variant<int, double> value;

    template<Number T>
	Word(char l, T v) : letter(l), value(v) {}
};

// Шаблон один на все типы. Тело в заголовке: шаблон должен быть виден в точке вызова.
template<Number T>
[[nodiscard]] std::expected<T, ParseError> parse_number(std::string_view digits) {
    if (digits.empty( )) {
        return std::unexpected(ParseError{ ParseErrorKind::MissingNumber, 0, "missing number" });
    }
    T value{};
    auto [ptr, ec] = std::from_chars(digits.data( ), digits.data( ) + digits.size( ), value);

    if (ec != std::errc{}) {
        return std::unexpected(ParseError{ ParseErrorKind::BadNumber, 0, "bad number" });
    }
    else if (ptr != digits.data( ) + digits.size( )) {
        return std::unexpected(ParseError{ ParseErrorKind::BadNumber, 0, "bad number" });
    }
    else {
        return value;
    }
}

std::expected<Word, ParseError> parse_word(char letter, std::string_view digits);

[[nodiscard]] std::expected<std::vector<Word>, ParseError> parse_line(std::string_view line, int line_number);

void output_words(const std::vector<Word>& words, int line_number);

void load_parse(std::istream& in);