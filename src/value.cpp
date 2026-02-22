#include "chunk.hpp"

void writeValue(std::vector<Value>& array, Value value)
{
    array.push_back(value);
}

auto addConstant(Chunk& chunk, Value value) -> std::uint8_t
{
    writeValue(chunk.constants, value);
    return static_cast<std::uint8_t>(chunk.constants.size()) - 1;
}

void writeConstant(Chunk& chunk, Value value, int line)  // NOLINT
{
    writeValue(chunk.constants, value);
    std::size_t index {chunk.constants.size() - 1ULL};

    const std::size_t byte1Mask {0xF00};
    const std::size_t byte1Shift {8};
    std::uint8_t byte1 {static_cast<uint8_t>((index & byte1Mask) >> byte1Shift)};

    const std::size_t byte2Mask {0xF0};
    const std::size_t byte2Shift {4};
    std::uint8_t byte2 {static_cast<uint8_t>((index & byte2Mask) >> byte2Shift)};

    const std::size_t byte3Mask {0xF};
    const std::size_t byte3Shift {0};
    std::uint8_t byte3 {static_cast<uint8_t>((index & byte3Mask) >> byte3Shift)};

    writeChunk(chunk, OpCode::OP_CONSTANT_LONG, line);
    writeChunk(chunk, byte1, line);
    writeChunk(chunk, byte2, line);
    writeChunk(chunk, byte3, line);
}

void printValue(Value value)
{
    std::cout << value;
}