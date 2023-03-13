#pragma once

#include <vector>
#include "Token.h"
#include "ASTNode.h"

class Parser
{
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    ASTNode* parse();

private:
    std::vector<Token> tokens;
    size_t current;

    ASTNode* expression();
    ASTNode* call();
    ASTNode* finishCall(ASTNode* callee);
    ASTNode* primary();

    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    bool check(TokenType type);

    Token advance();
    Token previous();

    bool IsAtEnd();
};

//class Parser
//{
//public:
//	void GetAST(const std::vector<Token>& tokens);
//};
