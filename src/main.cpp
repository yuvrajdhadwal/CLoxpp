#include "common.hpp"
#include "chunk.hpp"
#include "vm.hpp"

#include <cstdint>

// int argc, char* argv[]
auto main() -> int
{
    VirtualMachine virtm{};
    Chunk chunk{};

    std::uint8_t constant {chunk.addConstant(1.2)};  // NOLINT
    chunk.writeChunk(OpCode::OP_CONSTANT, 4);
    chunk.writeChunk(constant, 4);

//    constant = chunk.addConstant(3.4);
//    chunk.writeChunk(OpCode::OP_CONSTANT, 4);
//    chunk.writeChunk(constant, 4);
    chunk.writeConstant(3.4, 4);

    chunk.writeChunk(OpCode::OP_ADD, 4);

//    constant = chunk.addConstant(5.6);
//    chunk.writeChunk(OpCode::OP_CONSTANT, 4);
//    chunk.writeChunk(constant, 4);
    chunk.writeConstant(5.6, 4);
    
    chunk.writeChunk(OpCode::OP_DIVIDE, 4);

    chunk.writeChunk(OpCode::OP_NEGATE, 4);
    chunk.writeChunk(OpCode::OP_RETURN, 4);

    chunk.writeChunk(OpCode::OP_RETURN, 1);
    chunk.writeChunk(OpCode::OP_RETURN, 1);
    chunk.writeChunk(OpCode::OP_RETURN, 1);
    chunk.writeChunk(OpCode::OP_RETURN, 2);
    chunk.writeChunk(OpCode::OP_RETURN, 3);
    chunk.writeChunk(OpCode::OP_RETURN, 3);
    chunk.writeConstant(15, 8);  // NOLINT
    chunk.writeConstant(12, 9);  // NOLINT
    chunk.writeConstant(11, 15);  // NOLINT
    chunk.writeConstant(25, 99);  // NOLINT
    
    chunk.disassembleChunk("test chunk");
    virtm.interpret(chunk);

    return 0;
}
