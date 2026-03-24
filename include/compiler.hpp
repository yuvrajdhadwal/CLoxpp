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
    void advance(Scanner& scanner);
	void consume(TokenType type, std::string_view message, Scanner& scanner);

    void errorAtCurrent(std::string_view message) { errorAt(m_current, message); }
    void error(std::string_view message) { errorAt(m_previous, message); }
    void errorAt(Token& token, std::string_view message);

    Token m_current;
    Token m_previous;
    std::string_view m_source;
    bool m_hadError;
    bool m_panicMode;
};
