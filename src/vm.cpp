#include "common.hpp"
#include "vm.hpp"

auto VirtualMachine::interpret(Chunk& chunk)
{
    m_chunk = std::make_unique<Chunk>(chunk);
    m_ip = "
}
