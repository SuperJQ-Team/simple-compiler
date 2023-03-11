#pragma once

#include <iostream>
#include <string>
#include <vector>

class Token;
class Parser;

namespace __Parser {

	enum TokenType
	{
		error = -1,
		number = 0,
		option = 1,
		define = 2,
		variable = 3,
		string = 4,
		matrix = 5,
		function = 6,
		end = 7,
	};
	enum OptionType
	{
		error_option = -1,
		plus = 0,
		minus = 1,
		times = 2,
		division = 3,
		modulo = 4,

		left_brack = 5,
		right_brack = 6,

		bitands = 7,
		bitors = 8,
		xors = 9,
		bitnot = 10,
		right_move = 11,
		left_move = 12,

		equal = 13,
		bigger = 14,
		lower = 15,
		bigorequ = 16,
		loworequ = 17,
		notequal = 18,

		logicand = 19,
		logicor = 20,
		logicnot = 21,

		is = 30,
		plusis = 31,
		minusis = 32,
		timesis = 33,
		divisionis = 34,
		modulois = 35,
		andis = 36,
		oris = 37,
		xoris = 38,
		notis = 39,

		left_block_brack = 40,
		right_block_brack = 41,

		left_moveis = 42,
		right_moveis=43,

		question = 44,
		colon = 45,

		power = 46,
	};

	OptionType GetOptType(std::string c);
}

class Token
{
public:
	__Parser::TokenType type;
	std::string value;
	Token();
	Token(__Parser::TokenType type, const std::string& value);
};

class Parser
{
public:
	static std::vector<Token> getTokens(const std::string& input_string);
};