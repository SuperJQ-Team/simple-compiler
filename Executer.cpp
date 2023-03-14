#include "Executer.h"

#include <stack>
#include "Logger.h"
#include "UI.h"
#include "Matrix.h"
#include "Function.h"

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
	std::stack<Variable> vars;
	for (int i = index; i < tokens.size(); ++i)
	{

		if (tokens[i].type == TokenType::Function)
		{
			if (sign_map[tokens[i].value] != _func)
			{
				UI::PrintErr(tokens[i].value + " is undefine function!");
				return Variable::err;
			}
			printf("deal with function %s\n", tokens[i].value.c_str());
			opts.push(tokens[i]);
			++i;
		}
		else if (tokens[i].type == TokenType::RightParen)
		{
			printf("deal with right paren \n");
			std::stack<Variable> pam_stack;
			while (!opts.empty() && opts.top().type != TokenType::LeftParen && opts.top().type != TokenType::Function)
			{
				if (opts.top().type == TokenType::Comma)
				{
					pam_stack.push(vars.top());
					vars.pop();
					opts.pop();
				}
				else
				{
					if (vars.size() < 2)return Variable::err;//ERROR;
					Variable v1, v2;
					v2 = vars.top(); vars.pop();
					v1 = vars.top(); vars.pop();
					vars.push(RunOption(v1, v2, tokens[i].value));
					opts.pop();
				}
			}
			if (opts.top().type == TokenType::Function)
			{
				pam_stack.push(vars.top());
				vars.pop();
				vars.push(func_map[opts.top().value]->run(pam_stack, this));
			}
			opts.pop();
		}
		else if (tokens[i].type == TokenType::Operator || tokens[i].type == TokenType::LeftParen)
		{
			printf("deal with operator %s\n", tokens[i].value.c_str());
			while (!opts.empty() && opts.top().type != TokenType::LeftParen && GetPriority(opts.top().value) >= GetPriority(tokens[i].value))
			{
				if (vars.size() < 2)return Variable::err;//ERROR;
				Variable v1, v2;
				v2 = vars.top(); vars.pop();
				v1 = vars.top(); vars.pop();
				vars.push(RunOption(v1, v2, tokens[i].value));
				opts.pop();
			}
			opts.push(tokens[i]);
		}
		else if (tokens[i].type == TokenType::Comma)
		{
			printf("deal with Comma \n");
			while (!opts.empty() && opts.top().type != TokenType::Comma && opts.top().type != TokenType::Function)
			{
				if (vars.size() < 2)return Variable::err;//ERROR;
				Variable v1, v2;
				v2 = vars.top(); vars.pop();
				v1 = vars.top(); vars.pop();
				vars.push(RunOption(v1, v2, tokens[i].value));
				opts.pop();
			}
			opts.push(tokens[i]);
		}
		else
		{
			printf("deal with var %s\n", tokens[i].value.c_str());
			vars.push(Variable(tokens[i]));
		}
	}
	while (!opts.empty())
	{
		if (vars.size() < 2)return Variable::err;//ERROR;
		Variable v1, v2;
		v2 = vars.top(); vars.pop();
		v1 = vars.top(); vars.pop();
		vars.push(RunOption(v1, v2, opts.top().value));
		opts.pop();
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

Variable Executer::Execute(const std::vector<Token>& tokens)
{
	if (tokens.size() <= 0) return Variable::nul;
	if (definingfunc)
	{
		if (tokens[0].value == "end")
			UI::infunc = definingfunc = false;
		else
			func_map[tempstr]->pushInstruction(tokens);
		return Variable::nul;
	}
	for (Token token : tokens)
	{
		if (token.type == TokenType::Error)
		{
			UI::PrintErr("Syntax error");
			return Variable::nul;
		}
	}
	if (tokens[0].type == TokenType::Keyword)
	{
		for (int i = 1; i < tokens.size(); ++i)
		{
			if (tokens[i].type == TokenType::Keyword)
			{
				UI::PrintErr("Format error");
				return Variable::nul;
			}
		}
		if (tokens[0].value == "end")
		{
			UI::PrintErr("\"End\" should not exist at there");
			return Variable::nul;
		}

		if (tokens[0].value == "let")
		{
			if (tokens.size() < 2 || tokens[1].type != TokenType::Variable)
			{
				UI::PrintErr("Cannot find the variable name");
				return Variable::nul;
			}
			const std::string& name = tokens[1].value;

			if (isOccured(name))
			{
				UI::PrintDefErr(tokens[1].value, "This sign is already existed");
				return Variable::nul;
			}
			if (tokens.size() < 3)
			{
				sign_map[name] = _var;
				var_map[name] = Variable::nul;
				UI::PrintDefVar(name);
				return Variable::nul;
			}
			if (tokens[2].type != TokenType::Operator || tokens[2].value != "=" || tokens.size() < 4)
			{
				UI::PrintDefErr(name, "Cannot find the initial value of the variable");
				return Variable::nul;
			}
			Variable var = Calculate(tokens, 3);
			if (var.type == __Variable::_error)
			{
				UI::PrintDefErr(name, "Initial error");
				return Variable::nul;
			}
			sign_map[name] = _var;
			var_map[name] = var;
			UI::PrintDefVar(name);
			return Variable::nul;
		}
		else if (tokens[0].value == "def")
		{
			if (tokens.size() < 2)
			{
				UI::PrintErr("Cannot find the function name");
				return Variable::err;
			}
			std::string name = tokens[1].value;
			if (isOccured(name))
			{
				UI::PrintDefErr(tokens[1].value, "This sign is already existed");
				return Variable::nul;
			}
			std::vector<std::string> argvs;
			if (tokens[2].value != "(")
			{
				UI::PrintDefErr(name, "Cannot find the list of arguments");
				return Variable::nul;
			}
			int i = 3;
			for (; i < tokens.size(); ++i)
			{
				if (tokens[i].type == TokenType::RightParen) break;
				if (tokens[i].type == TokenType::Comma) continue;
				argvs.emplace_back(tokens[i].value);
			}
			if (i != tokens.size() - 1 || tokens[i].type != TokenType::RightParen)
			{
				UI::PrintDefErr(name, "Cannot find \')\'");
				return Variable::err;
			}
			++i;
			std::vector<Token> ts;
			for (; i < tokens.size(); ++i)
			{
				ts.emplace_back(tokens[i]);
			}
			Function *func = new Function(name, argvs);
			if (!ts.empty())
			{
				func->pushInstruction(ts);
			}
			definingfunc = true;
			func_map[name] = func;
			sign_map[name] = _func;
			tempstr = name;
			UI::infunc = true;
			return Variable::nul;
		}
		else if (tokens[0].value == "return")
		{
			UI::PrintLog("----------FUcK---------\n");
			UI::PrintTokens(tokens);
			UI::Print(Calculate(tokens, 1));
			UI::PrintLog("\n-----------------------\n");
			return Calculate(tokens, 1);
		}
		else
		{
			UI::PrintErr("Undefined define");
			return Variable::err;
		}
	}

	Variable var = Calculate(tokens, 0);
	if (var.type == __Variable::_error)
	{
		UI::PrintErr("Cannot calculate");
		return Variable::err;
	}

	if (var.type == __Variable::_varible) UI::Print(var_map[*(std::string*)var.value]);
	else UI::Print(var);
	return Variable::nul;
}

void Executer::RegisterVarialbe(const std::string& name, const Variable& var)
{
	if (sign_map.find(name) != sign_map.end())
		return;
	else
	{
		sign_map[name] = _var;
		var_map[name] = var;
	}
}

void Executer::RegisterFunction(const std::string& name, Function* func)
{
	if (sign_map.find(name) != sign_map.end())
		return;
	else
	{
		sign_map[name] = _func;
		func_map[name] = func;
	}
		
}
