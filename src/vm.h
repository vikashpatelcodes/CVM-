#pragma once
#include "compiler.h"
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

struct Value {
    int intVal;
    bool boolVal;
    bool isBool;

    Value(int v)  : intVal(v), boolVal(false), isBool(false) {}
    Value(bool v) : intVal(0), boolVal(v),     isBool(true)  {}
    Value()       : intVal(0), boolVal(false), isBool(false) {}

    bool isTrue() const {
        return isBool ? boolVal : (intVal != 0);
    }

    bool operator==(const Value& other) const {
        if (isBool && other.isBool)   return boolVal == other.boolVal;
        if (!isBool && !other.isBool) return intVal  == other.intVal;
        return false;
    }
};

class VM {
public:
    void execute(const vector<Instruction>& code);

private:
    vector<Value> stack;
    unordered_map<string, Value> vars;

    void push(Value v);
    Value pop();
    void printValue(const Value& v);
};