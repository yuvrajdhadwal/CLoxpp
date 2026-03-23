#include "scanner.hpp"

Scanner::Scanner(std::string_view source) : m_file{source}, m_start{0}, m_current{0}, m_line{1} {}

auto Scanner::isAtEnd() -> bool { return m_current == m_file.length(); }

auto Scanner::makeToken(TokenType type) const -> Token {
    return Token{type, m_start, m_current - m_start, m_line};
}

auto Scanner::errorToken(std::string_view message) const -> Token {
    return Token{TokenType::TOKEN_ERROR, message, m_line};
}

auto inline Scanner::match(char expected) -> bool {
    if (isAtEnd()) {
        return false;
    }
    if (m_file[m_current] != expected) {
        return false;
    }

    ++m_current;
    return true;
}

void Scanner::skipWhitespace() {
    for (;;) {
        char curr_c{peek()};

        switch (curr_c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                ++m_line;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

auto Scanner::getString() -> Token {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            ++m_line;
        }
        advance();
    }

    if (isAtEnd()) {
        return errorToken("Unterminated String.");
    }

    advance();
    return makeToken(TokenType::TOKEN_STRING);
}

static auto inline isDigit(char curr_c) { return curr_c >= '0' && curr_c <= '9'; }
static auto inline isAlpha(char curr_c) {
    return (curr_c >= 'a' && curr_c <= 'z') || (curr_c >= 'A' && curr_c <= 'Z') || curr_c == '_';
}

auto Scanner::getNumber() -> Token {
    while (isDigit(peek())) {
        advance();
    }

    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) {
            advance();
        }
    }

    return makeToken(TokenType::TOKEN_NUMBER);
}

auto Scanner::checkKeyword(std::size_t start, std::size_t length, std::string_view rest,
                           TokenType type) {
    if (m_current - m_start == start + length &&
        m_file.compare(m_start + start, length, rest) == 0) {
        return type;
    }

    return TokenType::TOKEN_IDENTIFIER;
}

auto Scanner::getIdentifierType() -> TokenType {
    switch (m_file[m_start]) {
        case 'a':
            return checkKeyword(1, 2, "nd", TokenType::TOKEN_AND);
        case 'c':
            return checkKeyword(1, 4, "lass", TokenType::TOKEN_CLASS);
        case 'e':
            return checkKeyword(1, 3, "lse", TokenType::TOKEN_ELSE);
        case 'i':
            return checkKeyword(1, 1, "f", TokenType::TOKEN_IF);
        case 'n':
            return checkKeyword(1, 2, "il", TokenType::TOKEN_NIL);
        case 'o':
            return checkKeyword(1, 1, "r", TokenType::TOKEN_OR);
        case 'p':
            return checkKeyword(1, 4, "rint", TokenType::TOKEN_PRINT);
        case 'r':
            return checkKeyword(1, 5, "eturn", TokenType::TOKEN_RETURN);  // NOLINT
        case 's':
            return checkKeyword(1, 4, "uper", TokenType::TOKEN_SUPER);
        case 'v':
            return checkKeyword(1, 2, "ar", TokenType::TOKEN_VAR);
        case 'w':
            return checkKeyword(1, 4, "hile", TokenType::TOKEN_WHILE);

        case 'f':
            if (m_current - m_start > 1) {
                switch (m_file[m_start + 1]) {
                    case 'a':
                        return checkKeyword(2, 3, "lse", TokenType::TOKEN_FALSE);
                    case 'o':
                        return checkKeyword(2, 1, "r", TokenType::TOKEN_FOR);
                    case 'u':
                        return checkKeyword(2, 1, "n", TokenType::TOKEN_FUN);
                    default:
                        break;
                }
            }
            break;

        case 't':
            if (m_current - m_start > 1) {
                switch (m_file[m_start + 1]) {
                    case 'h':
                        return checkKeyword(2, 2, "is", TokenType::TOKEN_THIS);
                    case 'r':
                        return checkKeyword(2, 2, "ue", TokenType::TOKEN_TRUE);
                    default:
                        break;
                }
            }
            break;

        default:
            break;
    }

    return TokenType::TOKEN_IDENTIFIER;
}

auto Scanner::getIdentifier() -> Token {
    while (isAlpha(peek()) || isDigit(peek())) {
        advance();
    }
    return makeToken(getIdentifierType());
}

auto Scanner::scanToken() -> Token {
    skipWhitespace();
    m_start = m_current;

    if (isAtEnd()) {
        return makeToken(TokenType::TOKEN_EOF);
    }

    char curr_c{advance()};

    if (isAlpha(curr_c)) {
        return getIdentifier();
    }

    if (isDigit(curr_c)) {
        return getNumber();
    }

    switch (curr_c) {
        case '(':
            return makeToken(TokenType::TOKEN_LEFT_PAREN);
        case ')':
            return makeToken(TokenType::TOKEN_RIGHT_PAREN);

        case '{':
            return makeToken(TokenType::TOKEN_LEFT_BRACE);
        case '}':
            return makeToken(TokenType::TOKEN_RIGHT_BRACE);

        case ';':
            return makeToken(TokenType::TOKEN_SEMICOLON);
        case ',':
            return makeToken(TokenType::TOKEN_COMMA);
        case '.':
            return makeToken(TokenType::TOKEN_DOT);

        case '+':
            return makeToken(TokenType::TOKEN_PLUS);
        case '-':
            return makeToken(TokenType::TOKEN_MINUS);
        case '*':
            return makeToken(TokenType::TOKEN_STAR);
        case '/':
            return makeToken(TokenType::TOKEN_SLASH);

        case '!':
            return makeToken(match('=') ? TokenType::TOKEN_BANG_EQUAL : TokenType::TOKEN_BANG);
        case '=':
            return makeToken(match('=') ? TokenType::TOKEN_EQUAL_EQUAL : TokenType::TOKEN_EQUAL);
        case '<':
            return makeToken(match('=') ? TokenType::TOKEN_LESS_EQUAL : TokenType::TOKEN_LESS);
        case '>':
            return makeToken(match('=') ? TokenType::TOKEN_GREATER_EQUAL
                                        : TokenType::TOKEN_GREATER);

        case '"':
            return getString();

        default:
            break;
    }

    return errorToken("Unexpected character.");
}
