#include "Parser.h"

ASTNode* Parser::expression()
{
    return call();
}

ASTNode* Parser::call()
{
    ASTNode* node = primary();
    while (match(TokenType::LeftParen))
    {
        node = finishCall(node);
    }
    return node;
}

ASTNode* Parser::finishCall(ASTNode* callee)
{
    std::vector<ASTNode*> arguments;
    if (!check(TokenType::RightParen))
    {
        do
        {
            arguments.push_back(expression());
        } while (match(TokenType::Comma));
    }
    if (arguments.size() > 255)
    {
        // handle error here
    }
    consume(TokenType::RightParen, "Expect ')' after arguments.");
    return new ASTNode(callee, arguments);
}

ASTNode* Parser::primary()
{
    if (match(TokenType::Keyword))
    {
        Token name = previous();
        return new ASTNode(name);
    }
    else if (match(TokenType::Number))
    {
        Token number = previous();
        return new ASTNode(number);
    }
    // handle error here
}

bool Parser::match(TokenType type)
{
    if (isAtEnd()) return false;
    if (tokens[current].type == type)
    {
        current++;
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message)
{
    if (check(type)) return advance();
    // handle error here
}

bool Parser::check(TokenType type)
{
    if (isAtEnd()) return false;
    return tokens[current].type == type;
}

Token Parser::advance()
{
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::previous()
{
    return tokens[current - 1];
}

bool Parser::isAtEnd()
{
    return current == tokens.size();
}