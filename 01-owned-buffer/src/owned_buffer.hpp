#include <cstddef>  // std::size_t
#include <istream>  // std::istream для load
#include <string>   // конструктор из строки и str()
#include <utility>  // std::move
#include <vector>   // std::vector<Block>
#include <algorithm> // std::copy

class OwnedBuffer {
public:
    OwnedBuffer( ) = default;
    explicit OwnedBuffer(const std::string& line);
    ~OwnedBuffer( );
    OwnedBuffer(const OwnedBuffer&) = delete;
    OwnedBuffer& operator=(const OwnedBuffer&) = delete;
    OwnedBuffer(OwnedBuffer&& other) noexcept;
    OwnedBuffer& operator=(OwnedBuffer&& other) noexcept;
    std::size_t size( ) const { return n_; }
    std::string str( ) const;
private:
    char* data_ = nullptr;
    std::size_t n_ = 0;
};

struct Block {          // rule of 0: своих особых членов нет
    int line = 0;
    OwnedBuffer buf;
};

std::vector<Block> load(std::istream& in);

struct StorageBlocks {
    std::size_t total_blocks{ 0 };
    std::size_t total_bytes{ 0 };
    std::string  first_line{ "" };
	std::string last_line{ "" };
};

StorageBlocks analyze_blocks(const std::vector<Block>& blocks);