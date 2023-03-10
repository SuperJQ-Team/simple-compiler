#pragma once
#include <map>

#include "Var.h"
#include "Parser.h"

class Executer
{
	enum SignType
	{
		_func = 0,
		_var = 1
	};
	Executer* father = nullptr;
	std::map<std::string, SignType> sign_map;
	std::map<std::string, Variable> var_map;
	Variable Calculate(const std::vector<Token>& tokens, int index);
	bool isOccured(const std::string& name);
public:
	Variable RunOption(const Variable& v1, const Variable& v2, const std::string& opt);
	Variable& GetValue(const std::string&);
	void Execute(const std::vector<Token>& tokens);
};