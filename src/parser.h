#pragma once
#include "lexer.h"
#include <memory>
#include <vector>

using namespace std;

enum class NodeType {
    Number,
    Boolean,
    Identifier,
    BinaryOp,
    LetStatement,
    PrintStatement,
    InputExpr,
    IfStatement,
    WhileStatement,
    Block,
};

struct ASTNode {
    NodeType type;
    ASTNode(NodeType type) : type(type) {}
    virtual ~ASTNode() = default;
};

struct NumberNode : ASTNode {
    int value;
    NumberNode(int value) : ASTNode(NodeType::Number), value(value) {}
};

struct BooleanNode : ASTNode {
    bool value;
    BooleanNode(bool value) : ASTNode(NodeType::Boolean), value(value) {}
};

struct IdentifierNode : ASTNode {
    string name;
    IdentifierNode(string name) : ASTNode(NodeType::Identifier), name(name) {}
};

struct BinaryOpNode : ASTNode {
    string op;
    unique_ptr<ASTNode> left;
    unique_ptr<ASTNode> right;
    BinaryOpNode(string op, unique_ptr<ASTNode> left, unique_ptr<ASTNode> right)
        : ASTNode(NodeType::BinaryOp), op(op),
          left(move(left)), right(move(right)) {}
};

struct LetNode : ASTNode {
    string name;
    unique_ptr<ASTNode> expr;
    LetNode(string name, unique_ptr<ASTNode> expr)
        : ASTNode(NodeType::LetStatement), name(name), expr(move(expr)) {}
};

struct PrintNode : ASTNode {
    unique_ptr<ASTNode> expr;
    PrintNode(unique_ptr<ASTNode> expr)
        : ASTNode(NodeType::PrintStatement), expr(move(expr)) {}
};

struct InputNode : ASTNode {
    InputNode() : ASTNode(NodeType::InputExpr) {}
};

struct IfNode : ASTNode {
    unique_ptr<ASTNode> condition;
    unique_ptr<ASTNode> thenBlock;
    unique_ptr<ASTNode> elseBlock;
    IfNode(unique_ptr<ASTNode> cond, unique_ptr<ASTNode> thenB, unique_ptr<ASTNode> elseB)
        : ASTNode(NodeType::IfStatement),
          condition(move(cond)),
          thenBlock(move(thenB)),
          elseBlock(move(elseB)) {}
};

struct WhileNode : ASTNode {
    unique_ptr<ASTNode> condition;
    unique_ptr<ASTNode> body;
    WhileNode(unique_ptr<ASTNode> cond, unique_ptr<ASTNode> body)
        : ASTNode(NodeType::WhileStatement),
          condition(move(cond)),
          body(move(body)) {}
};

struct BlockNode : ASTNode {
    vector<unique_ptr<ASTNode>> statements;
    BlockNode() : ASTNode(NodeType::Block) {}
};

class Parser {
public:
    Parser(vector<Token> tokens);
    unique_ptr<BlockNode> parse();

private:
    vector<Token> tokens;
    int pos;

    Token& current();
    Token& peek(int offset = 1);
    Token consume(TokenType expected);
    void advance();
    bool check(TokenType t);

    unique_ptr<ASTNode> parseStatement();
    unique_ptr<BlockNode> parseBlock();
    unique_ptr<ASTNode> parseExpression();
    unique_ptr<ASTNode> parseComparison();
    unique_ptr<ASTNode> parseAddSub();
    unique_ptr<ASTNode> parseMulDiv();
    unique_ptr<ASTNode> parsePrimary();
};