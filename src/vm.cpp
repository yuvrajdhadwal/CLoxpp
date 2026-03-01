#include "common.hpp"
#include "vm.hpp"

auto VirtualMachine::interpret(Chunk& chunk) -> InterpretResult {
    m_chunk = &chunk;
    m_ip = m_chunk->getFirstCode();
    m_stackTop = m_stack.data();

    return run();
}

auto VirtualMachine::run() -> InterpretResult {
    for (;;) {
#ifdef DEBUG_TRACE_EXEC
        std::cout << "        ";
        for (const auto& value : m_stack) {
            std::cout << "[ ";
            printValue(value);
            std::cout << " ]";
        }
        std::cout << '\n';
        m_chunk->disassembleInstruction(
                static_cast<std::size_t>(m_ip - m_chunk->getFirstCode()));
#endif
        OpCode instruction{};

        switch (instruction = read_byte()) {
            case OpCode::OP_CONSTANT:
                *m_stackTop = read_constant();
                ++m_stackTop;
                break;
            case OpCode::OP_CONSTANT_LONG:
                *m_stackTop = read_long_constant();
                ++m_stackTop;
                break;
            case OpCode::OP_NEGATE:
                *(m_stackTop - 1) *= -1.0;
                break;
            case OpCode::OP_ADD:
                binary_op<std::plus<Value>>();
                break;
            case OpCode::OP_SUBTRACT:
                binary_op<std::minus<Value>>();
                break;
            case OpCode::OP_MULTIPLY:
                binary_op<std::multiplies<Value>>();
                break;
            case OpCode::OP_DIVIDE:
                binary_op<std::divides<Value>>();
                break;
            case OpCode::OP_RETURN:
                printValue(*(--m_stackTop));
                std::cout << '\n';
                return InterpretResult::INTERPRET_OK;
            default:
                std::cout << "Cannot read this, error... continuing!\n";
                break;
                // return InterpretResult::INTERPRET_RUNTIME_ERROR;
        }
    }
}

template<typename Oper>
void VirtualMachine::binary_op() {
    Value second = *(--m_stackTop);
    Value first = *(--m_stackTop);
    Oper oper;
    *m_stackTop = oper(first, second);
    ++m_stackTop;
}
