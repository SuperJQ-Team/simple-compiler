#pragma once

#include <iostream>
#include <string>
#include <vector>

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
		function = 6
	};
	enum OptionType
	{
		error_option = -1, plus = 0, minus = 1, times = 2, division = 3, modulo = 4, left_brack = 5, right_brack = 6
	};

	class Token
	{
	public:
		TokenType type;
		std::string value;
	};

	class Parser
	{
	public:
		static std::vector<Token> getTokens(const std::string& input_string);
	};
	void PrintTokens(const std::vector<Token>& tokens);
}