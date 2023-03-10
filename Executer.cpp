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

namespace __Executer {

	Varible Executer::runoption(Varible& _v1, const Varible& _v2, const std::string& opt)
	{
		Varible v1 = _v1, v2 = _v2;
		if (isvol(opt))
		{
			if (_v1._type != __Varible::_varible)return Varible::err;
			if (v2._type == __Varible::_varible) v2 = getvalue(*(std::string*)(v2._val));
			if (opt == "=")_v1 = v2;
			else
			{
				return Varible::err;
			}
			return _v1;
		}
		Varible v;
		if (v1._type == __Varible::_varible) v1 = getvalue(*(std::string*)(v1._val));
		if (v2._type == __Varible::_varible) v2 = getvalue(*(std::string*)(v2._val));
		if (v1._type == __Varible::_int && v2._type == __Varible::_int)
		{
			v._type = __Varible::_int;
			v._val = new int;
			if (opt == "+")*(int*)v._val = *(int*)v1._val + *(int*)v2._val;
			else if (opt == "-")*(int*)v._val = *(int*)v1._val - *(int*)v2._val;
			else if (opt == "*")*(int*)v._val = *(int*)v1._val * *(int*)v2._val;
			else if (opt == "/")*(int*)v._val = *(int*)v1._val / *(int*)v2._val;
			else if (opt == "%")*(int*)v._val = *(int*)v1._val % *(int*)v2._val;
			else
			{
				v._type = __Varible::_error;
				delete v._val;
				v._val = nullptr;
			}
		}
		else
		{
			v._type = __Varible::_error;
			v._val = nullptr;
		}
		return v;
	}

	Varible& Executer::getvalue(const std::string& s)
	{
		if (var_map.count(s))
			return var_map[s];
		else if (father != nullptr)return father->getvalue(s);
		else return Varible::err;
	}

	Varible Executer::execute(const std::vector<Token>& tokenstream)
	{
		bool indefine = false;
		if (tokenstream.size() >= 2)
			if (tokenstream[0].type == __Parser::define)
			{
				if (tokenstream[1].type == __Parser::variable)
					indefine = true;
				else
					return Varible::err;
				if (tokenstream.size() >= 4)
					if (tokenstream[2].type != __Parser::option || tokenstream[2].value != "=")return Varible::err;

				var_map[tokenstream[1].value] = Varible();
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
					if (opts.empty());//ERROR;
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


		std::stack<Varible> vars;
		it = backtokens.begin();
		while (it != backtokens.end())
		{
			if (it->type == __Parser::option)
			{
				if (vars.size() < 2);//ERROR;
				Varible v1, v2;
				v2 = vars.top(); vars.pop();
				v1 = vars.top(); vars.pop();
				//std::cerr << "Is executing operator " << it->value << " \n";
				vars.push(runoption(v1, v2, it->value));
			}
			else
			{
				Varible v(*it);
				vars.push(v);
			}
			++it;
		}
		if (indefine)return Varible::deflog(tokenstream[1].value);
		return vars.top();
	}
}