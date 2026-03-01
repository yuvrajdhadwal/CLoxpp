#pragma once

#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "chunk.hpp"

enum class InterpretResult : uint8_t {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

const std::size_t MAX_STACK{256};

class VirtualMachine {
   public:
    auto interpret(Chunk& chunk) -> InterpretResult;

   private:
    auto run() -> InterpretResult;
    void binary_op(char oper);

    auto read_byte() -> OpCode { return static_cast<OpCode>(*m_ip++); }
    auto read_constant() -> Value {
        return m_chunk->getConstant(static_cast<std::size_t>(read_byte()));
    }
    auto read_long_constant() -> Value {
        const std::uint8_t constantIndex1{static_cast<std::uint8_t>(read_byte())};
        const std::uint8_t constantIndex2{static_cast<std::uint8_t>(read_byte())};
        const std::uint8_t constantIndex3{static_cast<std::uint8_t>(read_byte())};
        const std::size_t constantIndex{static_cast<size_t>(
            (constantIndex1 << 16ULL) | (constantIndex2 << 8ULL) | constantIndex3)};

        return m_chunk->getConstant(constantIndex);
    }

    std::unique_ptr<Chunk> m_chunk;
    std::vector<uint8_t>::iterator m_ip;
    std::vector<Value> m_stack;
};
