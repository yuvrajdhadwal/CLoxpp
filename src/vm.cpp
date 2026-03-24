#include "vm.hpp"

VirtualMachine::VirtualMachine() : m_ip {0}, m_stack {}, m_stackTop {m_stack.data()} {}

auto VirtualMachine::interpret(std::string_view source) -> InterpretResult {  // NOLINT
    Chunk chunk{};

    Compiler compiler {source, chunk};

    if (!compiler.isOK())
    {
        return InterpretResult::INTERPRET_COMPILE_ERROR;
    }

    m_chunk = std::move(chunk);
    m_ip = 0;

    return run();
}

auto VirtualMachine::run() -> InterpretResult {
    for (;;) {
#ifdef DEBUG_TRACE_EXEC
        std::cout << "        ";
        for (Value* i{m_stack.data()}; i < m_stackTop; ++i) {  // NOLINT
            std::cout << "[ ";
            printValue(*i);
            std::cout << " ]";
        }
        std::cout << '\n';
        m_chunk.disassembleInstruction(m_ip);
#endif
        OpCode instruction{};

        switch (instruction = read_byte()) {
            case OpCode::OP_CONSTANT:
                push(read_constant());
                break;
            case OpCode::OP_CONSTANT_LONG:
                push(read_long_constant());
                break;
            case OpCode::OP_NEGATE:
                assert(m_stackTop > m_stack.data());
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
                printValue(pop());
                std::cout << '\n';
                return InterpretResult::INTERPRET_OK;
            default:
                std::cout << "Cannot read this, error... continuing!\n";
                break;
                // return InterpretResult::INTERPRET_RUNTIME_ERROR;
        }
    }
}

template <typename Oper>
void VirtualMachine::binary_op() {
    Oper oper;

    assert(m_stackTop >= &m_stack[2]);

    *(m_stackTop - 2) = oper(*(m_stackTop - 2), *(m_stackTop - 1));  // NOLINT
    --m_stackTop;                                                    // NOLINT
}
