#include "Lexer.h"
#include "Logger.h"

#include <map>
#include <set>

using namespace __Lexer;

const std::set<char> sign = { '+','-','*','/','%','<','>','=','(',')','[',']','{','}',
						'!','@','#','$','^','&','~','?',':',';','\'','\\','|' };

const std::map<std::string, OptionType> operators =
{
	{"+",	OptionType::plus},
	{"-",	OptionType::minus},
	{"*",	OptionType::times},
	{"/",	OptionType::division},
	{"%",	OptionType::modulo},

	{"(",	OptionType::left_brack},
	{")",	OptionType::right_brack},

	{"&",	OptionType::bitands},
	{"|",	OptionType::bitors},
	{"^",	OptionType::xors},
	{"~",	OptionType::bitnot},
	{">>",	OptionType::right_move},
	{"<<",	OptionType::left_move},

	{"==",	OptionType::equal},
	{">",	OptionType::bigger},
	{"<",	OptionType::lower},
	{">=",	OptionType::bigorequ},
	{"<=",	OptionType::loworequ},
	{"!=",	OptionType::notequal},

	{"&&",	OptionType::logicand},
	{"||",	OptionType::logicor},
	{"!",	OptionType::logicnot},

	{"=",	OptionType::is},
	{"+=",	OptionType::plusis},
	{"-=",	OptionType::minusis},
	{"*=",	OptionType::timesis},
	{"/=",	OptionType::divisionis},
	{"%=",	OptionType::modulois},
	{"&=",	OptionType::andis},
	{"|=",	OptionType::oris},
	{"^=",	OptionType::xoris},
	{"~=",	OptionType::notis},
	{"<<=",	OptionType::left_moveis},
	{">>=",	OptionType::right_moveis},

	{"[",	OptionType::left_block_brack},
	{"]",	OptionType::right_block_brack},

	{"?",	OptionType::question},
	{":",	OptionType::colon},

	{"**",	OptionType::power}
};
const std::string keywords[] = { "let", "def", "end", "if", "for", "do", "while" };


bool issign(char c)
{
	for (char x : sign)
		if (c == x) return true;
	return false;
}
bool isoperator(const std::string& last)
{
	return operators.find(last) != operators.end();
}

bool isKeyword(const std::string& sign)
{
	for (auto kw : keywords)
	{
		if (sign == kw) return true;
	}
	return false;
}





OptionType __Lexer::GetOptType(const std::string& c)
{
	if (operators.find(c) != operators.end()) return operators.at(c);
	return OptionType::error_option;
}

class Automaton
{
	enum State
	{
		START = 0,
		ERROR,
		END,
		NUMBER,
		OPERATOR,
		VARIABLE,
		STRING,
		MATRIX,
		SPECIAL,
	};
	enum CharType
	{
		Error = -1,
		Space = 1,
		Digit,
		Alpha,
		LeftParen,			// (
		RightParen,			// )
		LeftFlowerParen,	// {
		RightFlowerParen,	// }
		Quotation,			// "
		Comma,				// ,
		Semicolon,			// ;
		Operator
	};
	const State state_trans[255][255] = {
		//			Space,	Digit,		Alpha,		(,		),		{,		},		",		,,		;		Operator	
		{START,		START,	NUMBER,		VARIABLE,	SPECIAL,SPECIAL,MATRIX,	ERROR,	STRING,	SPECIAL,SPECIAL,OPERATOR},
		{ERROR,		ERROR,	ERROR,		ERROR,		ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	END,	END,	ERROR},
		{END,		END,	END,		END,		END,	END,	END,	END,	END,	END,	END,	END},
		{NUMBER,	END,	NUMBER,		END,		END,	END,	END,	END,	END,	END,	END,	END},
		{OPERATOR,	END,	END,		END,		END,	END,	END,	END,	END,	END,	END,	OPERATOR},
		{VARIABLE,	END,	VARIABLE,	VARIABLE,	END,	END,	END,	END,	END,	END,	END,	END},
		{STRING,	STRING,	STRING,		STRING,		STRING,	STRING,	STRING,	STRING,	END,	STRING,	STRING,	STRING},
		{MATRIX,	MATRIX,	MATRIX,		MATRIX,		MATRIX,	MATRIX,	MATRIX,	MATRIX,	ERROR,	MATRIX,	ERROR,	MATRIX},
		{SPECIAL,	SPECIAL,END,		END,		END,	END,	END,	END,	END,	END,	END,	END},
	};

	std::string str;
	int current;
	TokenType last_token = TokenType::Error;

	CharType getCharType(char c);

	char advance();
	void previous();

	static TokenType getTokenType(State state, const Token& token);

public:
	Automaton(const std::string& str);
	Token GetNextToken();

	bool isAtEnd();
};

Automaton::CharType Automaton::getCharType(char c)
{
	if (c == ' ') return Space;
	if (isdigit(c) || c == '.') return Digit;
	if (isalpha(c)) return Alpha;
	if (c == '(') return LeftParen;
	if (c == ')') return RightParen;
	if (c == '{') return LeftFlowerParen;
	if (c == '}') return RightFlowerParen;
	if (c == '"') return Quotation;
	if (c == ',') return Comma;
	if (c == ';') return Semicolon;
	if (sign.find(c) != sign.end()) return Operator;
	return CharType::Error;
}

char Automaton::advance()
{
	if (current != str.size()) ++current;
	return str[current - 1];
}
void Automaton::previous()
{
	--current;
}
bool Automaton::isAtEnd()
{
	return current == str.size();
}

TokenType Automaton::getTokenType(State state, const Token& token)
{
	if (state == ERROR) return TokenType::Error;

	if (state == NUMBER) return TokenType::Number;
	if (state == OPERATOR) return TokenType::Operator;
	if (state == MATRIX) return TokenType::Matrix;
	if (state == STRING) return TokenType::String;

	if (state == VARIABLE)
	{
		if (isKeyword(token.value)) return TokenType::Keyword;
		return TokenType::Variable;
	}

	if (state == SPECIAL)
	{
		if (token.value == "(") return TokenType::LeftParen;
		if (token.value == ")") return TokenType::RightParen;
		if (token.value == ",") return TokenType::Comma;
		if (token.value == ";")return TokenType::Semicolon;
		return TokenType::Error;
	}

	return TokenType::Error;
}

Automaton::Automaton(const std::string& str)
{
	this->str = str;
	current = 0;
}

Token Automaton::GetNextToken()
{
	int lfpnum = 0;// number of left flower paren(s)
	char quot_type = '\0';
	State state = START, last_state = START;
	Token token;
	while (!isAtEnd())
	{
		char c = advance();
		CharType type = getCharType(c);

		if (state == START || state == SPECIAL)
		{
			state = state_trans[state][type];
			if (state == STRING) quot_type = c;
			if (state == MATRIX) ++lfpnum;
		}
		else if (state == OPERATOR)
		{
			state = state_trans[state][type];
			if (state == OPERATOR && !isoperator(token.value + c))state = END;
		}
		else if (state == ERROR || state == END)
		{
			return Token::error;
		}
		else if (state == VARIABLE)
		{
			token.type = TokenType::Variable;
			state = state_trans[state][type];
		}
		else if (state == NUMBER)
		{
			token.type = TokenType::Number;
			if (!isdigit(c) && c != '.') state = END;
			state = state_trans[state][type];
		}
		else if (state == STRING)
		{
			token.type = TokenType::String;
			if (quot_type == c) state = END;
		}
		else if (state == MATRIX)
		{
			token.type = TokenType::Matrix;
			if (c == '{')
			{
				++lfpnum;
			}
			else if (c == '}')
			{
				if (lfpnum <= 0) return Token::error;
				--lfpnum;

				if (lfpnum == 0)
				{
					token.value.push_back('}');
					state = END;
				}
			}
			// else do nothing
		}
		else
		{
			state = state_trans[state][type];
		}

		if (state == END)
		{
			if (last_state != MATRIX) previous();
			break;
		}
		if (!isspace(c)) token.value.push_back(c);
		last_state = state;
	}
	token.type = getTokenType(last_state, token);
	return token;
}


std::vector<Token> Lexer::GetTokens(const std::string& target)
{
	std::vector<Token> tokens;
	if (target.empty())return tokens;
	Automaton automaton(target);
	while (!automaton.isAtEnd())
	{
		Token token = automaton.GetNextToken();
		tokens.emplace_back(token);
		if (token.type == TokenType::Error) break;
		if (token.type == TokenType::Function) tokens.emplace_back(Token(TokenType::Operator, "("));
	}
	return tokens;
}

