#include "chunk.hpp"

void Chunk::writeChunk(OpCode opcode, int line)
{
    writeChunk(static_cast<std::uint8_t>(opcode), line);
}

void Chunk::writeChunk(std::uint8_t byte, int line)  // NOLINT
{
    m_code.push_back(byte);
    
    if (m_lines.empty() || m_lines.back().line != line)
    {
        m_lines.emplace_back(m_code.size(), line);
    } else
    {
        m_lines.back().endOffset = m_code.size();
    }
}
