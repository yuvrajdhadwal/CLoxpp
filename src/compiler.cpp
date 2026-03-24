#include "compiler.hpp"

void Compiler::advance(Scanner& scanner) {
    m_previous = m_current;

    for (;;) {
        m_current = scanner.scanToken();
        if (m_current.getType() != TokenType::TOKEN_ERROR) {
            break;
        }

        errorAtCurrent("Failure in Advance().");
    }
}

void Compiler::consume(TokenType type, std::string_view message, Scanner& scanner) {
    if (m_current.getType() == type) {
        advance(scanner);
        return;
    }

    errorAtCurrent(message);
}

void Compiler::startCompile() {
    Scanner scanner{m_source};

    advance(scanner);
    expression();
    consume(TokenType::TOKEN_EOF, "Expect End of Expression.", scanner);
}

Compiler::Compiler(std::string_view source, Chunk& chunk)
    : m_chunk{chunk}, m_source{source}, m_hadError{false}, m_panicMode{false} {
    startCompile();
    finishCompile();
}

void Compiler::errorAt(Token& token, std::string_view message) {
    if (m_panicMode) {
        return;
    }

    m_panicMode = true;
    std::cerr << "[line " << token.getLine() << "] Error";

    if (token.getType() == TokenType::TOKEN_EOF) {
        std::cerr << " at end";
    } else if (token.getType() != TokenType::TOKEN_ERROR) {
        std::cerr << " at " << m_source.substr(token.getStart(), token.getLength());
    }

    std::cerr << ": " << message << '\n';
    m_hadError = true;
}
