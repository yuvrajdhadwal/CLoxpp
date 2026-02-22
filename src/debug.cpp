#include "chunk.hpp"

void printValue(Value value);

static auto simpleInstruction(const std::string_view name, std::size_t offset) -> std::size_t
{
    std::cout << name << '\n';
    return offset + 1;
}

static auto constantInstruction(const char* name, const Chunk& chunk, std::size_t offset) 
    -> std::size_t
{
    // gets the index of constant in the constants vector
    const std::uint8_t constantIndex {chunk.getCode(offset + 1)};  
    printf("%-16s %4d '", name, constantIndex);  // NOLINT
    
    printValue(chunk.getConstant(constantIndex));
    std::cout << '\n';
    return offset + 2;
}

static auto constantLongInstruction(const char* name, const Chunk& chunk, std::size_t offset)
    -> std::size_t
{
    // gets the index of constant in the constants vector
    const std::uint8_t constantIndex1 {chunk.getCode(offset + 1)};
    const std::uint8_t constantIndex2 {chunk.getCode(offset + 2)};  
    const std::uint8_t constantIndex3 {chunk.getCode(offset + 3)};  
    const std::size_t constantIndex {static_cast<size_t>((constantIndex1 << 8ULL) 
        | (constantIndex2 << 4ULL) | constantIndex3)};
    
    printf("%-16s %4d '", name, static_cast<int>(constantIndex));  // NOLINT
    
    printValue(chunk.getConstant(constantIndex));
    std::cout << '\n';
    return offset + 4;
}

void disassembleChunk(Chunk& chunk, const std::string_view name)
{
    std::cout << "== " << name << " ==\n";

    for (std::size_t offset {0}; offset < chunk.code.size();)
    {
        offset = disassembleInstruction(chunk, offset);
    }
}

auto disassembleInstruction(Chunk& chunk, std::size_t offset) -> std::size_t
{
    printf("%04d ", static_cast<int>(offset));  // NOLINT

    if (offset > 0 && chunk.lines[offset]  == chunk.lines[offset - 1])
    {
        std::cout << "   | ";
    } else { printf("%4d ",  // NOLINT
        static_cast<int>(getLine(chunk, offset))); }  

    OpCode instruction {chunk.code[offset]};

    switch (instruction)
    {
        case OpCode::OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OpCode::OP_CONSTANT_LONG:
            return constantLongInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OpCode::OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            std::cout << "Unknown opcode: " << static_cast<uint8_t>(instruction) << '\n';
            return offset + 1;
    }
}

auto getLine(Chunk& chunk, std::size_t instrIndex) -> std::size_t
{
    std::size_t currInstrIndex {0};
    std::size_t currLineIndex {0};

    while (currInstrIndex + chunk.lines[currLineIndex + 1] <= instrIndex)
    {
        currInstrIndex += chunk.lines[currLineIndex + 1];
        currLineIndex += 2;
    }

    return chunk.lines[currLineIndex];
}