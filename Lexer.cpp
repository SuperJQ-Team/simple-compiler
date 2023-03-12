#include "Lexer.h"
#include "Logger.h"

#include <map>
#include <set>

using namespace __Lexer;

const std::set<char> sign = {'+','-','*','/','%','<','>','=','(',')','[',']','{','}',
						'!','@','#','$','^','&','~','?',':',';','\'','\\','|' };

const std::map<std::string, OptionType> options =
{
	{"+",	plus},
	{"-",	minus},
	{"*",	times},
	{"/",	division},
	{"%",	modulo},

	{"(",	left_brack},
	{")",	right_brack},

	{"&",	bitands},
	{"|",	bitors},
	{"^",	xors},
	{"~",	bitnot},
	{">>",	right_move},
	{"<<",	left_move},

	{"==",	equal},
	{">",	bigger},
	{"<",	lower},
	{">=",	bigorequ},
	{"<=",	loworequ},
	{"!=",	notequal},

	{"&&",	logicand},
	{"||",	logicor},
	{"!",	logicnot},

	{"=",	is},
	{"+=",	plusis},
	{"-=",	minusis},
	{"*=",	timesis},
	{"/=",	divisionis},
	{"%=",	modulois},
	{"&=",	andis},
	{"|=",	oris},
	{"^=",	xoris},
	{"~=",	notis},
	{"<<=",	left_moveis},
	{">>=",	right_moveis},

	{"[",	left_block_brack},
	{"]",	right_block_brack},

	{"?",	question},
	{":",	colon},

	{"**",	power}
};
const std::string keywords[] = { "let", "def", "end", "if", "for", "do", "while" };


bool issign(char c)
{
	for (char x : sign)
		if (c == x) return true;
	return false;
}

bool isoperation(const std::string& last)
{
	return options.find(last) != options.end();
}

bool mayKeyword(const std::string& last_sign, char c)
{
	std::string temp = last_sign + c;
	for (std::string s : keywords)
	{
		if (temp.length() > s.length()) continue;
		bool ok = true;
		for (int i = 0; i < temp.length(); ++i)
		{
			if (temp[i] != s[i]) ok = false;
		}
		if (ok) return true;
	}
	return false;
}

bool isKeyword(const std::string& sign)
{
	for (auto kw : keywords)
	{
		if (sign == kw) return true;
	}
	return false;
}

bool mayBeforeOpt(const std::string& sign)
{
	return sign == "-" || sign == "~" || sign == "!";
}



Token::Token() : type(error)
{

}
Token::Token(TokenType type, const std::string& value) : type(type), value(value)
{

}


OptionType __Lexer::GetOptType(const std::string& c)
{
	if (options.find(c) != options.end()) return options.at(c);
	return error_option;
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
		KEYWORD,
		MATRIX
	};
	enum CharType
	{
		ERROR = -1,
		Space = 1,
		Digit,
		Alpha,
		LeftParen,			// (
		RightParen,			// )
		LeftFlowerParen,	// {
		RightFlowerParen,	// }
		Quotation,			// "
		Comma,				// ,
		OPERATOR
	};
	const State state_t[255][255] = {
		//			Space,	Digit,		Alpha,		(,		),		{,		},		",		,,		Operator	
		{START,		START,	NUMBER,		KEYWORD,	END,	END,	MATRIX,	ERROR,	STRING,	END,	OPERATOR},
		{ERROR,		ERROR,	ERROR,		ERROR,		ERROR,	ERROR,	ERROR,	ERROR,	ERROR,	END,	ERROR},
		{END,		END,	END,		END,		END,	END,	END,	END,	END,	END,	END},
		{NUMBER,	END,	NUMBER,		END,		END,	END,	END,	END,	END,	END,	END},
		{OPERATOR,	END,	END,		END,		END,	END,	END,	END,	END,	END,	OPERATOR},
		{VARIABLE,	END,	VARIABLE,	VARIABLE,	END,	END,	END,	END,	END,	END,	END},
		{STRING,	STRING,	STRING,		STRING,		STRING,	STRING,	STRING,	STRING,	END,	STRING,	STRING},
		{KEYWORD,	END,	KEYWORD,	KEYWORD,	END,	END,	END,	END,	END,	END,	END},
		{MATRIX,	MATRIX,	MATRIX,		MATRIX,		MATRIX,	MATRIX,	MATRIX,	MATRIX,	ERROR,	MATRIX,	MATRIX}
	};
	const State state_trans[255][255] = {
		{START,		ERROR,	END,	NUMBER,		OPERATOR, VARIABLE,	STRING, Space,	Keyword,	Matrix},
		{Error,		Error,	Error,	Error,		Error,	Error,		Error,	Error,	Error,		Error},
		{End,		End,	End,	End,		End,	End,		End,	End,	End,		End},
		{Number,	Error,	End,	Number,		End,	Error,		Error,	End,	Error,		End},
		{Operator,	Error,	End,	End,		End,	End,		End,	End,	Error,		End},
		{Variable,	Error,	End,	Variable,	End,	Variable,	Error,	End,	Error,		End},
		{String,	Error,	Error,	String,		String,	String,		End,	String,	String,		String},
		{Keyword,	Error,	End,	Variable,	Error,	Variable,	Error,	End,	Keyword,	Error},
		{Matrix,	Error,	End,	Matrix,		Error,	Error,		Error,	Matrix,	Error,		Matrix},
	};
	

	std::string str;
	std::string::iterator it;
	int current;
	TokenType last_token = TokenType::Error;

	State state;

	CharType getCharType(char c);

	char advance();
	char previous();

	

public:
	Automaton(const std::string& str);
	Token GetNextToken();

	bool isAtEnd();
};

Automaton::CharType Automaton::getCharType(char c)
{
	if (c == ' ') return CharType::Space;
	if (isdigit(c)) return CharType::Digit;
	if (isalpha(c)) return CharType::Alpha;
	if (c == '(') return CharType::LeftParen;
	if (c == ')') return CharType::RightParen;
	if (c == '{') return CharType::LeftFlowerParen;
	if (c == '}') return CharType::RightFlowerParen;
	if (c == '"') return CharType::Quotation;
	if (c == ',') return CharType::Comma;
	if (sign.find(c) != sign.end()) return CharType::OPERATOR;
	return CharType::ERROR;
}

char Automaton::advance()
{
	if (current != str.size()) ++current;
	return str[current];
}
char Automaton::previous()
{
	return str[current - 1];
}
bool Automaton::isAtEnd()
{
	return current == str.size();
}

Automaton::Automaton(const std::string& str)
{
	this->str = str;
	this->it = this->str.begin();
	current = 0;
}

Token Automaton::GetNextToken()
{
	state = START;
	Token token;
	while (!isAtEnd())
	{
		if (state == START)
		{
			char c = advance();
			CharType type = getCharType(c);
			state = state_t[state][type];
		}
	}
}

Token Automaton::GetNextToken()
{
	char str_sign = -1;
	char last_sign = -1;
	int left_bracks_num = 0;
	state = START;
	Token token;
	while (it != str.end())
	{
		if (*it == '\0' || *it == ';' || *it == '\n')
		{
			if (state == START || state == Space) token.type = TokenType::End;
			if (state == STRING && *it == ';')state = STRING;
			else
			{
				state = state_trans[state][END];
				++it;
				break;
			}
		}
		else if (isdigit(*it) || *it == '.')
		{
			if (state == START || state == Space) token.type = TokenType::Number;
			state = state_trans[state][NUMBER];
		}
		else if (*it == '{' || *it == '}')
		{
			if (state == START || state == Space) token.type = TokenType::Matrix;
			if (*it == '{') ++left_bracks_num;
			else if (*it == '}')
			{
				if (left_bracks_num == 0) state = ERROR;
				--left_bracks_num;
				if (left_bracks_num == 0)
				{
					token.value.push_back('}');
					++it;
					state = END;
				}
			}
			if (state != END && state != ERROR) state = MATRIX;
		}
		else if (*it == '"' || *it == '\'')
		{
			if (state == START || state == Space)
			{
				token.type = string;
				str_sign = *it;
			}
			if (*it != str_sign && state == STRING)
				state = state_trans[state][OPERATOR];
			else
				state = state_trans[state][STRING];
			if (state == END && *it == str_sign) ++it;
		}
		else if (issign(*it))
		{
			if ((last_token == __Lexer::option || last_token == __Lexer::beforpot || last_token == __Lexer::keyword || last_token == __Lexer::error)
				&& mayBeforeOpt(token.value + *it))
			{
				if (state == START || state == Space || token.type == TokenType::Operator) token.type = TokenType::BeforeOp;
				state = OPERATOR;
			}
			else if ((last_token != __Lexer::option && last_token != __Lexer::beforpot && last_token != __Lexer::keyword && last_token != __Lexer::error)
				&& (isoperation(token.value + *it) && !mayBeforeOpt(token.value + *it)))
			{
				if (state == START || state == Space) token.type = TokenType::Operator;
				state = OPERATOR;
			}
			else
			{
				if (state == START || state == Space) token.type = TokenType::Operator;
				if (*it == '(' && ((state == VARIABLE && token.type == TokenType::Variable) || (state == KEYWORD && !isKeyword(token.value))))
				{
					token.type = function;
					++it;
					state = END;
				}
				if ((*it == '(' && state == OPERATOR) || last_sign == ')')state = END;
				else state = state_trans[state][OPERATOR];
			}
		}
		else if (isalpha(*it) || *it == '_')
		{
			if (mayKeyword(token.value, *it))
			{
				if (state == START || state == Space) token.type = TokenType::Keyword;
				state = state_trans[state][KEYWORD];
			}
			else
			{
				if (state == START || state == Space || state == KEYWORD) token.type = TokenType::Variable;
				state = state_trans[state][VARIABLE];
			}
		}
		else if (*it == ' ' || *it == ',')
		{
			state = state_trans[state][Space];
		}

		if (state == ERROR)
		{
			token.type = error;
			return token;
		}

		if (state == END)
		{
			break;
		}
		if ((state == STRING && *it != str_sign) ||
			(state != STRING && state != START && state != Space && *it != ' ')) token.value.push_back(*it);
		last_sign = *it;
		++it;
	}
	if (state == ERROR)token.type = error;
	if (it == str.end()) lex_end = true;
	if (left_bracks_num != 0) token.type = error;
	if (token.type == keyword && !isKeyword(token.value)) token.type = variable;
	last_token = token.type;
	return token;
}


std::vector<Token> Lexer::GetTokens(const std::string& target)
{
	std::vector<Token> tokens;
	if (target.empty())return tokens;
	Automaton automaton(target);
	while (automaton.isAtEnd())
	{
		Token token = automaton.GetNextToken();
		tokens.emplace_back(token);
		if (token.type == error) break;
		if (token.type == function) tokens.emplace_back(Token(option, "("));
	}
	return tokens;
}

