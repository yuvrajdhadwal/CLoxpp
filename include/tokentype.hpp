#pragma once

#include <cstdint>
#include <iostream>

enum class TokenType : uint8_t {
    // Single-character tokens.
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,
    // One or two character tokens.
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    // Literals.
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,
    // Keywords.
    TOKEN_AND,
    TOKEN_CLASS,
    TOKEN_ELSE,
    TOKEN_FALSE,
    TOKEN_FOR,
    TOKEN_FUN,
    TOKEN_IF,
    TOKEN_NIL,
    TOKEN_OR,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_SUPER,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VAR,
    TOKEN_WHILE,

    TOKEN_ERROR,
    TOKEN_EOF
};

auto inline operator<<(std::ostream& out, const TokenType& token) -> std::ostream& {
    switch (token) {
        case TokenType::TOKEN_LEFT_PAREN:
            out << "TOKEN_LEFT_PAREN";
            break;
        case TokenType::TOKEN_RIGHT_PAREN:
            out << "TOKEN_RIGHT_PAREN";
            break;
        case TokenType::TOKEN_LEFT_BRACE:
            out << "TOKEN_LEFT_BRACE";
            break;
        case TokenType::TOKEN_RIGHT_BRACE:
            out << "TOKEN_RIGHT_BRACE";
            break;
        case TokenType::TOKEN_COMMA:
            out << "TOKEN_COMMA";
            break;
        case TokenType::TOKEN_DOT:
            out << "TOKEN_DOT";
            break;
        case TokenType::TOKEN_MINUS:
            out << "TOKEN_MINUS";
            break;
        case TokenType::TOKEN_PLUS:
            out << "TOKEN_PLUS";
            break;
        case TokenType::TOKEN_SEMICOLON:
            out << "TOKEN_SEMICOLON";
            break;
        case TokenType::TOKEN_SLASH:
            out << "TOKEN_SLASH";
            break;
        case TokenType::TOKEN_STAR:
            out << "TOKEN_STAR";
            break;
        case TokenType::TOKEN_BANG:
            out << "TOKEN_BANG";
            break;
        case TokenType::TOKEN_BANG_EQUAL:
            out << "TOKEN_BANG_EQUAL";
            break;
        case TokenType::TOKEN_EQUAL:
            out << "TOKEN_EQUAL";
            break;
        case TokenType::TOKEN_EQUAL_EQUAL:
            out << "TOKEN_EQUAL_EQUAL";
            break;
        case TokenType::TOKEN_GREATER:
            out << "TOKEN_GREATER";
            break;
        case TokenType::TOKEN_GREATER_EQUAL:
            out << "TOKEN_GREATER_EQUAL";
            break;
        case TokenType::TOKEN_LESS:
            out << "TOKEN_LESS";
            break;
        case TokenType::TOKEN_LESS_EQUAL:
            out << "TOKEN_LESS_EQUAL";
            break;
        case TokenType::TOKEN_IDENTIFIER:
            out << "TOKEN_IDENTIFIER";
            break;
        case TokenType::TOKEN_STRING:
            out << "TOKEN_STRING";
            break;
        case TokenType::TOKEN_NUMBER:
            out << "TOKEN_NUMBER";
            break;
        case TokenType::TOKEN_AND:
            out << "TOKEN_AND";
            break;
        case TokenType::TOKEN_CLASS:
            out << "TOKEN_CLASS";
            break;
        case TokenType::TOKEN_ELSE:
            out << "TOKEN_ELSE";
            break;
        case TokenType::TOKEN_FALSE:
            out << "TOKEN_FALSE";
            break;
        case TokenType::TOKEN_FOR:
            out << "TOKEN_FOR";
            break;
        case TokenType::TOKEN_FUN:
            out << "TOKEN_FUN";
            break;
        case TokenType::TOKEN_IF:
            out << "TOKEN_IF";
            break;
        case TokenType::TOKEN_NIL:
            out << "TOKEN_NIL";
            break;
        case TokenType::TOKEN_OR:
            out << "TOKEN_OR";
            break;
        case TokenType::TOKEN_PRINT:
            out << "TOKEN_PRINT";
            break;
        case TokenType::TOKEN_RETURN:
            out << "TOKEN_RETURN";
            break;
        case TokenType::TOKEN_SUPER:
            out << "TOKEN_SUPER";
            break;
        case TokenType::TOKEN_THIS:
            out << "TOKEN_THIS";
            break;
        case TokenType::TOKEN_TRUE:
            out << "TOKEN_TRUE";
            break;
        case TokenType::TOKEN_VAR:
            out << "TOKEN_VAR";
            break;
        case TokenType::TOKEN_WHILE:
            out << "TOKEN_WHILE";
            break;
        case TokenType::TOKEN_ERROR:
            out << "TOKEN_ERROR";
            break;
        case TokenType::TOKEN_EOF:
            out << "TOKEN_EOF";
            break;
    }

    return out;
}
