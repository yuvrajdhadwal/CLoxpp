cmake_minimum_required(VERSION 3.20)

# Set Project Name and Version
project(CLoxpp.exe VERSION 0.1.0 LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd
set(CMAKE_BUILD_TYPE DEBUG)

add_executable(CLoxpp.exe 
    src/main.cpp
    src/chunk.cpp
    src/debug.cpp
    src/value.cpp
)

target_include_directories(CLoxpp.exe PRIVATE
    include    
)

target_compile_options(CLoxpp.exe PRIVATE 
    -Wall -Wextra -Wpedantic -Werror 
    -Wshadow -Wconversion -Wsign-conversion
)

