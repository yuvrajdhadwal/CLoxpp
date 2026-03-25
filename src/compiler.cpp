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
    consume(TokenType::TOKEN_EOF, "Expect End of Expression.");
}

Compiler::Compiler(std::string_view source, Chunk& chunk)
    : m_chunk{chunk}, m_source{source}, m_hadError{false}, m_panicMode{false}, m_scanner{m_source} {
    startCompile();
    finishCompile();

#ifdef DEBUG_PRINT_CODE
    if (!m_hadError) {
        m_chunk.disassembleChunk("code");
    }
#endif
}

void Compiler::errorAt(Token& token, std::string_view message) {
    if (m_panicMode) {
        return;
    }

    m_panicMode = true;
    std::cerr << "[line " << token.getLine() << "] Error";

    if (token.getType() == TokenType::TOKEN_EOF) {
        std::cerr << " at end";
    } else if (token.getType() != TokenType::ERROR) {
        std::cerr << " at " << m_source.substr(token.getStart(), token.getLength());
    }

    std::cerr << ": " << message << '\n';
    m_hadError = true;
}

void Compiler::number() {
    double value{std::strtod(&m_source[m_previous.getStart()], nullptr)};
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

void Compiler::parsePrecedence(Precedence precedence) {
    advance();
    auto prefixRule = getRule(m_previous.getType()).prefix;

    if (prefixRule == nullptr) {
        error("Expect expression.");
        return;
    }

    (this->*prefixRule)();

    while (precedence <= getRule(m_current.getType()).precedence) {
        advance();
        auto infixRule = getRule(m_previous.getType()).infix;
        (this->*infixRule)();
    }
}

void Compiler::expression() { parsePrecedence(Precedence::ASSIGNMENT); }

void Compiler::binary() {
    TokenType operatorType{m_previous.getType()};
    ParseRule& rule{getRule(operatorType)};
    parsePrecedence(static_cast<Precedence>(static_cast<int>(rule.precedence) + 1));

    switch (operatorType) {
        case TokenType::PLUS:
            emitByte(OpCode::ADD);
            break;
        case TokenType::MINUS:
            emitByte(OpCode::SUBTRACT);
            break;
        case TokenType::STAR:
            emitByte(OpCode::MULTIPLY);
            break;
        case TokenType::SLASH:
            emitByte(OpCode::DIVIDE);
            break;
        default:
            return;
    }
}

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
