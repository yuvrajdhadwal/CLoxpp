#pragma once

#include <string_view>
#include <iostream>

#include "scanner.hpp"
#include "token.hpp"
#include "tokentype.hpp"

void compile(std::string_view source);
