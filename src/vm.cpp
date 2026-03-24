#include "vm.hpp"

auto VirtualMachine::interpret(std::string_view source) -> InterpretResult {  // NOLINT
    compile(source);

    return InterpretResult::INTERPRET_OK;
}

auto VirtualMachine::run() -> InterpretResult {
    for (;;) {
#ifdef DEBUG_TRACE_EXEC
        std::cout << "        ";
        for (Value* i {m_stack.data()}; i < m_stackTop; ++i) {  // NOLINT
            std::cout << "[ ";
            printValue(*i);
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
                ++m_stackTop;  // NOLINT
                break;
            case OpCode::OP_CONSTANT_LONG:
                *m_stackTop = read_long_constant();
                ++m_stackTop;  // NOLINT
                break;
            case OpCode::OP_NEGATE:
                *(m_stackTop - 1) *= -1.0;  // NOLINT
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
                printValue(*(--m_stackTop));  // NOLINT
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
    Oper oper;
    *(m_stackTop - 2) = oper(*(m_stackTop - 2), *(m_stackTop - 1));  // NOLINT
    --m_stackTop;  // NOLINT
}
