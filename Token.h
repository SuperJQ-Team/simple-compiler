#pragma once

#include <string>

enum class TokenType
{
	Undefined = -2,
	Error = -1,
	Number = 0,
	Operator,
	Keyword,
	Variable,
	String,
	Matrix,
	Function,
	BeforeOp,
	LeftParen,
	RightParen,
	Comma,
	Semicolon,
	End,
};

class Token
{
public:
	TokenType type;
	std::string value;

	static Token error;
	
	Token();
	Token(TokenType type);
	Token(TokenType type, const std::string& value);
	
	void SetType(TokenType type);
};


