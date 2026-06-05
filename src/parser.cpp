#include "parser.h"
#include <stdexcept>

using namespace std;

Parser::Parser(vector<Token> tokens)
    : tokens(move(tokens)), pos(0) {}

Token& Parser::current() { return tokens[pos]; }

Token& Parser::peek(int offset) {
    int idx = pos + offset;
    if (idx >= (int)tokens.size()) return tokens.back();
    return tokens[idx];
}

void Parser::advance() {
    if (pos < (int)tokens.size() - 1) pos++;
}

bool Parser::check(TokenType t) {
    return current().type == t;
}

Token Parser::consume(TokenType expected) {
    if (current().type != expected)
        throw runtime_error("Unexpected token: " + current().value);
    Token t = current();
    advance();
    return t;
}

unique_ptr<BlockNode> Parser::parse() {
    auto block = make_unique<BlockNode>();
    while (!check(TokenType::END_OF_FILE)) {
        block->statements.push_back(parseStatement());
    }
    return block;
}

unique_ptr<ASTNode> Parser::parseStatement() {
    if (check(TokenType::LET)) {
        advance();
        string name = consume(TokenType::IDENTIFIER).value;
        consume(TokenType::EQUALS);
        auto expr = parseExpression();
        return make_unique<LetNode>(name, move(expr));
    }

    if (check(TokenType::PRINT)) {
        advance();
        auto expr = parseExpression();
        return make_unique<PrintNode>(move(expr));
    }

    if (check(TokenType::IF)) {
        advance();
        consume(TokenType::LPAREN);
        auto cond = parseExpression();
        consume(TokenType::RPAREN);
        auto thenBlock = parseBlock();
        unique_ptr<ASTNode> elseBlock = nullptr;
        if (check(TokenType::ELSE)) {
            advance();
            elseBlock = parseBlock();
        }
        return make_unique<IfNode>(move(cond), move(thenBlock), move(elseBlock));
    }

    if (check(TokenType::WHILE)) {
        advance();
        consume(TokenType::LPAREN);
        auto cond = parseExpression();
        consume(TokenType::RPAREN);
        auto body = parseBlock();
        return make_unique<WhileNode>(move(cond), move(body));
    }

    throw runtime_error("Unknown statement starting with: " + current().value);
}

unique_ptr<BlockNode> Parser::parseBlock() {
    consume(TokenType::LBRACE);
    auto block = make_unique<BlockNode>();
    while (!check(TokenType::RBRACE) && !check(TokenType::END_OF_FILE)) {
        block->statements.push_back(parseStatement());
    }
    consume(TokenType::RBRACE);
    return block;
}

unique_ptr<ASTNode> Parser::parseExpression() {
    return parseComparison();
}

unique_ptr<ASTNode> Parser::parseComparison() {
    auto left = parseAddSub();
    while (check(TokenType::EQ_EQ) || check(TokenType::LESS) || check(TokenType::GREATER)) {
        string op = current().value;
        advance();
        auto right = parseAddSub();
        left = make_unique<BinaryOpNode>(op, move(left), move(right));
    }
    return left;
}

unique_ptr<ASTNode> Parser::parseAddSub() {
    auto left = parseMulDiv();
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        string op = current().value;
        advance();
        auto right = parseMulDiv();
        left = make_unique<BinaryOpNode>(op, move(left), move(right));
    }
    return left;
}

unique_ptr<ASTNode> Parser::parseMulDiv() {
    auto left = parsePrimary();
    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        string op = current().value;
        advance();
        auto right = parsePrimary();
        left = make_unique<BinaryOpNode>(op, move(left), move(right));
    }
    return left;
}

unique_ptr<ASTNode> Parser::parsePrimary() {
    if (check(TokenType::NUMBER)) {
        int val = stoi(current().value);
        advance();
        return make_unique<NumberNode>(val);
    }
    if (check(TokenType::TRUE)) {
        advance();
        return make_unique<BooleanNode>(true);
    }
    if (check(TokenType::FALSE)) {
        advance();
        return make_unique<BooleanNode>(false);
    }
    if (check(TokenType::IDENTIFIER)) {
        string name = current().value;
        advance();
        return make_unique<IdentifierNode>(name);
    }
    if (check(TokenType::INPUT)) {
        advance();
        return make_unique<InputNode>();
    }
    if (check(TokenType::LPAREN)) {
        advance();
        auto expr = parseExpression();
        consume(TokenType::RPAREN);
        return expr;
    }

    throw runtime_error("Unexpected token in expression: " + current().value);
}