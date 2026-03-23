#include "common.hpp"
#include "chunk.hpp"
#include "vm.hpp"

#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <iostream>

enum class ErrorCodes : uint8_t
{
    FILE_ERROR = 74,
    FILE_PATH_ERROR = 64,
    COMPILE_ERROR = 65,
    RUNTIME_ERROR = 70,
};

static void repl(VirtualMachine& virtm)
{
    std::string line;
    std::cin >> line;
    virtm.interpret(line);
}

static auto readFile(std::string_view path) -> std::string
{
    std::ifstream file (static_cast<std::string>(path));

    if (!file.is_open())
    {
        std::cerr << "Failed to open file \"" << path << "\".\n";
        exit(static_cast<int>(ErrorCodes::FILE_ERROR));
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    std::string output = oss.str();

    return output;
}

static void runFile(std::string_view path, VirtualMachine& virtm)
{
    std::string source = readFile(path);
    InterpretResult result = virtm.interpret(source);

    if (result == InterpretResult::INTERPRET_COMPILE_ERROR) { exit(static_cast<int>(ErrorCodes::COMPILE_ERROR)); }
    if (result == InterpretResult::INTERPRET_RUNTIME_ERROR) { exit(static_cast<int>(ErrorCodes::RUNTIME_ERROR)); }
}

auto main(int argc, const char* argv[]) -> int
{
    VirtualMachine virtm{};

    if (argc == 1)
    {
        repl(virtm);
    } else if (argc == 2)
    {
        runFile(argv[1], virtm);  // NOLINT
    } else 
{
        std::cerr << "Usage: Clox [path]\n";
        exit(static_cast<int>(ErrorCodes::FILE_PATH_ERROR));
    }

    return 0;
}
