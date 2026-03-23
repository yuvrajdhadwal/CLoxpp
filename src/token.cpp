#include "token.hpp"

Token::Token(TokenType type, std::size_t start, std::size_t length, std::size_t line)  // NOLINT
    : m_type{type}, m_start{start}, m_length{length}, m_line{line} {}

Token::Token(TokenType type, std::string_view message, std::size_t line)
    : m_type{type}, m_line {line}, m_message {message} {}
