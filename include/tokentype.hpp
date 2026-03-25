#pragma once

#include <cstdint>
#include <iostream>

enum class TokenType : uint8_t {
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,
    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FOR,
    FUN,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    ERROR,
    TOKEN_EOF
};

auto inline operator<<(std::ostream& out, const TokenType& token) -> std::ostream& {
    switch (token) {
        case TokenType::LEFT_PAREN:
            out << "TOKEN_LEFT_PAREN";
            break;
        case TokenType::RIGHT_PAREN:
            out << "TOKEN_RIGHT_PAREN";
            break;
        case TokenType::LEFT_BRACE:
            out << "TOKEN_LEFT_BRACE";
            break;
        case TokenType::RIGHT_BRACE:
            out << "TOKEN_RIGHT_BRACE";
            break;
        case TokenType::COMMA:
            out << "TOKEN_COMMA";
            break;
        case TokenType::DOT:
            out << "TOKEN_DOT";
            break;
        case TokenType::MINUS:
            out << "TOKEN_MINUS";
            break;
        case TokenType::PLUS:
            out << "TOKEN_PLUS";
            break;
        case TokenType::SEMICOLON:
            out << "TOKEN_SEMICOLON";
            break;
        case TokenType::SLASH:
            out << "TOKEN_SLASH";
            break;
        case TokenType::STAR:
            out << "TOKEN_STAR";
            break;
        case TokenType::BANG:
            out << "TOKEN_BANG";
            break;
        case TokenType::BANG_EQUAL:
            out << "TOKEN_BANG_EQUAL";
            break;
        case TokenType::EQUAL:
            out << "TOKEN_EQUAL";
            break;
        case TokenType::EQUAL_EQUAL:
            out << "TOKEN_EQUAL_EQUAL";
            break;
        case TokenType::GREATER:
            out << "TOKEN_GREATER";
            break;
        case TokenType::GREATER_EQUAL:
            out << "TOKEN_GREATER_EQUAL";
            break;
        case TokenType::LESS:
            out << "TOKEN_LESS";
            break;
        case TokenType::LESS_EQUAL:
            out << "TOKEN_LESS_EQUAL";
            break;
        case TokenType::IDENTIFIER:
            out << "TOKEN_IDENTIFIER";
            break;
        case TokenType::STRING:
            out << "TOKEN_STRING";
            break;
        case TokenType::NUMBER:
            out << "TOKEN_NUMBER";
            break;
        case TokenType::AND:
            out << "TOKEN_AND";
            break;
        case TokenType::CLASS:
            out << "TOKEN_CLASS";
            break;
        case TokenType::ELSE:
            out << "TOKEN_ELSE";
            break;
        case TokenType::FALSE:
            out << "TOKEN_FALSE";
            break;
        case TokenType::FOR:
            out << "TOKEN_FOR";
            break;
        case TokenType::FUN:
            out << "TOKEN_FUN";
            break;
        case TokenType::IF:
            out << "TOKEN_IF";
            break;
        case TokenType::NIL:
            out << "TOKEN_NIL";
            break;
        case TokenType::OR:
            out << "TOKEN_OR";
            break;
        case TokenType::PRINT:
            out << "TOKEN_PRINT";
            break;
        case TokenType::RETURN:
            out << "TOKEN_RETURN";
            break;
        case TokenType::SUPER:
            out << "TOKEN_SUPER";
            break;
        case TokenType::THIS:
            out << "TOKEN_THIS";
            break;
        case TokenType::TRUE:
            out << "TOKEN_TRUE";
            break;
        case TokenType::VAR:
            out << "TOKEN_VAR";
            break;
        case TokenType::WHILE:
            out << "TOKEN_WHILE";
            break;
        case TokenType::ERROR:
            out << "TOKEN_ERROR";
            break;
        case TokenType::TOKEN_EOF:
            out << "TOKEN_EOF";
            break;
    }

    return out;
}
