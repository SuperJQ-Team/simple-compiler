#include "Parser.h"
#include "Logger.h"

char sign[] = { '+','-','*','/','%','<','>','=','(',')','[',']','{','}',
						'!','@','#','$','^','&','~','?',':',';','\'','"','\\','|' };

std::string optsig[] = { "+","-","*","/","%",">","<","=","(",")","[","]","{","}"
					,"!","^","&","|","~","?",":",";",",","'","\"","."
					">=","<=","==","!=","||","&&",
					"+=","-=","*=","/=","&=","|=","~=","^=",
					"++","--","->","::" };
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

__Parser::OptionType GetOptType(std::string c)
{
	if (c == "+") return __Parser::plus;
	if (c == "-") return __Parser::minus;
	if (c == "*") return __Parser::times;
	if (c == "/") return __Parser::division;
	if (c == "%") return __Parser::modulo;
	if (c == "(") return __Parser::left_brack;
	if (c == ")") return __Parser::right_brack;
	return __Parser::error_option;
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

	};
	State state_trans[255][255] = {
		{START,		ERROR,	END,	NUMBER,		OPTION, VARIABLE,	STRING, SPACE,	DEFINE, MATRIX},
		{ERROR,		ERROR,	ERROR,	ERROR,		ERROR,	ERROR,		ERROR,	ERROR,	ERROR,	ERROR},
		{END,		END,	END,	END,		END,	END,		END,	END,	END,	END},
		{NUMBER,	ERROR,	END,	NUMBER,		END,	ERROR,		ERROR,	END,	ERROR,	END},
		{OPTION,	ERROR,	END,	END,		ERROR,	END,		STRING,	END,	ERROR,	END},
		{VARIABLE,	ERROR,	END,	VARIABLE,	END,	VARIABLE,	STRING,	END,	ERROR,	END},
		{STRING,	ERROR,	END,	STRING,		STRING,	STRING,		END,	STRING,	STRING,	STRING},
		{SPACE,		ERROR,	END,	NUMBER,		OPTION,	VARIABLE,	STRING,	SPACE,	DEFINE,	MATRIX},
		{DEFINE,	ERROR,	END,	VARIABLE,	ERROR,	VARIABLE,	ERROR,	END,	DEFINE, ERROR},
		{MATRIX,	ERROR,	END,	MATRIX,		ERROR,	ERROR,		ERROR,	MATRIX,	ERROR,	MATRIX}
	};

	std::string str;
	std::string::iterator it;

	State state;

public:
	Automaton(const std::string& str);
	__Parser::Token ParseToken();
	bool parse_end;
};
Automaton::Automaton(const std::string& str)
{
	this->str = str;
	this->it = this->str.begin();
	parse_end = false;
}
__Parser::Token Automaton::ParseToken()
{
	char str_sign = '\0';
	char last_sign = '\0';
	int left_bracks_num = 0;
	state = START;
	__Parser::Token token;
	while (it != str.end())
	{
		if (isdigit(*it) || *it == '.')
		{
			if (state == START || state == SPACE) token.type = __Parser::number;
			state = state_trans[state][NUMBER];
		}
		else if (*it == '{' || *it == '}')
		{
			if (state == START || state == SPACE) token.type = __Parser::matrix;
			if (*it == '{') ++left_bracks_num;
			else if (*it == '}')
			{
				if (left_bracks_num == 0) state = ERROR;
				--left_bracks_num;
			}
			if (state != END && state != ERROR) state = MATRIX;
		}
		else if (*it == '"' || *it == '\'')
		{
			if (state == START || state == SPACE)
			{
				token.type = __Parser::string;
				str_sign = *it;
			}
			if (*it != str_sign)
				state = state_trans[state][OPTION];
			else
				state = state_trans[state][STRING];
			if (state == END) ++it;
		}
		else if (isoption(*it))
		{
			if (isoperation(token.value, *it))
			{
				if (state == START || state == SPACE) token.type = __Parser::option;
				state = OPTION;
			}
			else
			{
				if (state == START || state == SPACE) token.type = __Parser::option;
				if (*it == '(' && state == VARIABLE && token.type == __Parser::variable)
				{
					token.type = __Parser::function;
					it++;
				}
				if ((*it == '(' && state == OPTION) || last_sign == ')')state = END;
				else state = state_trans[state][OPTION];
			}
		}
		else if (isalpha(*it) || *it == '_')
		{
			if (isDefineSign(token.value, *it))
			{
				if (state == START || state == SPACE) token.type = __Parser::define;
				state = state_trans[state][DEFINE];
			}
			else
			{
				if (state == START || state == SPACE || state == DEFINE) token.type = __Parser::variable;
				state = state_trans[state][VARIABLE];
			}
		}
		else if (*it == ' ' || *it == ',')
		{
			state = state_trans[state][SPACE];
		}

		if (state == ERROR)
		{
			token.type = __Parser::error;
			return token;
		}

		if (state == END)
		{
			break;
		}
		if ((state == STRING && *it != str_sign) || (state != STRING && state != START && state != SPACE && *it != ' ')) token.value.push_back(*it);
		last_sign = *it;
		++it;
	}
	if (it == str.end()) parse_end = true;
	if (left_bracks_num != 0) token.type = __Parser::error;
	return token;
}


namespace __Parser {
	std::vector<Token> Parser::getTokens(const std::string& input_string)
	{
		std::vector<Token> tokens;
		Automaton automaton(input_string);
		while (automaton.parse_end == false)
		{
			Token token = automaton.ParseToken();
			tokens.emplace_back(token);
			if (token.type == error) break;
		}
		return tokens;
	}
	void PrintTokens(const std::vector<Token>& tokens)
	{
		auto getType = [](TokenType t) {
			if (t == error) return "error";
			if (t == number) return "number";
			if (t == option) return "option";
			if (t == define) return "define";
			if (t == variable) return "variable";
			if (t == string) return "string";
			if (t == matrix) return "matrix";
			if (t == function)return "function";
			return "unknown";
		};
		for (Token token : tokens)
		{
			std::cout << getType(token.type) << ", " << token.value << '\n';
		}
	}
}