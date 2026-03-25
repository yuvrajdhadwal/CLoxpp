#include "chunk.hpp"

void Chunk::writeChunk(OpCode opcode, std::size_t line) {
    writeChunk(static_cast<std::uint8_t>(opcode), line);
}

void Chunk::writeChunk(std::uint8_t byte, std::size_t line)  // NOLINT
{
    m_code.push_back(byte);

    if (m_lines.empty() || m_lines.back().line != line) {
        m_lines.emplace_back(m_code.size(), line);
    } else {
        m_lines.back().endOffset = m_code.size();
    }
}
