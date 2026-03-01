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
    int line;
};

void printValue(Value value);

class Chunk {
   public:
    Chunk() = default;

    // chunk.cpp
    void writeChunk(OpCode opcode, int line);
    void writeChunk(std::uint8_t byte, int line);
    void writeConstant(Value value, int line);

    // debug.cpp
    void disassembleChunk(std::string_view name) const;
    auto disassembleInstruction(std::size_t offset) const -> std::size_t;

    // value.cpp
    auto addConstant(Value value) -> std::uint8_t;

    // inline
    [[nodiscard]] auto getCode(std::size_t index) const -> uint8_t { return code[index]; }
    [[nodiscard]] auto getConstant(std::size_t index) const -> Value { return constants[index]; }
    [[nodiscard]] auto getFirstCode() const -> const uint8_t* { return code.data(); }

   private:
    // debug.cpp
    [[nodiscard]] auto getLine(std::size_t instrIndex) const -> std::size_t;

    // value.cpp
    void writeValue(Value value);

    std::vector<uint8_t> code;
    std::vector<Value> constants;
    std::vector<LineEntry> lines;
};
