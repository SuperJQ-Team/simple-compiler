#include "Function.h"
#include "Executer.h"


void Function::pushInstruction(const std::vector<Token>& tokens)
{
	instructions.emplace_back(tokens);
}

Variable Function::run(const std::vector<Variable>& args, Executer* parent)
{
	// 先创建instruction的备份，然后往备份里放各个argument的定义
	// 然后ext逐tokens执行备份内容
	Executer ext(parent);

	if (args.size() != arguments.size())
	{
		// raise error
	}

	for (int i = 0; i < arguments.size(); ++i)
	{
		ext.RegisterVarialbe(arguments[i], args[i]);
	}
	for (auto& tokens : instructions)
	{
		auto v = ext.Execute(tokens);
		if (v.type != __Variable::_error && v.type != __Variable::_null)return v;
	}
	return Variable::nul;
}

Variable WelcomeFunc::run(const std::vector<Variable>& args, Executer* parent)
{
	UI::PrintLog("Calculator v1.1. Licensed by GPL v4.0.\n\n");
	UI::PrintLog("Type \"help()\" for more info.\n");
	return Variable::nul;
}

Variable PrintFunc::run(const std::vector<Variable>& args, Executer* parent)
{
	bool first = true;
	for (auto& arg : args)
	{
		if (!first) UI::PrintLog(", ");
		first = false;
		UI::Print(arg);
	}
	return Variable::nul;
}