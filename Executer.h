#pragma once
#include <map>

#include "Var.h"
#include "Parser.h"

class Executer
{
	Executer* father = nullptr;
	std::map<std::string, Variable> var_map;
	Variable Calculate(const std::vector<Token>& tokens, int index);
public:
	Variable RunOption(const Variable& v1, const Variable& v2, const std::string& opt);
	Variable& GetValue(const std::string&);
	void Execute(const std::vector<Token>& tokens);
};