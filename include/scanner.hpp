#pragma once

#include <string_view>

#include "token.hpp"

class Scanner {
   public:
    Scanner(std::string_view source);

    [[nodiscard]] auto scanToken() -> Token;
    auto getLex(std::size_t start, std::size_t length) { return m_file.substr(start, length); }

   private:
    auto isAtEnd() -> bool;
    [[nodiscard]] auto makeToken(TokenType type) const -> Token;
    [[nodiscard]] auto errorToken(std::string_view message) const -> Token;
    void skipWhitespace();
    auto getString() -> Token;
    auto getNumber() -> Token;
    auto getIdentifier() -> Token;
    auto getIdentifierType() -> TokenType;
    auto checkKeyword(std::size_t start, std::size_t length, std::string_view rest, TokenType type);

    auto advance() -> char { return m_file[m_current++]; }
    auto inline match(char expected) -> bool;
    auto peek() -> char {
        if (isAtEnd()) {
            return '\0';
        }
        return m_file[m_current];
    }
    auto peekNext() -> char {
        if (isAtEnd()) {
            return '\0';
        }
        return m_file[m_current + 1];
    }

    std::string_view m_file;

    std::size_t m_start;
    std::size_t m_current;
    std::size_t m_line;
};
