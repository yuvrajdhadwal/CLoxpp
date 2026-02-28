#pragma once

#include <memory>
#include <vector>

#include "chunk.hpp"

enum class InterpretResult : std::uint8_t {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VirtualMachine {
   public:
    auto interpret(Chunk& chunk) -> InterpretResult;

   private:
    std::unique_ptr<Chunk> m_chunk;
    std::vector<std::uint8_t>::iterator m_ip;
};
