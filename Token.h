#pragma once

#include <string>

enum class TokenType
{
	Error = -1,
	Number = 0,
	Operator,
	Keyword,
	Variable,
	String,
	Matrix,
	Function,
	End,
	BeforeOp,
	LeftParen,
	RightParen,
	Comma,
};

class Token
{
public:
	TokenType type;
	std::string value;
	Token();
	Token(TokenType type, const std::string& value);
};


