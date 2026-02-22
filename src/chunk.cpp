#include "chunk.hpp"
#include <cstdio>

void Chunk::writeChunk(OpCode opcode, int line)
{
    writeChunk(static_cast<std::uint8_t>(opcode), line);
}

void Chunk::writeChunk(std::uint8_t byte, int line)  // NOLINT
{
    code.push_back(byte);
    std::size_t lineE {static_cast<std::size_t>(line)};

    // run-length encoding
    if (!lines.empty() && lines[lines.size() - 2] == lineE) {
        ++lines[lines.size() - 1]; 
    }
    else 
    { 
        lines.push_back(lineE);
        lines.push_back(1);
    }
}