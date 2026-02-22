#pragma once

#include "common.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string_view>
#include <vector>

using Value = double;

enum class OpCode : std::uint8_t
{
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_RETURN
};

class Chunk
{
    public:
    Chunk() = default;

    // chunk.cpp
    void writeChunk(OpCode opcode, int line);
    void writeChunk(std::uint8_t byte, int line);
    void writeConstant(Value value, int line);

    // debug.cpp
    void disassembleChunk(std::string_view name);

    // value.cpp
    auto addConstant(Value value) -> std::uint8_t;

    // inline
    [[nodiscard]] auto getCode(std::size_t index) const -> uint8_t { return code[index]; }
    [[nodiscard]] auto getConstant(std::size_t index) const -> Value { return constants[index]; }

    private:
    // debug.cpp
    auto disassembleInstruction(std::size_t offset) -> std::size_t;
    auto getLine(std::size_t instrIndex) -> std::size_t;

    // value.cpp
    void writeValue(Value value);

    std::vector<uint8_t> code;
    std::vector<Value> constants;
    std::vector<std::size_t> lines;
};