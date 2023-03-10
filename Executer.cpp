#include "Executer.h"
#include "Logger.h"
#include "UI.h"
#include <stack>

int GetPriority(const std::string& opt)
{
	if (opt == "+")return 0;
	if (opt == "-")return 0;
	if (opt == "*")return 1;
	if (opt == "/")return 1;
	if (opt == "%")return 2;
	if (opt == "(")return 255;
	return -1;
}

inline bool isvol(const std::string s)
{
	return s == "=" || s == "+=" || s == "-=" || s == "*=" || s == "/=" || s == "%=" || s == "&=" || s == "|=" || s == "^=";
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
	
	Variable var =  Calculate(tokens, 0);
	if (var._type == __Variable::_error)
	{
		UI::PrintErr("Cannot calculate");
		return;
	}

	if (var._type == __Variable::_varible) UI::Print(var_map[*(std::string*)var._val]);
	else UI::Print(var);
}