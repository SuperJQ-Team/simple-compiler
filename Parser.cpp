#include "Parser.h"


/*

define -> (def FUNCNAME)| (let VARNAME ("=" value)*)
expr -> value | (expr (("+" | "-" | "*" | "/"...) expr)*) | ("-" value)
value -> NUMBER | MATRIX | VARIABLE | STRING | funccall

funccall -> FUNCNAME "(" (NUMBER ("," value("," value...))) ")"

*/

ASTNode* Parser::parse()
{
    current = 0;
    return expression();
}
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
    if (IsAtEnd()) return false;
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
    if (IsAtEnd()) return false;
    return tokens[current].type == type;
}

Token Parser::advance()
{
    if (!IsAtEnd()) current++;
    return previous();
}

Token Parser::previous()
{
    return tokens[current - 1];
}

bool Parser::IsAtEnd()
{
    return current == tokens.size();
}

//enum class TokenType
//{
//	Undefined = -2,
//	Error = -1,
//	Number = 0,
//	Operator,
//	Keyword,
//	Variable,
//	String,
//	Matrix,
//	Function,
//	End,
//	BeforeOp,
//	LeftParen,
//	RightParen,
//	Comma,
//};

//class Automton
//{
//public:
//	enum ParserState
//	{
//		End = -2,
//		Error = -1,
//
//		Start = 1,
//		Keyword = 2,
//		Operator = 3,
//		Variable = 4,
//		Function = 5,
//		BeforOpt = 6,
//
//	};
//
//	ParserState state_trans[255][255] = {
//		{Start,		Keyword,	Operator,	Variable,	Function},//
//		{Variable,	},//	Number = 0,
//		{Error,		},//	Operator,
//		{Keyword,	},//	Keyword,
//		{Variable,	},//	Variable,
//		{Variable	},//	String,
//		{Variable	},//	Matrix,
//		{Function	},//	Function,
//		{End		},//	End,
//		{Operator	},//	BeforeOp,
//		{Operator	},//	LeftParen,
//		{Operator	},//	RightParen,
//		{Operator	},//	Comma,
//	};
//
//	ParserState state;
//};


