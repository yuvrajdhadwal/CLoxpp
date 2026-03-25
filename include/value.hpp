#pragma once

#include <print>
#include <variant>

// std::monostate for nil
using Value = std::variant<std::monostate, bool, double>;

void printValue(const Value& value);
