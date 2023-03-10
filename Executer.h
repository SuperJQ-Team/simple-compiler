#pragma once
#include <map>

#include "Var.h"
#include "Parser.h"

namespace __Executer {

	using __Parser::Token;
	using __Parser::Parser;
	using __Varible::Varible;
	using __Parser::OptionType;


	class Executer
	{
		Executer* father = nullptr;
		std::map<std::string, Varible> var_map;
	public:
		Varible runoption(Varible& v1, const Varible& v2, const std::string& opt);
		Varible& getvalue(const std::string&);
		Varible execute(const std::vector<Token>& tokens);
	};
}
