#include <expected>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include "chunk.hpp"
#include "common.hpp"
#include "vm.hpp"

enum class ErrorCodes : uint8_t {
    FILE_PATH_ERROR = 64,
    COMPILE_ERROR = 65,
    RUNTIME_ERROR = 70,
    FILE_ERROR = 74,
};

static auto readFile(std::string_view path) -> std::expected<std::string, ErrorCodes> {
    std::ifstream file(static_cast<std::string>(path));

    if (!file.is_open()) {
        return std::unexpected(ErrorCodes::FILE_ERROR);
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    std::string output = oss.str();

    return output;
}

static auto runFile(std::string_view path, VirtualMachine& virtm)
    -> std::expected<void, ErrorCodes> {
    const auto source{readFile(path)};

    if (!source) {
        return std::unexpected(source.error());
    }

    InterpretResult result = virtm.interpret(*source);

    if (result != InterpretResult::INTERPRET_OK) {
        return std::unexpected(result == InterpretResult::INTERPRET_COMPILE_ERROR
                                   ? ErrorCodes::COMPILE_ERROR
                                   : ErrorCodes::RUNTIME_ERROR);
    }

    return {};
}

auto main(int argc, const char* argv[]) -> int {
    VirtualMachine virtm{};

    if (argc == 2) {
        const auto fileOut = runFile(argv[1], virtm);  // NOLINT

        if (!fileOut) {
            return static_cast<int>(fileOut.error());
        }
    } else {
        std::cerr << "Usage: Clox [path]\n";
        return static_cast<int>(ErrorCodes::FILE_PATH_ERROR);
    }

    return 0;
}
