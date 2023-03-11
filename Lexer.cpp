#include "Lexer.h"
#include "Logger.h"

#include <map>

using namespace __Lexer;

const char sign[] = { '+','-','*','/','%','<','>','=','(',')','[',']','{','}',
						'!','@','#','$','^','&','~','?',':',';','\'','"','\\','|' };

const std::map<std::string, OptionType> options =
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






OptionType __Lexer::GetOptType(const std::string& c)
{
	if (options.find(c) != options.end()) return options.at(c);
	return OptionType::error_option;
}

class Automaton
{
	enum State
	{
		START = 0,
		ERROR = 1,
		END = 2,
		NUMBER = 3,
		OPTION = 4,
		VARIABLE = 5,
		STRING = 6,
		SPACE = 7,
		KEYWORD = 8,
		MATRIX = 9,
		BEFOROPT = 10,

	};
	const State state_trans[255][255] = {
		{START,		ERROR,	END,	NUMBER,		OPTION, VARIABLE,	STRING, SPACE,	KEYWORD,	MATRIX,	BEFOROPT},
		{ERROR,		ERROR,	ERROR,	ERROR,		ERROR,	ERROR,		ERROR,	ERROR,	ERROR,		ERROR,	ERROR},
		{END,		END,	END,	END,		END,	END,		END,	END,	END,		END,	END},
		{NUMBER,	ERROR,	END,	NUMBER,		END,	ERROR,		ERROR,	END,	ERROR,		END,	},
		{OPTION,	ERROR,	END,	END,		END,	END,		END,	END,	ERROR,		END},
		{VARIABLE,	ERROR,	END,	VARIABLE,	END,	VARIABLE,	ERROR,	END,	ERROR,		END},
		{STRING,	ERROR,	ERROR,	STRING,		STRING,	STRING,		END,	STRING,	STRING,		STRING},
		{SPACE,		ERROR,	END,	NUMBER,		OPTION,	VARIABLE,	STRING,	SPACE,	KEYWORD,	MATRIX},
		{KEYWORD,	ERROR,	END,	VARIABLE,	ERROR,	VARIABLE,	ERROR,	END,	KEYWORD,	ERROR},
		{MATRIX,	ERROR,	END,	MATRIX,		ERROR,	ERROR,		ERROR,	MATRIX,	ERROR,		MATRIX},
	};

	std::string str;
	std::string::iterator it;
	TokenType last_token = TokenType::Error;

	State state;

public:
	Automaton(const std::string& str);
	Token GetNextToken();
	bool lex_end;
};

Automaton::Automaton(const std::string& str)
{
	this->str = str;
	this->str.push_back('\0');
	this->it = this->str.begin();
	lex_end = false;
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
			if (state == START || state == SPACE) token.type = TokenType::End;
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
			if (state == START || state == SPACE) token.type = TokenType::Number;
			state = state_trans[state][NUMBER];
		}
		else if (*it == '{' || *it == '}')
		{
			if (state == START || state == SPACE) token.type = TokenType::Matrix;
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
			if (state == START || state == SPACE)
			{
				token.type = TokenType::String;
				str_sign = *it;
			}
			if (*it != str_sign && state == STRING)
				state = state_trans[state][OPTION];
			else
				state = state_trans[state][STRING];
			if (state == END && *it == str_sign) ++it;
		}
		else if (issign(*it))
		{
			if ((last_token == TokenType::Operator || last_token == TokenType::BeforeOp || last_token == TokenType::Keyword || last_token == TokenType::Error)
				&& mayBeforeOpt(token.value + *it))
			{
				if (state == START || state == SPACE || token.type == TokenType::Operator) token.type = TokenType::BeforeOp;
				state = OPTION;
			}
			else if ((last_token != TokenType::Operator && last_token != TokenType::BeforeOp && last_token != TokenType::Keyword && last_token != TokenType::Error)
				&& (isoperation(token.value + *it) && !mayBeforeOpt(token.value + *it)))
			{
				if (state == START || state == SPACE) token.type = TokenType::Operator;
				state = OPTION;
			}
			else
			{
				if (state == START || state == SPACE) token.type = TokenType::Operator;
				if (*it == '(' && ((state == VARIABLE && token.type == TokenType::Variable) || (state == KEYWORD && !isKeyword(token.value))))
				{
					token.type = TokenType::Function;
					++it;
					state = END;
				}
				if ((*it == '(' && state == OPTION) || last_sign == ')')state = END;
				else state = state_trans[state][OPTION];
			}
		}
		else if (isalpha(*it) || *it == '_')
		{
			if (mayKeyword(token.value, *it))
			{
				if (state == START || state == SPACE) token.type = TokenType::Keyword;
				state = state_trans[state][KEYWORD];
			}
			else
			{
				if (state == START || state == SPACE || state == KEYWORD) token.type = TokenType::Variable;
				state = state_trans[state][VARIABLE];
			}
		}
		else if (*it == ' ' || *it == ',')
		{
			state = state_trans[state][SPACE];
		}

		if (state == ERROR)
		{
			token.type = TokenType::Error;
			return token;
		}

		if (state == END)
		{
			break;
		}
		if ((state == STRING && *it != str_sign) ||
			(state != STRING && state != START && state != SPACE && *it != ' ')) token.value.push_back(*it);
		last_sign = *it;
		++it;
	}
	if (state == ERROR)token.type = TokenType::Error;
	if (it == str.end()) lex_end = true;
	if (left_bracks_num != 0) token.type = TokenType::Error;
	if (token.type == TokenType::Keyword && !isKeyword(token.value)) token.type = TokenType::Variable;
	last_token = token.type;
	return token;
}


std::vector<Token> Lexer::GetTokens(const std::string& input_string)
{
	std::vector<Token> tokens;
	if (input_string.empty())return tokens;
	Automaton automaton(input_string);
	while (automaton.lex_end == false)
	{
		Token token = automaton.GetNextToken();
		tokens.emplace_back(token);
		if (token.type == TokenType::Error) break;
		if (token.type == TokenType::Function) tokens.emplace_back(Token(TokenType::Operator, "("));
	}
	return tokens;
}

