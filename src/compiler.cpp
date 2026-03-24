#include "compiler.hpp"

void compile(std::string_view source) {
    Scanner scanner{source};

    std::size_t line = 0;
    for (;;) {
        Token token = scanner.scanToken();
        if (token.getLine() != line) {
            std::cout << token.getLine() << '\n';
            line = token.getLine();
        }
        std::cout << "    | ";

        if (token.getMessage().empty()) {
            std::cout << token.getType() << " "
                      << scanner.getLex(token.getStart(), token.getLength()) << '\n';
        } else {
            std::cout << token.getType() << " " << token.getMessage() << '\n';
        }
        if (token.getType() == TokenType::TOKEN_EOF) {
            break;
        }
    }
}
