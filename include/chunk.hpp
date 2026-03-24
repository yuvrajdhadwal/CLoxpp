#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

#include "common.hpp"

using Value = double;

enum class OpCode : std::uint8_t {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN
};

struct LineEntry {
    std::size_t endOffset;
    std::size_t line;
};

void printValue(Value value);

class Chunk {
   public:
    Chunk() = default;

    // chunk.cpp
    void writeChunk(OpCode opcode, std::size_t line);
    void writeChunk(std::uint8_t byte, std::size_t line);
    void writeConstant(Value value, std::size_t line);

    // debug.cpp
    void disassembleChunk(std::string_view name) const;
    auto disassembleInstruction(std::size_t offset) const -> std::size_t;  // NOLINT

    // value.cpp
    auto addConstant(Value value) -> std::uint8_t;

    // inline
    [[nodiscard]] auto getCode(std::size_t index) const -> uint8_t { return m_code[index]; }
    [[nodiscard]] auto getConstant(std::size_t index) const -> Value { return m_constants[index]; }
    [[nodiscard]] auto getFirstCode() const -> const uint8_t* { return m_code.data(); }
    [[nodiscard]] auto getCodeSize() const -> std::size_t { return m_code.size(); }

   private:
    // debug.cpp
    [[nodiscard]] auto getLine(std::size_t instrIndex) const -> std::size_t;

    // value.cpp
    void writeValue(Value value);

    std::vector<uint8_t> m_code;
    std::vector<Value> m_constants;
    std::vector<LineEntry> m_lines;
};
