#include "Token.h"
#include "Var.h"

Token Token::error = Token(TokenType::Error);

Token::Token() : type(TokenType::Undefined)
{

}
Token::Token(TokenType type) : type(type)
{

}
Token::Token(TokenType type, const std::string& value) : type(type), value(value)
{

}

void Token::SetType(TokenType type)
{
	if (this->type == TokenType::Error) this->type = type;
}
