#include "vm.hpp"

VirtualMachine::VirtualMachine() : m_ip{0}, m_stack{}, m_stackTop{m_stack.data()} {}

auto VirtualMachine::interpret(std::string_view source) -> InterpretResult {  // NOLINT
    Chunk chunk{};

    Compiler compiler{source, chunk};

    if (!compiler.isOK()) {
        return InterpretResult::INTERPRET_COMPILE_ERROR;
    }

    m_chunk = std::move(chunk);
    m_ip = 0;
    m_stackTop = m_stack.data();

    return run();
}

auto VirtualMachine::run() -> InterpretResult {  // NOLINT
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
            case OpCode::CONSTANT:
                push(read_constant());
                break;
            case OpCode::CONSTANT_LONG:
                push(read_long_constant());
                break;
            case OpCode::NIL:
                push(std::monostate());
                break;
            case OpCode::TRUE:
                push(true);
                break;
            case OpCode::FALSE:
                push(false);
                break;
            case OpCode::EQUAL: {
                Value second_val{pop()};
                Value first_val{pop()};
                push(valuesEqual(first_val, second_val));
                break;
            }
            case OpCode::NEGATE:
                assert(m_stackTop > m_stack.data());

                if (!std::holds_alternative<double>(peek(0))) {
                    runtimeError("Operand must be a number.");
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                }

                push(-std::get<double>(pop()));
                break;
            case OpCode::GREATER:
                if (binary_op<std::greater<double>>() != InterpretResult::INTERPRET_OK) {
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                };
                break;
            case OpCode::LESS:
                if (binary_op<std::less<double>>() != InterpretResult::INTERPRET_OK) {
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                }
                break;
            case OpCode::ADD:
                if (binary_op<std::plus<double>>() != InterpretResult::INTERPRET_OK) {
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                }
                break;
            case OpCode::SUBTRACT:
                if (binary_op<std::minus<double>>() != InterpretResult::INTERPRET_OK) {
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                }
                break;
            case OpCode::MULTIPLY:
                if (binary_op<std::multiplies<double>>() != InterpretResult::INTERPRET_OK) {
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                }
                break;
            case OpCode::DIVIDE:
                if (binary_op<std::divides<double>>() != InterpretResult::INTERPRET_OK) {
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                }
                break;
            case OpCode::NOT:
                push(isFalsey(pop()));
                break;
            case OpCode::RETURN:
                printValue(pop());
                std::cout << '\n';
                return InterpretResult::INTERPRET_OK;
            default:
                std::cout << "Cannot read this, error... continuing!\n";
                return InterpretResult::INTERPRET_RUNTIME_ERROR;
        }
    }
}

template <typename Oper>
auto VirtualMachine::binary_op() -> InterpretResult {
    Oper oper;

    assert(m_stackTop >= &m_stack[2]);
    if (!std::holds_alternative<double>(peek(0)) || !std::holds_alternative<double>(peek(1))) {
        runtimeError("Operands must be numbers.");
        return InterpretResult::INTERPRET_RUNTIME_ERROR;
    }

    double second_val = std::get<double>(pop());
    double first_val = std::get<double>(pop());

    push(oper(first_val, second_val));
    return InterpretResult::INTERPRET_OK;
}

template <typename... Args>
void VirtualMachine::runtimeError(std::string_view format, Args&&... args) {
    std::string message = std::vformat(format, std::make_format_args(std::forward<Args>(args)...));
    std::println(stderr, "{}", message);

    std::size_t instruction{m_ip - 1};
    std::size_t line{m_chunk.getLine(instruction)};

    std::println(stderr, "[line {}] in script", line);
    resetStack();
}

auto VirtualMachine::valuesEqual(const Value& first, const Value& second) -> bool {
    return first == second;
}
