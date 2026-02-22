#include "chunk.hpp"
#include <cstdio>

void Chunk::writeChunk(OpCode opcode, int line)
{
    writeChunk(static_cast<std::uint8_t>(opcode), line);
}

void Chunk::writeChunk(std::uint8_t byte, int line)  // NOLINT
{
    code.push_back(byte);
    
    if (lines.empty() || lines.back().line != line)
    {
        lines.push_back({code.size(), line});
    } else
    {
        lines.back().endOffset = code.size();
    }
}