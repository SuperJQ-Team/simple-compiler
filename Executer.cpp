#include "Executer.h"
#include "Logger.h"
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

	Variable Executer::Execute(const std::vector<Token>& tokenstream)
	{
		bool indefine = false;
		if (tokenstream.size() >= 2)
			if (tokenstream[0].type == __Parser::define && tokenstream[0].value == "let")
			{
				if (tokenstream[1].type == __Parser::variable)
					indefine = true;
				else
					return Variable::err;
				if (tokenstream.size() >= 4)
					if (tokenstream[2].type != __Parser::option || tokenstream[2].value != "=")return Variable::err;

				var_map[tokenstream[1].value] = Variable();
			}
		std::stack<Token> opts;
		std::vector<Token> backtokens;
		auto it = tokenstream.begin();
		if (indefine)++it;
		while (it != tokenstream.end())
		{
			if (it->type == __Parser::option)
			{
				if (it->value == ")")
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
					while (!opts.empty() && opts.top().value != "(" && GetPriority(opts.top().value) >= GetPriority(it->value))
					{
						//std::cerr << "Pop operator " << opts.top().value << " \n";
						backtokens.emplace_back(opts.top());
						opts.pop();
					}
					opts.push(*it);
				}
			}
			else
			{
				backtokens.emplace_back(*it);
			}
			++it;
		}
		while (!opts.empty()) { backtokens.emplace_back(opts.top()); opts.pop(); };


		std::stack<Variable> vars;
		it = backtokens.begin();
		while (it != backtokens.end())
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
			++it;
		}
		if (indefine) return Variable::deflog(tokenstream[1].value);
		return vars.top();
	}