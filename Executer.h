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

	struct FuncDefVar
	{
		bool defining = false;
		int endnum = 0;
		std::string name;
		enum FuncDefType
		{
			_func = 0,
			_if = 1,
			_else = 2,
			_for = 3
		}
		type = _func;
		Variable _ifparm;
		void set(const std::string& n, FuncDefType t);
		void del();
	}funcDefVar;

	Executer* father = nullptr;
	std::map<std::string, SignType> sign_map;
	std::map<std::string, Variable> var_map;
	std::map<std::string, Function*> func_map;
	
	bool isOccured(const std::string& name);
public:
	bool ifreturn = false;
public:
	
	Executer() {}
	Executer(Executer* parent) : father(parent) {}
	~Executer();
	Variable RunOption(const Variable& v1, const Variable& v2, const std::string& opt);
	Variable Calculate(const std::vector<Token>& tokens, int index);
	Variable& GetValue(const std::string&);
	Variable& GetValue(const Variable&);
	Function* GetFunction(const std::string&);
	Variable Execute(const std::vector<Token>& tokens);
	void RegisterVarialbe(const std::string& name, const Variable& value);
	void RegisterFunction(const std::string& name, Function* func);
};