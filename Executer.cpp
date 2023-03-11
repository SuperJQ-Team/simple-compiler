#include "Executer.h"

#include <stack>
#include "Logger.h"
#include "UI.h"
#include "Matrix.h"

int GetPriority(const std::string& opt)
{
	OptionType opty = __Lexer::GetOptType(opt);
	switch (opty)
	{
	case OptionType::error_option:
		return -1;
	case OptionType::plus:
		return 10;
	case OptionType::minus:
		return 10;
	case OptionType::times:
		return 11;
	case OptionType::division:
		return 11;
	case OptionType::modulo:
		return 11;
	case OptionType::left_brack:
		return 255;
	case OptionType::right_brack:
		return 255;
	case OptionType::bitands:
		return 6;
	case OptionType::bitors:
		return 4;
	case OptionType::xors:
		return 5;
	case OptionType::bitnot:
		return 13;
	case OptionType::right_move:
		return 9;
	case OptionType::left_move:
		return 9;
	case OptionType::equal:
		return 7;
	case OptionType::bigger:
		return 8;
	case OptionType::lower:
		return 8;
	case OptionType::bigorequ:
		return 8;
	case OptionType::loworequ:
		return 8;
	case OptionType::notequal:
		return 7;
	case OptionType::logicand:
		return 3;
	case OptionType::logicor:
		return 2;
	case OptionType::logicnot:
		return 13;
	case OptionType::is:
		return 0;
	case OptionType::plusis:
		return 0;
	case OptionType::minusis:
		return 0;
	case OptionType::timesis:
		return 0;
	case OptionType::divisionis:
		return 0;
	case OptionType::modulois:
		return 0;
	case OptionType::andis:
		return 0;
	case OptionType::oris:
		return 0;
	case OptionType::xoris:
		return 0;
	case OptionType::notis:
		return 0;
	case OptionType::left_block_brack:
		return 14;
	case OptionType::right_block_brack:
		return 14;
	case OptionType::left_moveis:
		return 0;
	case OptionType::right_moveis:
		return 0;
	case OptionType::question:
		return 1;
	case OptionType::colon:
		return 1;
	case OptionType::power:
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
		if (tokens[index].type == TokenType::End)
		{
			break;
		}
		if (tokens[index].type == TokenType::Operator)
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
		if (it->type == TokenType::Operator)
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

		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
			break;
		case OptionType::plus:
			v.type = __Variable::_int;
			v.value = new int(x1 + x2);
			break;
		case OptionType::minus:
			v.type = __Variable::_int;
			v.value = new int(x1 - x2);
			break;
		case OptionType::times:
			v.type = __Variable::_int;
			v.value = new int(x1 * x2);
			break;
		case OptionType::division:
			v.type = __Variable::_int;
			v.value = new int(x1 / x2);
			break;
		case OptionType::modulo:
			v.type = __Variable::_int;
			v.value = new int(x1 % x2);
			break;
		case OptionType::bitands:
			v.type = __Variable::_int;
			v.value = new int(x1 & x2);
			break;
		case OptionType::bitors:
			v.type = __Variable::_int;
			v.value = new int(x1 | x2);
			break;
		case OptionType::xors:
			v.type = __Variable::_int;
			v.value = new int(x1 ^ x2);
			break;
		case OptionType::right_move:
			v.type = __Variable::_int;
			v.value = new int(x1 >> x2);
			break;
		case OptionType::left_move:
			v.type = __Variable::_int;
			v.value = new int(x1 << x2);
			break;
		case OptionType::equal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 == x2);
			break;
		case OptionType::bigger:
			v.type = __Variable::_bool;
			v.value = new bool(x1 > x2);
			break;
		case OptionType::lower:
			v.type = __Variable::_bool;
			v.value = new bool(x1 < x2);
			break;
		case OptionType::bigorequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 >= x2);
			break;
		case OptionType::loworequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 <= x2);
			break;
		case OptionType::notequal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 != x2);
			break;
		case OptionType::power:
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

		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
		case OptionType::plus:
			v.type = __Variable::_float;
			v.value = new double(x1 + x2);
			break;
		case OptionType::minus:
			v.type = __Variable::_float;
			v.value = new double(x1 - x2);
			break;
		case OptionType::times:
			v.type = __Variable::_float;
			v.value = new double(x1 * x2);
			break;
		case OptionType::division:
			v.type = __Variable::_float;
			v.value = new double(x1 / x2);
			break;
		case OptionType::equal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 == x2);
			break;
		case OptionType::bigger:
			v.type = __Variable::_bool;
			v.value = new bool(x1 > x2);
			break;
		case OptionType::lower:
			v.type = __Variable::_bool;
			v.value = new bool(x1 < x2);
			break;
		case OptionType::bigorequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 >= x2);
			break;
		case OptionType::loworequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 <= x2);
			break;
		case OptionType::notequal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 != x2);
			break;
		case OptionType::power:
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
		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
		case OptionType::plus:
			v.type = __Variable::_string;
			v.value = new std::string(x1 + x2);
			break;
		case OptionType::equal:
			v.type = __Variable::_bool;
			v.value = new bool(x1 == x2);
			break;
		case OptionType::bigger:
			v.type = __Variable::_bool;
			v.value = new bool(x1 > x2);
			break;
		case OptionType::lower:
			v.type = __Variable::_bool;
			v.value = new bool(x1 < x2);
			break;
		case OptionType::bigorequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 >= x2);
			break;
		case OptionType::loworequ:
			v.type = __Variable::_bool;
			v.value = new bool(x1 <= x2);
			break;
		case OptionType::notequal:
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
		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
		case OptionType::plus:
			v.type = __Variable::_matrix;
			v.value = new Matrix(x1 + x2);
			break;
		case OptionType::minus:
			v.type = __Variable::_matrix;
			v.value = new Matrix(x1 - x2);
			break;
		case OptionType::times:
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
		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
		case OptionType::times:
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
		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
		case OptionType::logicand:
			v.type = __Variable::_bool;
			v.value = new bool(x1 && x2);
			break;
		case OptionType::logicor:
			v.type = __Variable::_bool;
			v.value = new bool(x1 || x2);
			break;
		case OptionType::xors:
			v.type = __Variable::_bool;
			v.value = new bool(x1 ^ x2);
			break;
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
		if (token.type == TokenType::Error)
		{
			UI::PrintErr("Syntax error");
			return;
		}
	}
	if (tokens[0].type == TokenType::Keyword)
	{
		for (int i = 1; i < tokens.size(); ++i)
		{
			if (tokens[i].type == TokenType::Keyword)
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
			if (tokens.size() < 2 || tokens[1].type != TokenType::Variable)
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
			if (tokens[2].type != TokenType::Operator || tokens[2].value != "=" || tokens.size() < 4)
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