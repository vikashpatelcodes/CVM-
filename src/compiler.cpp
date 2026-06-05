#include "compiler.h"
#include <iostream>
#include <stdexcept>

using namespace std;

int Compiler::emit(Instruction instr) {
    code.push_back(instr);
    return code.size() - 1;
}

void Compiler::patch(int idx, int target) {
    code[idx].operand = target;
}

vector<Instruction> Compiler::compile(BlockNode* program) {
    code.clear();
    compileBlock(program);
    emit(Instruction(OpCode::HALT));
    return code;
}

void Compiler::compileBlock(BlockNode* node) {
    for (auto& stmt : node->statements)
        compileNode(stmt.get());
}

void Compiler::compileNode(ASTNode* node) {
    switch (node->type) {

        case NodeType::Number: {
            auto* n = static_cast<NumberNode*>(node);
            emit(Instruction(OpCode::PUSH, n->value));
            break;
        }

        case NodeType::Boolean: {
            auto* n = static_cast<BooleanNode*>(node);
            emit(Instruction(OpCode::PUSH_BOOL, n->value ? 1 : 0));
            break;
        }

        case NodeType::Identifier: {
            auto* n = static_cast<IdentifierNode*>(node);
            emit(Instruction(OpCode::LOAD, n->name));
            break;
        }

        case NodeType::BinaryOp: {
            auto* n = static_cast<BinaryOpNode*>(node);
            compileNode(n->left.get());
            compileNode(n->right.get());
            if      (n->op == "+")  emit(Instruction(OpCode::ADD));
            else if (n->op == "-")  emit(Instruction(OpCode::SUB));
            else if (n->op == "*")  emit(Instruction(OpCode::MUL));
            else if (n->op == "/")  emit(Instruction(OpCode::DIV));
            else if (n->op == "==") emit(Instruction(OpCode::EQ));
            else if (n->op == "<")  emit(Instruction(OpCode::LESS));
            else if (n->op == ">")  emit(Instruction(OpCode::GREATER));
            else throw runtime_error("Unknown operator: " + n->op);
            break;
        }

        case NodeType::LetStatement: {
            auto* n = static_cast<LetNode*>(node);
            compileNode(n->expr.get());
            emit(Instruction(OpCode::STORE, n->name));
            break;
        }

        case NodeType::PrintStatement: {
            auto* n = static_cast<PrintNode*>(node);
            compileNode(n->expr.get());
            emit(Instruction(OpCode::PRINT));
            break;
        }

        case NodeType::InputExpr: {
            emit(Instruction(OpCode::INPUT));
            break;
        }

        case NodeType::IfStatement: {
            auto* n = static_cast<IfNode*>(node);
            compileNode(n->condition.get());

            int jumpIfFalseIdx = emit(Instruction(OpCode::JUMP_IF_FALSE, 0));

            compileBlock(static_cast<BlockNode*>(n->thenBlock.get()));

            if (n->elseBlock) {
                int jumpIdx = emit(Instruction(OpCode::JUMP, 0));
                patch(jumpIfFalseIdx, code.size());
                compileBlock(static_cast<BlockNode*>(n->elseBlock.get()));
                patch(jumpIdx, code.size());
            } else {
                patch(jumpIfFalseIdx, code.size());
            }
            break;
        }

        case NodeType::WhileStatement: {
            auto* n = static_cast<WhileNode*>(node);

            int loopStart = code.size();
            compileNode(n->condition.get());

            int jumpIfFalseIdx = emit(Instruction(OpCode::JUMP_IF_FALSE, 0));

            compileBlock(static_cast<BlockNode*>(n->body.get()));
            emit(Instruction(OpCode::JUMP, loopStart));

            patch(jumpIfFalseIdx, code.size());
            break;
        }

        case NodeType::Block: {
            compileBlock(static_cast<BlockNode*>(node));
            break;
        }

        default:
            throw runtime_error("Unknown AST node in compiler");
    }
}

void Compiler::printBytecode(const vector<Instruction>& code) {
    for (int i = 0; i < (int)code.size(); i++) {
        cout << i << "\t";
        switch (code[i].op) {
            case OpCode::PUSH:          cout << "PUSH "          << code[i].operand; break;
            case OpCode::PUSH_BOOL:     cout << "PUSH_BOOL "     << (code[i].operand ? "true" : "false"); break;
            case OpCode::LOAD:          cout << "LOAD "          << code[i].name; break;
            case OpCode::STORE:         cout << "STORE "         << code[i].name; break;
            case OpCode::ADD:           cout << "ADD"; break;
            case OpCode::SUB:           cout << "SUB"; break;
            case OpCode::MUL:           cout << "MUL"; break;
            case OpCode::DIV:           cout << "DIV"; break;
            case OpCode::EQ:            cout << "EQ"; break;
            case OpCode::LESS:          cout << "LESS"; break;
            case OpCode::GREATER:       cout << "GREATER"; break;
            case OpCode::PRINT:         cout << "PRINT"; break;
            case OpCode::INPUT:         cout << "INPUT"; break;
            case OpCode::JUMP:          cout << "JUMP "          << code[i].operand; break;
            case OpCode::JUMP_IF_FALSE: cout << "JUMP_IF_FALSE " << code[i].operand; break;
            case OpCode::POP:           cout << "POP"; break;
            case OpCode::HALT:          cout << "HALT"; break;
        }
        cout << "\n";
    }
}