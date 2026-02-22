#include "chunk.hpp"
#include <cstdio>

void writeChunk(Chunk& chunk, OpCode byte, int line)
{
    writeChunk(chunk, static_cast<std::uint8_t>(byte), line);
}

void writeChunk(Chunk& chunk, std::uint8_t byte, int line)  // NOLINT
{
    chunk.code.push_back(byte);
    std::size_t lineE {static_cast<std::size_t>(line)};

    // run-length encoding
    if (!chunk.lines.empty() && chunk.lines[chunk.lines.size() - 2] == lineE) {
        ++chunk.lines[chunk.lines.size() - 1]; 
    }
    else 
    { 
        chunk.lines.push_back(lineE);
        chunk.lines.push_back(1);
    }
}