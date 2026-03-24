#pragma once

#include <iostream>
#include <string_view>

#include "chunk.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "tokentype.hpp"

class Compiler {
   public:
    Compiler(std::string_view source, Chunk& chunk);
    [[nodiscard]] auto isOK() const { return !m_hadError; }

   private:
    void startCompile();
    void finishCompile() { emitReturn(); }

    void advance(Scanner& scanner);
    void consume(TokenType type, std::string_view message, Scanner& scanner);

    void emitByte(uint8_t byte) { m_chunk.writeChunk(byte, m_previous.getLine()); }
    void emitByte(OpCode byte) { m_chunk.writeChunk(byte, m_previous.getLine()); }
    void emitReturn() { emitByte(OpCode::OP_RETURN); }
    void emitBytes(uint8_t byteLeft, uint8_t byteRight) {
        emitByte(byteLeft);
        emitByte(byteRight);
    }
    void emitBytes(OpCode byteLeft, OpCode byteRight) {
        emitByte(byteLeft);
        emitByte(byteRight);
    }

    void errorAtCurrent(std::string_view message) { errorAt(m_current, message); }
    void error(std::string_view message) { errorAt(m_previous, message); }
    void errorAt(Token& token, std::string_view message);

    Chunk& m_chunk;  // NOLINT
    Token m_current;
    Token m_previous;
    std::string_view m_source;
    bool m_hadError;
    bool m_panicMode;
};
