#include "scanner.hpp"

Scanner::Scanner(std::string_view source) : m_file{source}, m_start{0}, m_current{0}, m_line{1} {}

auto Scanner::isAtEnd() -> bool { return m_current == m_file.length(); }

auto Scanner::makeToken(TokenType type) const -> Token {
    return Token{type, m_start, m_current - m_start, m_line};
}

auto Scanner::errorToken(std::string_view message) const -> Token {
    return Token{TokenType::ERROR, message, m_line};
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
    return makeToken(TokenType::STRING);
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

    return makeToken(TokenType::NUMBER);
}

auto Scanner::checkKeyword(std::size_t start, std::size_t length, std::string_view rest,
                           TokenType type) {
    if (m_current - m_start == start + length &&
        m_file.compare(m_start + start, length, rest) == 0) {
        return type;
    }

    return TokenType::IDENTIFIER;
}

auto Scanner::getIdentifierType() -> TokenType {
    switch (m_file[m_start]) {
        case 'a':
            return checkKeyword(1, 2, "nd", TokenType::AND);
        case 'c':
            return checkKeyword(1, 4, "lass", TokenType::CLASS);
        case 'e':
            return checkKeyword(1, 3, "lse", TokenType::ELSE);
        case 'i':
            return checkKeyword(1, 1, "f", TokenType::IF);
        case 'n':
            return checkKeyword(1, 2, "il", TokenType::NIL);
        case 'o':
            return checkKeyword(1, 1, "r", TokenType::OR);
        case 'p':
            return checkKeyword(1, 4, "rint", TokenType::PRINT);
        case 'r':
            return checkKeyword(1, 5, "eturn", TokenType::RETURN);  // NOLINT
        case 's':
            return checkKeyword(1, 4, "uper", TokenType::SUPER);
        case 'v':
            return checkKeyword(1, 2, "ar", TokenType::VAR);
        case 'w':
            return checkKeyword(1, 4, "hile", TokenType::WHILE);

        case 'f':
            if (m_current - m_start > 1) {
                switch (m_file[m_start + 1]) {
                    case 'a':
                        return checkKeyword(2, 3, "lse", TokenType::FALSE);
                    case 'o':
                        return checkKeyword(2, 1, "r", TokenType::FOR);
                    case 'u':
                        return checkKeyword(2, 1, "n", TokenType::FUN);
                    default:
                        break;
                }
            }
            break;

        case 't':
            if (m_current - m_start > 1) {
                switch (m_file[m_start + 1]) {
                    case 'h':
                        return checkKeyword(2, 2, "is", TokenType::THIS);
                    case 'r':
                        return checkKeyword(2, 2, "ue", TokenType::TRUE);
                    default:
                        break;
                }
            }
            break;

        default:
            break;
    }

    return TokenType::IDENTIFIER;
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
            return makeToken(TokenType::LEFT_PAREN);
        case ')':
            return makeToken(TokenType::RIGHT_PAREN);

        case '{':
            return makeToken(TokenType::LEFT_BRACE);
        case '}':
            return makeToken(TokenType::RIGHT_BRACE);

        case ';':
            return makeToken(TokenType::SEMICOLON);
        case ',':
            return makeToken(TokenType::COMMA);
        case '.':
            return makeToken(TokenType::DOT);

        case '+':
            return makeToken(TokenType::PLUS);
        case '-':
            return makeToken(TokenType::MINUS);
        case '*':
            return makeToken(TokenType::STAR);
        case '/':
            return makeToken(TokenType::SLASH);

        case '!':
            return makeToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        case '=':
            return makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        case '<':
            return makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        case '>':
            return makeToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);

        case '"':
            return getString();

        default:
            break;
    }

    return errorToken("Unexpected character.");
}
