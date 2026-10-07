#pragma once
#include <expected>
#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <filesystem>

void open_or_throw(const std::filesystem::path& path, std::ifstream& file);

struct Word {
	char letter{};
	double value{};
};

struct ParseError {
	int line = 0;
	std::string why;
};

[[nodiscard]] std::string_view strip_comment(std::string_view line);

[[nodiscard]] std::expected<Word, ParseError>
parse_word(std::string_view token, int line); // не бросает

[[nodiscard]] std::expected<std::vector<Word>, ParseError>
parse_line(std::string_view line, int line_no); // комментарий срезать здесь

void output_words(const std::vector<Word>& words, int line_number);

void load_parse(std::istream& in);