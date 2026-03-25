#include "compiler.hpp"

void Compiler::advance() {
    m_previous = m_current;

    for (;;) {
        m_current = m_scanner.scanToken();
        if (m_current.getType() != TokenType::ERROR) {
            break;
        }

        errorAtCurrent("Failure in Advance().");
    }
}

void Compiler::consume(TokenType type, std::string_view message) {
    if (m_current.getType() == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

void Compiler::startCompile() {
    advance();
    expression();
    consume(TokenType::EOF, "Expect End of Expression.");
}

Compiler::Compiler(std::string_view source, Chunk& chunk)
    : m_chunk{chunk}, m_source{source}, m_hadError{false}, m_panicMode{false}, m_scanner{m_source} {
    startCompile();
    finishCompile();
}

void Compiler::errorAt(Token& token, std::string_view message) {
    if (m_panicMode) {
        return;
    }

    m_panicMode = true;
    std::cerr << "[line " << token.getLine() << "] Error";

    if (token.getType() == TokenType::EOF) {
        std::cerr << " at end";
    } else if (token.getType() != TokenType::ERROR) {
        std::cerr << " at " << m_source.substr(token.getStart(), token.getLength());
    }

    std::cerr << ": " << message << '\n';
    m_hadError = true;
}

void Compiler::number() {
    double value{std::strtod(&m_source[m_current.getStart()], nullptr)};
    emitConstant(value);
}

auto Compiler::makeConstant(Value value) -> uint8_t {
    int constant = m_chunk.addConstant(value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }

    return static_cast<uint8_t>(constant);
}

void Compiler::grouping() {
    expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after an expression.");
}

void Compiler::parsePrecedence(Precedence precedence) {}

void Compiler::expression() { parsePrecedence(Precedence::ASSIGNMENT); }

void Compiler::unary() {
    TokenType operatorType{m_previous.getType()};

    parsePrecedence(Precedence::UNARY);

    switch (operatorType) {
        case TokenType::MINUS:
            emitByte(OpCode::NEGATE);
            break;
        default:
            return;
    }
}
