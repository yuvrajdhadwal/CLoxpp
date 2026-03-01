#include "common.hpp"
#include "vm.hpp"

auto VirtualMachine::interpret(Chunk& chunk) -> InterpretResult {
    m_chunk = std::make_unique<Chunk>(chunk);
    m_ip = m_chunk->getCodeIter();
    m_stack.reserve(MAX_STACK);

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
            static_cast<std::size_t>(std::distance(m_chunk->getCodeIter(), m_ip)));
#endif
        OpCode instruction{};

        switch (instruction = read_byte()) {
            case OpCode::OP_CONSTANT:
                m_stack.push_back(read_constant());
                break;
            case OpCode::OP_CONSTANT_LONG:
                m_stack.push_back(read_long_constant());
                break;
            case OpCode::OP_NEGATE:
                m_stack[m_stack.size() - 1] *= -1;
                break;
            case OpCode::OP_ADD:
                binary_op('+');
                break;
            case OpCode::OP_SUBTRACT:
                binary_op('-');
                break;
            case OpCode::OP_MULTIPLY:
                binary_op('*');
                break;
            case OpCode::OP_DIVIDE:
                binary_op('/');
                break;
            case OpCode::OP_RETURN:
                printValue(m_stack.back());
                m_stack.pop_back();
                std::cout << '\n';
                return InterpretResult::INTERPRET_OK;
            default:
                std::cout << "Cannot read this, error... continuing!\n";
                break;
                // return InterpretResult::INTERPRET_RUNTIME_ERROR;
        }
    }
}

void VirtualMachine::binary_op(char oper) {
    Value second = m_stack.back();
    m_stack.pop_back();
    Value first = m_stack.back();
    m_stack.pop_back();
    switch (oper) {
        case '+':
            m_stack.push_back(first + second);
            break;
        case '-':
            m_stack.push_back(first - second);
            break;
        case '*':
            m_stack.push_back(first * second);
            break;
        case '/':
            m_stack.push_back(first / second);
            break;
        default:
            return;
    }
}
