#include "Executer.h"

#include <stack>
#include "Logger.h"
#include "UI.h"
#include "Matrix.h"

int GetPriority(const std::string& opt)
{
	__Lexer::OptionType opty = __Lexer::GetOptType(opt);
	switch (opty)
	{
	case __Lexer::error_option:
		return -1;
	case __Lexer::plus:
		return 10;
	case __Lexer::minus:
		return 10;
	case __Lexer::times:
		return 11;
	case __Lexer::division:
		return 11;
	case __Lexer::modulo:
		return 11;
	case __Lexer::left_brack:
		return 255;
	case __Lexer::right_brack:
		return 255;
	case __Lexer::bitands:
		return 6;
	case __Lexer::bitors:
		return 4;
	case __Lexer::xors:
		return 5;
	case __Lexer::bitnot:
		return 13;
	case __Lexer::right_move:
		return 9;
	case __Lexer::left_move:
		return 9;
	case __Lexer::equal:
		return 7;
	case __Lexer::bigger:
		return 8;
	case __Lexer::lower:
		return 8;
	case __Lexer::bigorequ:
		return 8;
	case __Lexer::loworequ:
		return 8;
	case __Lexer::notequal:
		return 7;
	case __Lexer::logicand:
		return 3;
	case __Lexer::logicor:
		return 2;
	case __Lexer::logicnot:
		return 13;
	case __Lexer::is:
		return 0;
	case __Lexer::plusis:
		return 0;
	case __Lexer::minusis:
		return 0;
	case __Lexer::timesis:
		return 0;
	case __Lexer::divisionis:
		return 0;
	case __Lexer::modulois:
		return 0;
	case __Lexer::andis:
		return 0;
	case __Lexer::oris:
		return 0;
	case __Lexer::xoris:
		return 0;
	case __Lexer::notis:
		return 0;
	case __Lexer::left_block_brack:
		return 14;
	case __Lexer::right_block_brack:
		return 14;
	case __Lexer::left_moveis:
		return 0;
	case __Lexer::right_moveis:
		return 0;
	case __Lexer::question:
		return 1;
	case __Lexer::colon:
		return 1;
	case __Lexer::power:
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
		if (tokens[index].type == __Lexer::end)
		{
			break;
		}
		if (tokens[index].type == __Lexer::option)
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
		if (it->type == __Lexer::option)
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
	if (vars.size() != 1) return Variable::err;
	if (vars.top().type == __Variable::_varible)return GetValue(*(std::string*)vars.top().value);
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
		if (v1.type != __Variable::_varible) return Variable::err;
		if (v2.type == __Variable::_varible) v2 = GetValue(*(std::string*)(v2.value));
		if (opt == "=")GetValue(*(std::string*)(v1.value)) = v2;
		else
		{
			return Variable::err;
		}
		return _v1;
	}
	Variable v;
	if (v1.type == __Variable::_varible) v1 = GetValue(*(std::string*)(v1.value));
	if (v2.type == __Variable::_varible) v2 = GetValue(*(std::string*)(v2.value));
	if (v1.type == __Variable::_int && v2.type == __Variable::_int)
	{
		int x1 = *(int*)v1.value, x2 = *(int*)v2.value;

		__Lexer::OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case __Lexer::error_option:
			v.type = __Variable::_error;
			return v;
			break;
		case __Lexer::plus:
			v.type = __Variable::_int;
			v.value = new int(x1 + x2);
			break;
		case __Lexer::minus:
			v.type = __Variable::_int;
			v.value = new int(x1 - x2);
			break;
		case __Lexer::times:
			v.type = __Variable::_int;
			v.value = new int(x1 * x2);
			break;
		case __Lexer::division:
			v.type = __Variable::_int;
			v.value = new int(x1 / x2);
			break;
		case __Lexer::modulo:
			v.type = __Variable::_int;
			v.value = new int(x1 % x2);
			break;
		case __Lexer::bitands:
			v.type = __Variable::_int;
			v.value = new int(x1 & x2);
			break;
		case __Lexer::bitors:
			v.type = __Variable::_int;
			v.value = new int(x1 | x2);
			break;
		case __Lexer::xors:
			v.type = __Variable::_int;
			v.value = new int(x1 ^ x2);
			break;
		case __Lexer::right_move:
			v.type = __Variable::_int;
			v.value = new int(x1 >> x2);
			break;
		case __Lexer::left_move:
			v.type = __Variable::_int;
			v.value = new int(x1 << x2);
			break;
		case __Lexer::equal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 == x2);
			break;
		case __Lexer::bigger:
			v.type = __Variable::_bool;
			v.value = new bool(x1 > x2);
			break;
		case __Lexer::lower:
			v.type = __Variable::_bool;
			v.value = new bool(x1 < x2);
			break;
		case __Lexer::bigorequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 >= x2);
			break;
		case __Lexer::loworequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 <= x2);
			break;
		case __Lexer::notequal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 != x2);
			break;
		case __Lexer::power:
			v.type = __Variable::_int;
			v.value = new int(pow(x1, x2));
			break;
		default:
			v.type = __Variable::_error;
			return v;
		}

	}
	else if ((v1.type == __Variable::_float || v1.type == __Variable::_int) && (v2.type == __Variable::_float || v2.type == __Variable::_int))
	{
		double x1 = 0, x2 = 0;
		if (v1.type == __Variable::_float)x1 = *(double*)v1.value;
		else x1 = (double)*(int*)v1.value;
		if (v2.type == __Variable::_float)x2 = *(double*)v2.value;
		else x2 = (double)*(int*)v2.value;

		__Lexer::OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case __Lexer::error_option:
			v.type = __Variable::_error;
			return v;
		case __Lexer::plus:
			v.type = __Variable::_float;
			v.value = new double(x1 + x2);
			break;
		case __Lexer::minus:
			v.type = __Variable::_float;
			v.value = new double(x1 - x2);
			break;
		case __Lexer::times:
			v.type = __Variable::_float;
			v.value = new double(x1 * x2);
			break;
		case __Lexer::division:
			v.type = __Variable::_float;
			v.value = new double(x1 / x2);
			break;
		case __Lexer::equal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 == x2);
			break;
		case __Lexer::bigger:
			v.type = __Variable::_bool;
			v.value = new bool(x1 > x2);
			break;
		case __Lexer::lower:
			v.type = __Variable::_bool;
			v.value = new bool(x1 < x2);
			break;
		case __Lexer::bigorequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 >= x2);
			break;
		case __Lexer::loworequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 <= x2);
			break;
		case __Lexer::notequal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 != x2);
			break;
		case __Lexer::power:
			v.type = __Variable::_float;
			v.value = new double(pow(x1, x2));
			break;
		default:
			v.type = __Variable::_error;
			return v;
		}
	}
	else if (v1.type == __Variable::_string && v2.type == __Variable::_string)
	{
		std::string& x1 = *(std::string*)v1.value, & x2 = *(std::string*)v2.value;
		__Lexer::OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case __Lexer::error_option:
			v.type = __Variable::_error;
			return v;
		case __Lexer::plus:
			v.type = __Variable::_string;
			v.value = new std::string(x1 + x2);
			break;
		case __Lexer::equal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 == x2);
			break;
		case __Lexer::bigger:
			v.type = __Variable::_bool;
			v.value = new bool(x1 > x2);
			break;
		case __Lexer::lower:
			v.type = __Variable::_bool;
			v.value = new bool(x1 < x2);
			break;
		case __Lexer::bigorequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 >= x2);
			break;
		case __Lexer::loworequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 <= x2);
			break;
		case __Lexer::notequal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 != x2);
			break;
		default:
			v.type = __Variable::_error;
			return v;
		}
	}
	else if (v1.type == __Variable::_matrix && v2.type == __Variable::_matrix)
	{
		Matrix& x1 = *(Matrix*)v1.value, & x2 = *(Matrix*)v2.value;
		__Lexer::OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case __Lexer::error_option:
			v.type = __Variable::_error;
			return v;
		case __Lexer::plus:
			v.type = __Variable::_matrix;
			v.value = new Matrix(x1 + x2);
			break;
		case __Lexer::minus:
			v.type = __Variable::_matrix;
			v.value = new Matrix(x1 - x2);
			break;
		case __Lexer::times:
			v.type = __Variable::_matrix;
			v.value = new Matrix(x1 * x2);
			break;
		default:
			v.type = __Variable::_error;
			return v;
		}
	}
	else if ((v1.type == __Variable::_int || v1.type == __Variable::_float) && v2.type == __Variable::_matrix)
	{
		double x1;
		if (v1.type == __Variable::_int)x1 = *(int*)v1.value;
		else x1 = *(double*)v1.value;
		Matrix& x2 = *(Matrix*)v2.value;
		__Lexer::OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case __Lexer::error_option:
			v.type = __Variable::_error;
			return v;
		case __Lexer::times:
			v.type = __Variable::_matrix;
			v.value = new Matrix(x1 * x2);
			break;
		default:
			v.type = __Variable::_error;
			return v;
		}
	}
	else if (v1.type == __Variable::_bool && v2.type == __Variable::_bool)
	{
		bool x1 = *(bool*)v1.value, & x2 = *(bool*)v2.value;
		__Lexer::OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case __Lexer::error_option:
			v.type = __Variable::_error;
			return v;
		case __Lexer::logicand:
			v.type = __Variable::_bool;
			v.value = new bool(x1 && x2);
		case __Lexer::logicor:
			v.type = __Variable::_bool;
			v.value = new bool(x1 || x2);
		case __Lexer::xors:
			v.type = __Variable::_bool;
			v.value = new bool(x1 ^ x2);
		default:
			v.type = __Variable::_error;
			return v;
		}
	}
	else
	{
		v.type = __Variable::_error;
		v.value = nullptr;
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
	for (Token token : tokens)
	{
		if (token.type == __Lexer::error)
		{
			UI::PrintErr("Syntax error");
			return;
		}
	}
	if (tokens[0].type == __Lexer::keyword)
	{
		for (int i = 1; i < tokens.size(); ++i)
		{
			if (tokens[i].type == __Lexer::keyword)
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
			if (tokens.size() < 2 || tokens[1].type != __Lexer::variable)
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
			if (tokens[2].type != __Lexer::option || tokens[2].value != "=" || tokens.size() < 4)
			{
				UI::PrintDefErr(name, "Cannot find the initial value of the variable");
				return;
			}
			Variable var = Calculate(tokens, 3);
			if (var.type == __Variable::_error)
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
	if (var.type == __Variable::_error)
	{
		UI::PrintErr("Cannot calculate");
		return;
	}

	if (var.type == __Variable::_varible) UI::Print(var_map[*(std::string*)var.value]);
	else UI::Print(var);
}