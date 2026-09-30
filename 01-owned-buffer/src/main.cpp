#include "owned_buffer.hpp" // OwnedBuffer, Block, load
#include <iostream>         // cin, cout, ifstream
#include <fstream>          // файл из argv[1]
#include <format>		  // std::format

// argv[1] -> файл, иначе stdin
// auto blocks = load(...);   // без std::move на return внутри load
// печать: blocks, bytes, first, last
int main(int argc, char** argv) {
	std::istream* in = &std::cin;
	std::ifstream file;
	if (argc > 1) {
		file.open(argv[1]);
		if (!file) {
			std::cerr << "Error opening file: " << argv[1] << std::endl;
			return 1;
		}
		in = &file;
	}
	auto blocks = load(*in);
	auto stats = analyze_blocks(blocks);
	auto  format_str = std::format("blocks: {}\nbytes: {}\nfirst: {}\nlast: {}\n",
		stats.total_blocks, stats.total_bytes, stats.first_line, stats.last_line);

	std::cout << format_str;
	return 0;

}