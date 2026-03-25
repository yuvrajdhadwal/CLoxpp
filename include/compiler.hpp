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
    auto getRule(TokenType type) -> ParseRule& {
        return rules[static_cast<std::size_t>(type)];  // NOLINT
    }

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

    static constexpr std::size_t PARSE_RULE_COUNT{40};

    std::array<ParseRule, PARSE_RULE_COUNT> rules = {{
        // TOKEN_LEFT_PAREN
        {.prefix = &Compiler::grouping, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_RIGHT_PAREN
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_LEFT_BRACE
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_RIGHT_BRACE
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        //  TOKEN_COMMA
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        //  TOKEN_DOT
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_MINUS
        {.prefix = &Compiler::unary, .infix = &Compiler::binary, .precedence = Precedence::TERM},
        //  TOKEN_PLUS
        {.prefix = nullptr, .infix = &Compiler::binary, .precedence = Precedence::TERM},
        // TOKEN_SEMICOLON
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_SLASH
        {.prefix = nullptr, .infix = &Compiler::binary, .precedence = Precedence::FACTOR},
        // TOKEN_STAR
        {.prefix = nullptr, .infix = &Compiler::binary, .precedence = Precedence::FACTOR},
        // TOKEN_BANG
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_BANG_EQUAL
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_EQUAL
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_EQUAL_EQUAL
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_GREATER
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_GREATER_EQUAL
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_LESS
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_LESS_EQUAL
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_IDENTIFIER
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_STRING
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_NUMBER
        {.prefix = &Compiler::number, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_AND
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_CLASS
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_ELSE
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_FALSE
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_FOR
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_FUN
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_IF
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_NIL
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        //  TOKEN_OR
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_PRINT
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_RETURN
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_SUPER
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_THIS
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_TRUE
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_VAR
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_WHILE
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        //  TOKEN_ERROR
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
        // TOKEN_EOF
        {.prefix = nullptr, .infix = nullptr, .precedence = Precedence::NONE},
    }};
};
