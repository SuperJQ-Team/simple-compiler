#include "Token.h"

Token::Token() : type(TokenType::Error)
{

}
Token::Token(TokenType type, const std::string& value) : type(type), value(value)
{

}
