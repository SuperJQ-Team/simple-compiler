#include "Lexer.h"
#include "Logger.h"

#include <map>

using namespace __Lexer;

const char sign[] = { '+','-','*','/','%','<','>','=','(',')','[',']','{','}',
						'!','@','#','$','^','&','~','?',':',';','\'','"','\\','|' };

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

bool isoption(char c)
{
	for (char x : sign)
		if (c == x) return true;
	return false;
}

bool isoperation(const std::string& last, char c)
{
	std::string temp = last + c;
	if (options.find(temp) != options.end()) return true;
	return false;
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
		{OPTION,	ERROR,	END,	END,		ERROR,	END,		END,	END,	ERROR,		END},
		{VARIABLE,	ERROR,	END,	VARIABLE,	END,	VARIABLE,	ERROR,	END,	ERROR,		END},
		{STRING,	ERROR,	ERROR,	STRING,		STRING,	STRING,		END,	STRING,	STRING,		STRING},
		{SPACE,		ERROR,	END,	NUMBER,		OPTION,	VARIABLE,	STRING,	SPACE,	KEYWORD,	MATRIX},
		{KEYWORD,	ERROR,	END,	VARIABLE,	ERROR,	VARIABLE,	ERROR,	END,	KEYWORD,	ERROR},
		{MATRIX,	ERROR,	END,	MATRIX,		ERROR,	ERROR,		ERROR,	MATRIX,	ERROR,		MATRIX},
	};

	std::string str;
	std::string::iterator it;

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
		printf("Now is at %c\n", *it);
		if (*it == '\0' || *it == ';'||*it=='\n')
		{
			if (state == START || state == SPACE) token.type = end;
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
			if (state == START || state == SPACE) token.type = number;
			state = state_trans[state][NUMBER];
		}
		else if (*it == '{' || *it == '}')
		{
			if (state == START || state == SPACE) token.type = matrix;
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
				token.type = string;
				str_sign = *it;
			}
			if (*it != str_sign && state == STRING)
				state = state_trans[state][OPTION];
			else
				state = state_trans[state][STRING];
			if (state == END && *it == str_sign) ++it;
		}
		else if (isoption(*it))
		{
			if (isoperation(token.value, *it))
			{
				if (state == START || state == SPACE) token.type = option;
				state = OPTION;
			}
			else
			{
				if (state == START || state == SPACE) token.type = option;
				if (*it == '(' && ((state == VARIABLE && token.type == variable) || (state == KEYWORD && !isKeyword(token.value))))
				{
					token.type = function;
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
				if (state == START || state == SPACE) token.type = keyword;
				state = state_trans[state][KEYWORD];
			}
			else
			{
				if (state == START || state == SPACE || state == KEYWORD) token.type = variable;
				state = state_trans[state][VARIABLE];
			}
		}
		else if (*it == ' ' || *it == ',')
		{
			state = state_trans[state][SPACE];
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
			(state != STRING && state != START && state != SPACE && *it != ' ')) token.value.push_back(*it);
		last_sign = *it;
		++it;
	}
	if (state == ERROR)token.type = error;
	if (it == str.end()) lex_end = true;
	if (left_bracks_num != 0) token.type = error;
	if (token.type == keyword && !isKeyword(token.value)) token.type = variable;
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
		if (token.type == error) break;
		if (token.type == function) tokens.emplace_back(Token(option, "("));
	}
	return tokens;
}

