#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <format>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <print>
#include <string>
#include <vector>

#include "chunk.hpp"
#include "compiler.hpp"

enum class InterpretResult : uint8_t {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

const std::size_t MAX_STACK{256};

class VirtualMachine {
   public:
    auto interpret(std::string_view source) -> InterpretResult;
    VirtualMachine();

   private:
    auto run() -> InterpretResult;
    void resetStack() { m_stackTop = m_stack.data(); }

    template <typename Oper>
    auto binary_op() -> InterpretResult;

    auto read_byte() -> OpCode {
        assert(m_ip < m_chunk.getCodeSize());
        return static_cast<OpCode>(m_chunk.getCode(m_ip++));
    }

    [[nodiscard]] auto static isFalsey(const Value& value) -> bool {
        return std::holds_alternative<std::monostate>(value) ||
               (std::holds_alternative<bool>(value) && !std::get<bool>(value));
    }

    auto static inline valuesEqual(const Value& first, const Value& second) -> bool;

    auto read_constant() -> Value {
        return m_chunk.getConstant(static_cast<std::size_t>(read_byte()));
    }

    auto read_long_constant() -> Value {
        const std::uint8_t constantIndex1{static_cast<std::uint8_t>(read_byte())};
        const std::uint8_t constantIndex2{static_cast<std::uint8_t>(read_byte())};
        const std::uint8_t constantIndex3{static_cast<std::uint8_t>(read_byte())};
        const std::size_t constantIndex{static_cast<size_t>(
            (constantIndex1 << 16ULL) | (constantIndex2 << 8ULL) | constantIndex3)};

        return m_chunk.getConstant(constantIndex);
    }

    void push(Value value) {
        assert(m_stackTop < m_stack.data() + MAX_STACK && "Stack Overflow.");  // NOLINT
        *m_stackTop = value;
        ++m_stackTop;  // NOLINT
    }

    auto pop() -> Value {
        assert(m_stackTop > m_stack.data() && "Stack Underflow.");
        --m_stackTop;  // NOLINT
        return *m_stackTop;
    }

    auto peek(int distance) -> Value {
        return m_stackTop[-1 - distance];  // NOLINT
    }

    template <typename... Args>
    void runtimeError(std::string_view format, Args&&... args);

    Chunk m_chunk;
    std::size_t m_ip;
    std::array<Value, MAX_STACK> m_stack;
    Value* m_stackTop;
};
