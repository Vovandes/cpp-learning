#include "owned_buffer.hpp"

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
			block.line = container.size( ) + 1; // line number starts from 1
			block.buf = OwnedBuffer(line);
			container.push_back(std::move(block));
		}
	}
	return container;
}

StorageBlocks analyze_blocks(const std::vector<Block>& blocks) {
	StorageBlocks stats;
	for (const auto& elem : blocks) {
		stats.total_blocks = blocks.size( );
		stats.total_bytes += elem.buf.size( );
		stats.first_line = blocks.front( ).buf.str( );
		stats.last_line = blocks.back( ).buf.str( );
	}
	return stats;
}
