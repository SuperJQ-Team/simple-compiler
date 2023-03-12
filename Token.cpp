#include "Token.h"

Token::Token() : type(TokenType::Error)
{

}
Token::Token(TokenType type, const std::string& value) : type(type), value(value)
{

}

void Token::SetType(TokenType type)
{
	if (this->type == TokenType::Error) this->type = type;
}
