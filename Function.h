#pragma once

#include <vector>
#include <stack>

#include "Token.h"
#include "UI.h"
#include "Var.h"

class Executer;

class Function
{
protected:
	std::vector<std::vector<Token>> instructions;
	std::string name;

	std::vector<std::string> arguments;

public:

	Function() {}
	Function(std::string s, const std::vector<std::string>& argument) :name(s), arguments(argument) {}
	Function(const Function& func) : instructions(instructions), name(name), arguments(arguments) {}

	void pushInstruction(const std::vector<Token>& tokens);

	virtual Variable run(const std::stack<Variable>& args, Executer* parent = nullptr);

	static void RegisterGlobalFunc(Executer& executer);
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