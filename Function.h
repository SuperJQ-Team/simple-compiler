#pragma once

#include <vector>
#include <stack>

#include "Token.h"
#include "UI.h"
#include "Var.h"
class Executer;

enum class FuncType
{
	_func,
	_if,
	_for,
};

class Function
{
protected:
	std::vector<std::vector<Token>> instructions;
	std::string name;

	std::vector<std::string> arguments;

public:
	FuncType functype = FuncType::_func;
	Function() {}
	Function(std::string s, const std::vector<std::string>& argument) :name(s), arguments(argument) {}
	Function(const Function& func) : instructions(instructions), name(name), arguments(arguments) {}

	void pushInstruction(const std::vector<Token>& tokens);

	virtual Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);

	static void RegisterGlobalFunc(Executer& executer);
};

class ForFunc :public Function
{
private:
	std::vector<Token> init, judge, pass;
public:
	ForFunc(const std::vector<Token>& init, const std::vector<Token>& judge, const std::vector<Token>& pass);
	Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);
};

class WelcomeFunc : public Function
{
public:
	Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);
};

class PrintFunc : public Function
{
public:
	Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);
};

class PrintLnFunc : public Function
{
public:
	Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);
};


class HelpFunc : public Function
{
public:
	Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);
};

class DetFunc : public Function
{
public:
	Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);
};

class EFunc : public Function
{
public:
	Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);
};

class TFunc : public Function
{
public:
	Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);
};
