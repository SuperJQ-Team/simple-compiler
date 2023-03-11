#include "Parser.h"
#include "Logger.h"

using namespace __Parser;

char sign[] = { '+','-','*','/','%','<','>','=','(',')','[',']','{','}',
						'!','@','#','$','^','&','~','?',':',';','\'','"','\\','|' };

std::string optsig[] = { "+","-","*","/","%",">","<","=","(",")","[","]","{","}"
					,"!","^","&","|","~","?",":",",","'","\"",
					">=","<=","==","!=","||","&&",">>","<<",
					"+=","-=","*=","/=","&=","|=","~=","^=",
					"**","<<=",">>=" };
std::string defsgn[] = { "let", "def", "end" };

bool isoption(char c)
{
	for (char x : sign)
		if (c == x)return true;
	return false;
}

bool isoperation(std::string last, char c)
{
	last.push_back(c);
	for (std::string x : optsig)
		if (last == x)return true;
	return false;
}

bool isDefineSign(const std::string& last_sign, char c)
{
	std::string temp = last_sign + c;
	for (std::string s : defsgn)
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

Token::Token() : type(error)
{

}
Token::Token(TokenType type, const std::string& value) : type(type), value(value)
{

}


OptionType __Parser::GetOptType(std::string c)
{
	if (c == "+") return plus;
	if (c == "-") return minus;
	if (c == "*") return times;
	if (c == "/") return division;
	if (c == "%") return modulo;

	if (c == "(") return left_brack;
	if (c == ")") return right_brack;

	if (c == "&") return bitands;
	if (c == "|") return bitors;
	if (c == "^") return xors;
	if (c == "~") return bitnot;
	if (c == ">>") return right_move;
	if (c == "<<") return left_move;

	if (c == "==")return equal;
	if (c == ">")return bigger;
	if (c == "<")return lower;
	if (c == ">=")return bigorequ;
	if (c == "<=")return loworequ;
	if (c == "!=")return notequal;

	if (c == "&&")return logicand;
	if (c == "||")return logicor;
	if (c == "!")return logicnot;

	if (c == "=") return is;
	if (c == "+=") return plusis;
	if (c == "-=") return minusis;
	if (c == "*=") return timesis;
	if (c == "/=") return divisionis;
	if (c == "%=") return modulois;
	if (c == "&=") return andis;
	if (c == "|=") return oris;
	if (c == "^=") return xoris;
	if (c == "~=") return notis;
	if (c == "<<=")return left_moveis;
	if (c == ">>=")return right_moveis;

	if (c == "[")return left_block_brack;
	if (c == "]")return right_block_brack;

	if (c == "?")return question;
	if (c == ":")return colon;

	if (c == "**")return power;

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
		DEFINE = 8,
		MATRIX = 9,
		FINISH = 10,

	};
	State state_trans[255][255] = {
		{START,		ERROR,	END,	NUMBER,		OPTION, VARIABLE,	STRING, SPACE,	DEFINE, MATRIX},
		{ERROR,		ERROR,	ERROR,	ERROR,		ERROR,	ERROR,		ERROR,	ERROR,	ERROR,	ERROR},
		{END,		END,	END,	END,		END,	END,		END,	END,	END,	END},
		{NUMBER,	ERROR,	END,	NUMBER,		END,	ERROR,		ERROR,	END,	ERROR,	END},
		{OPTION,	ERROR,	END,	END,		ERROR,	END,		END,	END,	ERROR,	END},
		{VARIABLE,	ERROR,	END,	VARIABLE,	END,	VARIABLE,	ERROR,	END,	ERROR,	END},
		{STRING,	ERROR,	ERROR,	STRING,		STRING,	STRING,		END,	STRING,	STRING,	STRING},
		{SPACE,		ERROR,	END,	NUMBER,		OPTION,	VARIABLE,	STRING,	SPACE,	DEFINE,	MATRIX},
		{DEFINE,	ERROR,	END,	VARIABLE,	ERROR,	VARIABLE,	ERROR,	END,	DEFINE, ERROR},
		{MATRIX,	ERROR,	END,	MATRIX,		ERROR,	ERROR,		ERROR,	MATRIX,	ERROR,	MATRIX},
	};

	std::string str;
	std::string::iterator it;

	State state;

public:
	Automaton(const std::string& str);
	Token ParseToken();
	bool parse_end;
};
Automaton::Automaton(const std::string& str)
{
	this->str = str;
	this->str.push_back('\0');
	this->it = this->str.begin();
	parse_end = false;
}
Token Automaton::ParseToken()
{
	char str_sign = -1;
	char last_sign = -1;
	int left_bracks_num = 0;
	state = START;
	Token token;
	while (it != str.end())
	{
		if (*it == '\0' || *it == ';')
		{
			if (state == START || state == SPACE) token.type = end;
			state = state_trans[state][END];
			++it;
			break;
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
				if (*it == '(' && state == VARIABLE && token.type == variable)
				{
					token.type = function;
					++it;
				}
				if ((*it == '(' && state == OPTION) || last_sign == ')')state = END;
				else state = state_trans[state][OPTION];
			}
		}
		else if (isalpha(*it) || *it == '_')
		{
			if (isDefineSign(token.value, *it))
			{
				if (state == START || state == SPACE) token.type = define;
				state = state_trans[state][DEFINE];
			}
			else
			{
				if (state == START || state == SPACE || state == DEFINE) token.type = variable;
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
	if (it == str.end()) parse_end = true;
	if (left_bracks_num != 0) token.type = error;
	return token;
}


std::vector<Token> Parser::getTokens(const std::string& input_string)
{
	std::vector<Token> tokens;
	if (input_string.empty())return tokens;
	Automaton automaton(input_string);
	while (automaton.parse_end == false)
	{
		Token token = automaton.ParseToken();
		tokens.emplace_back(token);
		if (token.type == error) break;
		if (token.type == function) tokens.emplace_back(Token(option, "("));
	}
	return tokens;
}

