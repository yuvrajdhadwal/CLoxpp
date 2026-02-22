#include "chunk.hpp"

void Chunk::writeValue(Value value)
{
    constants.push_back(value);
}

auto Chunk::addConstant(Value value) -> std::uint8_t
{
    writeValue(value);
    return static_cast<std::uint8_t>(constants.size()) - 1;
}

void Chunk::writeConstant(Value value, int line)  // NOLINT
{
    writeValue(value);
    std::size_t index {constants.size() - 1ULL};

    const std::size_t byte1Mask {0xFF0000};
    const std::size_t byte1Shift {16};
    std::uint8_t byte1 {static_cast<uint8_t>((index & byte1Mask) >> byte1Shift)};

    const std::size_t byte2Mask {0xFF00};
    const std::size_t byte2Shift {8};
    std::uint8_t byte2 {static_cast<uint8_t>((index & byte2Mask) >> byte2Shift)};

    const std::size_t byte3Mask {0xFF};
    const std::size_t byte3Shift {0};
    std::uint8_t byte3 {static_cast<uint8_t>((index & byte3Mask) >> byte3Shift)};

    writeChunk(OpCode::OP_CONSTANT_LONG, line);
    writeChunk(byte1, line);
    writeChunk(byte2, line);
    writeChunk(byte3, line);
}

void printValue(Value value)
{
    std::cout << value;
}