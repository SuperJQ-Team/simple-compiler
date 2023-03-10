#pragma once
#include <map>

#include "Var.h"
#include "Parser.h"

class Executer
{
	Executer* father = nullptr;
	std::map<std::string, Variable> var_map;
public:
	Variable RunOption(const Varible& v1, const Varible& v2, const std::string& opt);
	Variable& GetValue(const std::string&);
	Variable Execute(const std::vector<Token>& tokens);
};