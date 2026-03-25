#pragma once

#include <cstdlib>
#include <functional>
#include <iostream>
#include <string_view>

#include "chunk.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "tokentype.hpp"

enum class Precedence : uint8_t {
    NONE,
    ASSIGNMENT,  // =
    OR,          // or
    AND,         // and
    EQUALITY,    // == !=
    COMPARISON,  // < > <= >=
    TERM,        // + -
    FACTOR,      // * /
    UNARY,       // ! -
    CALL,        // . ()
    PRIMARY
};

class Compiler;

struct ParseRule {
    void (Compiler::*prefix)();
    void (Compiler::*infix)();
    Precedence precedence;
};

class Compiler {
   public:
    Compiler(std::string_view source, Chunk& chunk);
    [[nodiscard]] auto isOK() const { return !m_hadError; }

   private:
    void startCompile();
    void finishCompile() { emitReturn(); }

    void advance();
    void consume(TokenType type, std::string_view message);
    void expression();
    void grouping();
    void unary();
    void binary();

    void parsePrecedence(Precedence precedence);
    auto getRule(TokenType type) -> ParseRule& { return rules[static_cast<std::size_t>(type)]; }  // NOLINT

    void number();
    auto makeConstant(Value value) -> uint8_t;

    void emitByte(uint8_t byte) { m_chunk.writeChunk(byte, m_previous.getLine()); }
    void emitByte(OpCode byte) { m_chunk.writeChunk(byte, m_previous.getLine()); }
    void emitReturn() { emitByte(OpCode::RETURN); }
    void emitBytes(uint8_t byteLeft, uint8_t byteRight) {
        emitByte(byteLeft);
        emitByte(byteRight);
    }
    void emitBytes(OpCode byteLeft, uint8_t byteRight) {
        emitByte(byteLeft);
        emitByte(byteRight);
    }
    void emitBytes(OpCode byteLeft, OpCode byteRight) {
        emitByte(byteLeft);
        emitByte(byteRight);
    }
    void emitConstant(Value value) { emitBytes(OpCode::CONSTANT, makeConstant(value)); }

    void errorAtCurrent(std::string_view message) { errorAt(m_current, message); }
    void error(std::string_view message) { errorAt(m_previous, message); }
    void errorAt(Token& token, std::string_view message);

    Chunk& m_chunk;  // NOLINT
    Token m_current;
    Token m_previous;
    std::string_view m_source;
    bool m_hadError;
    bool m_panicMode;
    Scanner m_scanner;

    std::array<ParseRule, 40> rules = {{
        {&Compiler::grouping, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {&Compiler::unary, &Compiler::binary, Precedence::TERM},
        {NULL, &Compiler::binary, Precedence::TERM},
        {NULL, NULL, Precedence::NONE},
        {NULL, &Compiler::binary, Precedence::FACTOR},
        {NULL, &Compiler::binary, Precedence::FACTOR},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {&Compiler::number, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
        {NULL, NULL, Precedence::NONE},
    }};
};
