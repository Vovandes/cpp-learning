#include "owned_buffer.hpp"
#include <algorithm> // std::copy
#include <numeric>

OwnedBuffer::OwnedBuffer(const std::string& line) {
	n_ = line.size( );
	data_ = new char[n_];
	std::copy(line.begin( ), line.end( ), data_);
}

OwnedBuffer::~OwnedBuffer( ) {
	delete[] data_;
}

OwnedBuffer::OwnedBuffer(OwnedBuffer&& other) noexcept :
	data_(other.data_),
	n_(other.n_) {
	other.data_ = nullptr;
	other.n_ = 0;
}

OwnedBuffer& OwnedBuffer::operator=(OwnedBuffer&& other) noexcept {
	if (this == &other) { return *this;	}
	delete[] data_;
	data_ = other.data_;
	n_ = other.n_;
	other.data_ = nullptr;
	other.n_ = 0;
	return *this;
}

std::string OwnedBuffer::str( ) const {
	return std::string( data_, n_ );
}

std::vector<Block> load(std::istream& in) {
	std::vector<Block> container;
	std::string line;
	while (std::getline(in, line)) {
		if (!line.empty( )) {
			Block block;
			block.line = container.size( ) + 1; // block number starts from 1
			block.buf = OwnedBuffer(line);
			container.push_back(std::move(block));
		}
	}
	return container;
}

StorageBlocks analyze_blocks(const std::vector<Block>& blocks) {
	StorageBlocks stats;
	if (!blocks.empty( )) {
		stats.total_blocks = blocks.size( );
		stats.first_line = blocks.front( ).buf.str( );
		stats.last_line = blocks.back( ).buf.str( );
		stats.total_bytes = std::reduce(blocks.begin( ), blocks.end( ), std::size_t{ 0 },
			[ ](std::size_t sum, const Block& block) {
				return sum + block.buf.size( );
			});
	}
	return stats;
}
