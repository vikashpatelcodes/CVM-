#include "lexer.h"
#include <stdexcept>

using namespace std;

Lexer::Lexer(const string& source)
    : source(source), pos(0) {
    current = source.empty() ? '\0' : source[0];
}

void Lexer::advance() {
    pos++;
    current = (pos < (int)source.size()) ? source[pos] : '\0';
}

void Lexer::skipWhitespace() {
    while (current == ' ' || current == '\t' || current == '\n' || current == '\r')
        advance();
}

Token Lexer::readNumber() {
    string num;
    while (isdigit(current)) {
        num += current;
        advance();
    }
    return Token(TokenType::NUMBER, num);
}

Token Lexer::readIdentifierOrKeyword() {
    string word;
    while (isalpha(current) || isdigit(current) || current == '_') {
        word += current;
        advance();
    }

    if (word == "let")   return Token(TokenType::LET, word);
    if (word == "if")    return Token(TokenType::IF, word);
    if (word == "else")  return Token(TokenType::ELSE, word);
    if (word == "while") return Token(TokenType::WHILE, word);
    if (word == "print") return Token(TokenType::PRINT, word);
    if (word == "input") return Token(TokenType::INPUT, word);
    if (word == "true")  return Token(TokenType::TRUE, word);
    if (word == "false") return Token(TokenType::FALSE, word);

    return Token(TokenType::IDENTIFIER, word);
}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;

    while (current != '\0') {
        skipWhitespace();
        if (current == '\0') break;

        if (isdigit(current)) {
            tokens.push_back(readNumber());
        }
        else if (isalpha(current) || current == '_') {
            tokens.push_back(readIdentifierOrKeyword());
        }
        else {
            switch (current) {
                case '+': tokens.push_back(Token(TokenType::PLUS,    "+")); advance(); break;
                case '-': tokens.push_back(Token(TokenType::MINUS,   "-")); advance(); break;
                case '*': tokens.push_back(Token(TokenType::STAR,    "*")); advance(); break;
                case '/': tokens.push_back(Token(TokenType::SLASH,   "/")); advance(); break;
                case '(': tokens.push_back(Token(TokenType::LPAREN,  "(")); advance(); break;
                case ')': tokens.push_back(Token(TokenType::RPAREN,  ")")); advance(); break;
                case '{': tokens.push_back(Token(TokenType::LBRACE,  "{")); advance(); break;
                case '}': tokens.push_back(Token(TokenType::RBRACE,  "}")); advance(); break;
                case '<': tokens.push_back(Token(TokenType::LESS,    "<")); advance(); break;
                case '>': tokens.push_back(Token(TokenType::GREATER, ">")); advance(); break;
                case '=':
                    advance();
                    if (current == '=') {
                        tokens.push_back(Token(TokenType::EQ_EQ, "=="));
                        advance();
                    } else {
                        tokens.push_back(Token(TokenType::EQUALS, "="));
                    }
                    break;
                default:
                    throw runtime_error(string("Unknown character: ") + current);
            }
        }
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, ""));
    return tokens;
}