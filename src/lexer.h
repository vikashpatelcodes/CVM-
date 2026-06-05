#pragma once
#include <string>
#include <vector>

using namespace std;

enum class TokenType {
    NUMBER, BOOLEAN, IDENTIFIER,

    LET, IF, ELSE, WHILE, PRINT, INPUT, TRUE, FALSE,

    PLUS, MINUS, STAR, SLASH,
    EQUALS,
    EQ_EQ,
    LESS,
    GREATER,

    LPAREN, RPAREN,
    LBRACE, RBRACE,

    END_OF_FILE
};

struct Token {
    TokenType type;
    string value;

    Token(TokenType type, string value)
        : type(type), value(value) {}
};

class Lexer {
public:
    Lexer(const string& source);
    vector<Token> tokenize();

private:
    string source;
    int pos;
    char current;

    void advance();
    void skipWhitespace();
    Token readNumber();
    Token readIdentifierOrKeyword();
};