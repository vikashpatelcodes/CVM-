#include "vm.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void VM::push(Value v) {
    stack.push_back(v);
}

Value VM::pop() {
    if (stack.empty())
        throw runtime_error("Stack underflow!");
    Value v = stack.back();
    stack.pop_back();
    return v;
}

void VM::printValue(const Value& v) {
    if (v.isBool)
        cout << (v.boolVal ? "true" : "false") << "\n";
    else
        cout << v.intVal << "\n";
}

void VM::execute(const vector<Instruction>& code) {
    int ip = 0;

    while (ip < (int)code.size()) {
        const Instruction& instr = code[ip];

        switch (instr.op) {

            case OpCode::PUSH: {
                push(Value(instr.operand));
                ip++;
                break;
            }

            case OpCode::PUSH_BOOL: {
                push(Value(instr.operand == 1));
                ip++;
                break;
            }

            case OpCode::LOAD: {
                if (vars.find(instr.name) == vars.end())
                    throw runtime_error("Undefined variable: " + instr.name);
                push(vars[instr.name]);
                ip++;
                break;
            }

            case OpCode::STORE: {
                vars[instr.name] = pop();
                ip++;
                break;
            }

            case OpCode::ADD: {
                int b = pop().intVal;
                int a = pop().intVal;
                push(Value(a + b));
                ip++;
                break;
            }

            case OpCode::SUB: {
                int b = pop().intVal;
                int a = pop().intVal;
                push(Value(a - b));
                ip++;
                break;
            }

            case OpCode::MUL: {
                int b = pop().intVal;
                int a = pop().intVal;
                push(Value(a * b));
                ip++;
                break;
            }

            case OpCode::DIV: {
                int b = pop().intVal;
                int a = pop().intVal;
                if (b == 0) throw runtime_error("Division by zero!");
                push(Value(a / b));
                ip++;
                break;
            }

            case OpCode::EQ: {
                Value b = pop();
                Value a = pop();
                push(Value(a == b));
                ip++;
                break;
            }

            case OpCode::LESS: {
                int b = pop().intVal;
                int a = pop().intVal;
                push(Value(a < b));
                ip++;
                break;
            }

            case OpCode::GREATER: {
                int b = pop().intVal;
                int a = pop().intVal;
                push(Value(a > b));
                ip++;
                break;
            }

            case OpCode::PRINT: {
                printValue(pop());
                ip++;
                break;
            }

            case OpCode::INPUT: {
                int val;
                cin >> val;
                push(Value(val));
                ip++;
                break;
            }

            case OpCode::JUMP: {
                ip = instr.operand;
                break;
            }

            case OpCode::JUMP_IF_FALSE: {
                Value cond = pop();
                if (!cond.isTrue())
                    ip = instr.operand;
                else
                    ip++;
                break;
            }

            case OpCode::POP: {
                pop();
                ip++;
                break;
            }

            case OpCode::HALT: {
                return;
            }

            default:
                throw runtime_error("Unknown opcode!");
        }
    }
}