#include "common.hpp"
#include "chunk.hpp"
#include <cstdint>

// int argc, char* argv[]
auto main() -> int
{
    Chunk chunk{};

    writeChunk(chunk, OpCode::OP_RETURN, 1);
    writeChunk(chunk, OpCode::OP_RETURN, 1);
    writeChunk(chunk, OpCode::OP_RETURN, 1);
    writeChunk(chunk, OpCode::OP_RETURN, 2);
    writeChunk(chunk, OpCode::OP_RETURN, 3);
    writeChunk(chunk, OpCode::OP_RETURN, 3);

    std::uint8_t constant {addConstant(chunk, 1.2)};  // NOLINT
    writeChunk(chunk, OpCode::OP_CONSTANT, 4);
    writeChunk(chunk, constant, 4);

    writeChunk(chunk, OpCode::OP_RETURN, 4);

    writeConstant(chunk, 15, 8);  // NOLINT
    writeConstant(chunk, 12, 9);  // NOLINT
    writeConstant(chunk, 11, 15);  // NOLINT
    writeConstant(chunk, 25, 99);  // NOLINT
    
    disassembleChunk(chunk, "test chunk");

    return 0;
}