#pragma once
#include <map>

#include "Var.h"
#include "Lexer.h"


class Function;

class Executer
{
	enum SignType
	{
		_keyword = 0,
		_func = 1,
		_var = 2
	};
	Executer* father = nullptr;
	std::map<std::string, SignType> sign_map;
	std::map<std::string, Variable> var_map;
	std::map<std::string, Function*> func_map;
	Variable Calculate(const std::vector<Token>& tokens, int index);
	bool isOccured(const std::string& name);
	bool definingfunc = false;
	std::string tempstr;
public:
	Executer() {}
	Executer(Executer* parent) : father(parent) {}
	~Executer();
	Variable RunOption(const Variable& v1, const Variable& v2, const std::string& opt);
	Variable& GetValue(const std::string&);
	Variable& GetValue(const Variable&);
	Function* GetFunction(const std::string&);
	Variable Execute(const std::vector<Token>& tokens);
	void RegisterVarialbe(const std::string& name, const Variable& value);
	void RegisterFunction(const std::string& name, Function* func);
};