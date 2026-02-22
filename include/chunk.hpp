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
    friend void writeChunk(Chunk& chunk, OpCode byte, int line);
    friend void writeChunk(Chunk& chunk, std::uint8_t byte, int line);
    friend void writeConstant(Chunk& chunk, Value value, int line);

    // debug.cpp
    friend void disassembleChunk(Chunk& chunk, std::string_view name);
    friend auto disassembleInstruction(Chunk& chunk, std::size_t offset) -> std::size_t;
    friend auto getLine(Chunk& chunk, std::size_t instrIndex) -> std::size_t;

    // value.cpp
    friend void writeValue(std::vector<Value> array, Value value);
    friend auto addConstant(Chunk& chunk, Value value) -> std::uint8_t;

    // inline
    [[nodiscard]] auto getCode(std::size_t index) const -> uint8_t { return code[index]; }
    [[nodiscard]] auto getConstant(std::size_t index) const -> Value { return constants[index]; }

    private:
    std::vector<uint8_t> code;
    std::vector<Value> constants;
    std::vector<std::size_t> lines;
};