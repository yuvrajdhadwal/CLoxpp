#pragma once

#include <string_view>

#include "tokentype.hpp"

class Token {
   public:
    Token(TokenType type, std::size_t start, std::size_t length, std::size_t line);
    Token(TokenType type, std::string_view message, std::size_t line);

    [[nodiscard]] auto getType() const -> TokenType { return m_type; }
    [[nodiscard]] auto getStart() const -> std::size_t { return m_start; }
    [[nodiscard]] auto getLength() const -> std::size_t { return m_length; }
    [[nodiscard]] auto getLine() const -> std::size_t { return m_line; }
    [[nodiscard]] auto getMessage() const -> std::string_view { return m_message; }

   private:
    TokenType m_type{};
    std::size_t m_start{};
    std::size_t m_length{};
    std::size_t m_line{};

    std::string_view m_message;
};
