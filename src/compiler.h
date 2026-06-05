#pragma once
#include "parser.h"
#include <vector>
#include <string>

using namespace std;

enum class OpCode : uint8_t {
    PUSH,
    PUSH_BOOL,
    LOAD,
    STORE,
    ADD,
    SUB,
    MUL,
    DIV,
    EQ,
    LESS,
    GREATER,
    PRINT,
    INPUT,
    JUMP,
    JUMP_IF_FALSE,
    POP,
    HALT
};

struct Instruction {
    OpCode op;
    int operand;
    string name;

    Instruction(OpCode op, int operand = 0)
        : op(op), operand(operand) {}

    Instruction(OpCode op, string name)
        : op(op), operand(0), name(move(name)) {}
};

class Compiler {
public:
    vector<Instruction> compile(BlockNode* program);
    void printBytecode(const vector<Instruction>& code);

private:
    vector<Instruction> code;

    void compileNode(ASTNode* node);
    void compileBlock(BlockNode* node);

    int emit(Instruction instr);
    void patch(int idx, int target);
};