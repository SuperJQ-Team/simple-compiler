#include "Executer.h"
#include "Logger.h"
#include "UI.h"
#include <stack>

int GetPriority(const std::string& opt)
{
	__Parser::OptionType opty = __Parser::GetOptType(opt);
	switch (opty)
	{
	case __Parser::error_option:
		return -1;
	case __Parser::plus:
		return 10;
	case __Parser::minus:
		return 10;
	case __Parser::times:
		return 11;
	case __Parser::division:
		return 11;
	case __Parser::modulo:
		return 11;
	case __Parser::left_brack:
		return 255;
	case __Parser::right_brack:
		return 255;
	case __Parser::bitands:
		return 6;
	case __Parser::bitors:
		return 4;
	case __Parser::xors:
		return 5;
	case __Parser::bitnot:
		return 13;
	case __Parser::right_move:
		return 9;
	case __Parser::left_move:
		return 9;
	case __Parser::equal:
		return 7;
	case __Parser::bigger:
		return 8;
	case __Parser::lower:
		return 8;
	case __Parser::bigorequ:
		return 8;
	case __Parser::loworequ:
		return 8;
	case __Parser::notequal:
		return 7;
	case __Parser::logicand:
		return 3;
	case __Parser::logicor:
		return 2;
	case __Parser::logicnot:
		return 13;
	case __Parser::is:
		return 0;
	case __Parser::plusis:
		return 0;
	case __Parser::minusis:
		return 0;
	case __Parser::timesis:
		return 0;
	case __Parser::divisionis:
		return 0;
	case __Parser::modulois:
		return 0;
	case __Parser::andis:
		return 0;
	case __Parser::oris:
		return 0;
	case __Parser::xoris:
		return 0;
	case __Parser::notis:
		return 0;
	case __Parser::left_block_brack:
		return 14;
	case __Parser::right_block_brack:
		return 14;
	case __Parser::left_moveis:
		return 0;
	case __Parser::right_moveis:
		return 0;
	case __Parser::question:
		return 1;
	case __Parser::colon:
		return 1;
	case __Parser::power:
		return 12;
	default:
		return -1;
	}
	return -1;
}

inline bool isvol(const std::string s)
{
	return s == "=" || s == "+=" || s == "-=" ||
		s == "*=" || s == "/=" || s == "%=" ||
		s == "&=" || s == "|=" || s == "^=" || s == "~=" ||
		s == "<<=" || s == ">>=";

}

Variable Executer::Calculate(const std::vector<Token>& tokens, int index)
{
	std::stack<Token> opts;
	std::vector<Token> backtokens;
	for (; index < tokens.size(); ++index)
	{
		if (tokens[index].type == __Parser::option)
		{
			if (tokens[index].value == ")")
			{
				while (!opts.empty() && opts.top().value != "(")
				{
					backtokens.emplace_back(opts.top());
					opts.pop();
				}
				if (opts.empty())return Variable::err;//ERROR;
				opts.pop();
			}
			else
			{
				//std::cerr << "Push operator " << it->value << " \n";
				while (!opts.empty() && opts.top().value != "(" && GetPriority(opts.top().value) >= GetPriority(tokens[index].value))
				{
					//std::cerr << "Pop operator " << opts.top().value << " \n";
					backtokens.emplace_back(opts.top());
					opts.pop();
				}
				opts.push(tokens[index]);
			}
		}
		else
		{
			backtokens.emplace_back(tokens[index]);
		}
	}
	while (!opts.empty()) { backtokens.emplace_back(opts.top()); opts.pop(); };


	std::stack<Variable> vars;
	for (auto it = backtokens.begin(); it != backtokens.end(); ++it)
	{
		if (it->type == __Parser::option)
		{
			if (vars.size() < 2)return Variable::err;//ERROR;
			Variable v1, v2;
			v2 = vars.top(); vars.pop();
			v1 = vars.top(); vars.pop();
			//std::cerr << "Is executing operator " << it->value << " \n";
			vars.push(RunOption(v1, v2, it->value));
		}
		else
		{
			Variable v(*it);
			vars.push(v);
		}
	}
	if (vars.top()._type == __Variable::_varible)return GetValue(*(std::string*)vars.top()._val);
	return vars.top();
}
bool Executer::isOccured(const std::string& name)
{
	if (sign_map.find(name) != sign_map.end()) return true;
	return false;
}

Variable Executer::RunOption(const Variable& _v1, const Variable& _v2, const std::string& opt)
{
	Variable v1 = _v1, v2 = _v2;
	if (isvol(opt))
	{
		if (v1._type != __Variable::_varible) return Variable::err;
		if (v2._type == __Variable::_varible) v2 = GetValue(*(std::string*)(v2._val));
		if (opt == "=")GetValue(*(std::string*)(v1._val)) = v2;
		else
		{
			return Variable::err;
		}
		return _v1;
	}
	Variable v;
	if (v1._type == __Variable::_varible) v1 = GetValue(*(std::string*)(v1._val));
	if (v2._type == __Variable::_varible) v2 = GetValue(*(std::string*)(v2._val));
	if (v1._type == __Variable::_int && v2._type == __Variable::_int)
	{
		v._type = __Variable::_int;
		v._val = new int;
		if (opt == "+")*(int*)v._val = *(int*)v1._val + *(int*)v2._val;
		else if (opt == "-")*(int*)v._val = *(int*)v1._val - *(int*)v2._val;
		else if (opt == "*")*(int*)v._val = *(int*)v1._val * *(int*)v2._val;
		else if (opt == "/")*(int*)v._val = *(int*)v1._val / *(int*)v2._val;
		else if (opt == "%")*(int*)v._val = *(int*)v1._val % *(int*)v2._val;
		else
		{
			v._type = __Variable::_error;
			delete v._val;
			v._val = nullptr;
		}
	}
	else
	{
		v._type = __Variable::_error;
		v._val = nullptr;
	}
	return v;
}

Variable& Executer::GetValue(const std::string& s)
{
	if (var_map.find(s) != var_map.end())
		return var_map[s];
	else if (father != nullptr)return father->GetValue(s);
	else return Variable::err;
}

void Executer::Execute(const std::vector<Token>& tokens)
{
	if (tokens.size() <= 0) return;
	if (tokens[0].type == __Parser::define)
	{
		for (int i = 1; i < tokens.size(); ++i)
		{
			if (tokens[i].type == __Parser::define)
			{
				UI::PrintErr("Format error");
				return;
			}
		}
		if (tokens[0].value == "end")
		{
			UI::PrintErr("\"End\" should not exist at there");
			return;
		}

		if (tokens[0].value == "let")
		{
			if (tokens.size() < 2 || tokens[1].type != __Parser::variable)
			{
				UI::PrintErr("Cannot find the variable name");
				return;
			}
			const std::string& name = tokens[1].value;

			if (isOccured(name))
			{
				UI::PrintDefErr(tokens[1].value, "This sign is already existed");
				return;
			}
			if (tokens.size() < 3)
			{
				sign_map[name] = _var;
				var_map[name] = Variable::nul;
				UI::PrintDefVar(name);
				return;
			}
			if (tokens[2].type != __Parser::option || tokens[2].value != "=" || tokens.size() < 4)
			{
				UI::PrintDefErr(name, "Cannot find the initial value of the variable");
				return;
			}
			Variable var = Calculate(tokens, 3);
			if (var._type == __Variable::_error)
			{
				UI::PrintDefErr(name, "Initial error");
				return;
			}
			sign_map[name] = _var;
			var_map[name] = var;
			UI::PrintDefVar(name);
			return;
		}
		else if (tokens[0].value == "def")
		{

		}
		else
		{
			UI::PrintErr("Undefined define");
			return;
		}
	}

	Variable var = Calculate(tokens, 0);
	if (var._type == __Variable::_error)
	{
		UI::PrintErr("Cannot calculate");
		return;
	}

	if (var._type == __Variable::_varible) UI::Print(var_map[*(std::string*)var._val]);
	else UI::Print(var);
}