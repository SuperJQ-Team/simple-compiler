#include "Executer.h"

#include <stack>
#include "Logger.h"
#include "UI.h"
#include "Matrix.h"
#include "Function.h"

Matrix quickpow(Matrix ma, int n)
{
	std::stack<Variable> parmv;
	parmv.push(Variable(__Variable::_int, new int(ma.GetCol())));
	Matrix ans = *(Matrix*)EFunc().run(parmv).value;
	while (n)
	{
		if (n & 1)ans = ans * ma;
		ma = ma * ma;
		n >>= 1;
	}
	return ans;
}

double quickpow(double ma, int n)
{
	double ans = 1;
	while (n)
	{
		if (n & 1)ans = ans * ma;
		ma = ma * ma;
		n >>= 1;
	}
	return ans;
}

std::string circstring(const std::string& s, int x)
{
	if (x <= 0)return "";
	std::string ans;
	do
		ans += s;
	while (--x);
	return ans;
}

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

std::string volsig[] = { "=" , "+=" , "-=" , "*=" , "/=" , "%=" , "&=" ,"|=", "^=" , "~=" ,"<<=" , ">>=" };

inline bool isvol(const std::string opt)
{
	for (auto x : volsig)
		if (opt == x) return true;
	return false;
}

Variable Executer::Calculate(const std::vector<Token>& tokens, int index)
{
	std::stack<Token> opts;
	std::stack<Variable> vars;
	for (int i = index; i < tokens.size(); ++i)
	{
		if (tokens[i].type == TokenType::End || tokens[i].type == TokenType::Semicolon)
		{
			break;
		}
		if (tokens[i].type == TokenType::Function)
		{
			if (GetFunction(tokens[i].value) == nullptr)
			{
				UI::PrintErr(tokens[i].value + " is undefine function!");
				return Variable::err;
			}
			//printf("deal with function %s\n", tokens[i].value.c_str());
			opts.push(tokens[i]);
			++i;
		}
		else if (tokens[i].type == TokenType::RightParen)
		{
			//printf("deal with right paren \n");
			if (i < 2)UI::PrintErr("Bad right parenthesis.");
			std::stack<Variable> pam_stack;
			if (tokens[i - 2].type == TokenType::Function)
			{
				vars.push(GetFunction(opts.top().value)->run({}, this));
				opts.pop();
			}
			else
			{
				while (!opts.empty() && opts.top().type != TokenType::LeftParen && opts.top().type != TokenType::Function)
				{
					if (opts.top().type == TokenType::Comma)
					{
						auto& x = GetValue(vars.top());
						if (x.type == __Variable::_error)return Variable::err;
						if (vars.top().type == __Variable::_varible)pam_stack.push(x);
						else pam_stack.push(vars.top());
						vars.pop();
						opts.pop();
					}
					else
					{
						if (vars.size() < 2)return Variable::err;//ERROR;
						Variable v1, v2;
						v2 = vars.top(); vars.pop();
						v1 = vars.top(); vars.pop();
						vars.push(RunOption(v1, v2, opts.top().value));
						opts.pop();
					}
				}
				if (opts.top().type == TokenType::Function)
				{
					auto& x = GetValue(vars.top());
					if (x.type == __Variable::_error)return Variable::err;
					if (vars.top().type == __Variable::_varible)pam_stack.push(x);
					else pam_stack.push(vars.top());
					vars.pop();
					vars.push(GetFunction(opts.top().value)->run(pam_stack, this));
				}
				opts.pop();
			}
		}
		else if (tokens[i].type == TokenType::Operator || tokens[i].type == TokenType::LeftParen)
		{
			//printf("deal with operator %s\n", tokens[i].value.c_str());
			while (!opts.empty() && opts.top().type != TokenType::LeftParen &&
				(GetPriority(opts.top().value) >= GetPriority(tokens[i].value) || opts.top().type == TokenType::BeforeOp))
			{
				if (opts.top().type != TokenType::BeforeOp)
				{
					if (vars.size() < 2)return Variable::err;//ERROR;
					Variable v1, v2;
					v2 = vars.top(); vars.pop();
					v1 = vars.top(); vars.pop();
					vars.push(RunOption(v1, v2, opts.top().value));
					opts.pop();
				}
				else
				{
					if (vars.size() < 1)return Variable::err;//ERROR;
					Variable v1;
					v1 = vars.top(); vars.pop();
					vars.push(RunOption(v1, Variable::nul, opts.top().value));
					opts.pop();
				}
			}
			opts.push(tokens[i]);
		}
		else if (tokens[i].type == TokenType::BeforeOp)
		{
			opts.push(tokens[i]);
		}
		else if (tokens[i].type == TokenType::Comma)
		{
			//printf("deal with Comma \n");
			while (!opts.empty() && opts.top().type != TokenType::Comma && opts.top().type != TokenType::Function)
			{
				if (opts.top().type != TokenType::BeforeOp)
				{
					if (vars.size() < 2)return Variable::err;//ERROR;
					Variable v1, v2;
					v2 = vars.top(); vars.pop();
					v1 = vars.top(); vars.pop();
					vars.push(RunOption(v1, v2, opts.top().value));
					opts.pop();
				}
				else
				{
					if (vars.size() < 1)return Variable::err;//ERROR;
					Variable v1;
					v1 = vars.top(); vars.pop();
					vars.push(RunOption(v1, Variable::nul, opts.top().value));
					opts.pop();
				}
			}
			opts.push(tokens[i]);
		}
		else
		{
			//printf("deal with var %s\n", tokens[i].value.c_str());
			vars.push(Variable(tokens[i]));
		}
	}
	while (!opts.empty())
	{
		if (opts.top().type != TokenType::BeforeOp)
		{
			if (vars.size() < 2)return Variable::err;//ERROR;
			Variable v1, v2;
			v2 = vars.top(); vars.pop();
			v1 = vars.top(); vars.pop();
			vars.push(RunOption(v1, v2, opts.top().value));
			opts.pop();
		}
		else
		{
			if (vars.size() < 1)return Variable::err;//ERROR;
			Variable v1;
			v1 = vars.top(); vars.pop();
			vars.push(RunOption(v1, Variable::nul, opts.top().value));
			opts.pop();
		}
	}
	if (vars.size() != 1) return Variable::err;
	if (vars.top().type == __Variable::_varible)return GetValue(vars.top());
	return vars.top();
}

bool Executer::isOccured(const std::string& name)
{
	if (sign_map.find(name) != sign_map.end()) return true;
	return false;
}

Executer::Executer()
{
	Function::RegisterGlobalFunc(*this);
}


Executer::~Executer()
{
	for (auto& x : func_map)
		delete x.second;
}

Variable Executer::RunOption(const Variable& _v1, const Variable& _v2, const std::string& opt)
{
	if (_v2.type == __Variable::_null)
	{
		Variable v, v1 = _v1;
		OptionType opty = __Lexer::GetOptType(opt);
		if (opty == OptionType::error_option)
		{
			v.type = __Variable::_error;
			return v;
		}
		if (v1.type == __Variable::_varible) v1 = GetValue(*(std::string*)(v1.value));
		if (v1.type == __Variable::_int)
		{
			int& x = *(int*)v1.value;
			switch (opty)
			{
			case OptionType::plus:
				v.type = __Variable::_int;
				v.value = new int(+x);
				break;
			case OptionType::minus:
				v.type = __Variable::_int;
				v.value = new int(-x);
				break;
			case OptionType::bitnot:
				v.type = __Variable::_int;
				v.value = new int(~x);
				break;
			default:
				v.type = __Variable::_error;
				return v;
			}
		}
		else if (v1.type == __Variable::_float)
		{
			double& x = *(double*)v1.value;
			switch (opty)
			{
			case OptionType::plus:
				v.type = __Variable::_float;
				v.value = new double(+x);
				break;
			case OptionType::minus:
				v.type = __Variable::_float;
				v.value = new double(-x);
				break;
			default:
				v.type = __Variable::_error;
				return v;
			}
		}
		else if (v1.type == __Variable::_bool)
		{
			bool& x = *(bool*)v1.value;
			switch (opty)
			{
			case OptionType::logicnot:
				v.type = __Variable::_bool;
				v.value = new bool(!x);
				break;
			default:
				v.type = __Variable::_error;
				return v;
			}
		}
		else if (v1.type == __Variable::_matrix)
		{
			Matrix& x = *(Matrix*)v1.value;
			switch (opty)
			{
			case OptionType::plus:
				v.type = __Variable::_matrix;
				v.value = new Matrix(x);
				break;
			case OptionType::minus:
				v.type = __Variable::_matrix;
				v.value = new Matrix(-x);
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
	Variable v1 = _v1, v2 = _v2;
	if (isvol(opt))
	{
		if (v1.type != __Variable::_varible) return Variable::err;
		if (v2.type == __Variable::_varible) v2 = GetValue(*(std::string*)(v2.value));
		if (opt == "=")GetValue(*(std::string*)(v1.value)) = v2;
		else
		{
			OptionType opty = __Lexer::GetOptType(opt);
			Variable& v = GetValue(*(std::string*)(v1.value));
			if (v.type == __Variable::_int)
			{
				if (v2.type == __Variable::_int)
				{
					int& x = *(int*)v2.value;
					switch (opty)
					{
					case OptionType::error_option:
						v.type = __Variable::_error;
						return _v1;
					case OptionType::plusis:
						v.type = __Variable::_int;
						*(int*)v.value += x;
						break;
					case OptionType::minusis:
						v.type = __Variable::_int;
						*(int*)v.value -= x;
						break;
					case OptionType::timesis:
						v.type = __Variable::_int;
						*(int*)v.value *= x;
						break;
					case OptionType::divisionis:
						v.type = __Variable::_int;
						if (x == 0)v.type = __Variable::_error;
						else *(int*)v.value /= x;
						break;
					case OptionType::modulois:
						v.type = __Variable::_int;
						*(int*)v.value %= x;
						break;
					case OptionType::andis:
						v.type = __Variable::_int;
						*(int*)v.value &= x;
						break;
					case OptionType::oris:
						v.type = __Variable::_int;
						*(int*)v.value |= x;
						break;
					case OptionType::xoris:
						v.type = __Variable::_int;
						*(int*)v.value ^= x;
						break;
					case OptionType::right_moveis:
						v.type = __Variable::_int;
						*(int*)v.value >>= x;
						break;
					case OptionType::left_moveis:
						v.type = __Variable::_int;
						*(int*)v.value <<= x;
						break;
					default:
						v.type = __Variable::_error;
						return _v1;
					}
				}
				else if (v2.type == __Variable::_float)
				{
					double& x = *(double*)v2.value;
					switch (opty)
					{
					case OptionType::error_option:
						v.type = __Variable::_error;
						return _v1;
					case OptionType::plusis:
						v.type = __Variable::_int;
						*(int*)v.value += x;
						break;
					case OptionType::minusis:
						v.type = __Variable::_int;
						*(int*)v.value -= x;
						break;
					case OptionType::timesis:
						v.type = __Variable::_int;
						*(int*)v.value *= x;
						break;
					case OptionType::divisionis:
						v.type = __Variable::_int;
						*(int*)v.value /= x;
						break;
					default:
						v.type = __Variable::_error;
						return _v1;
					}
				}
				else
				{
					v.type = __Variable::_error;
					return _v1;
				}
			}
			else if (v.type == __Variable::_float)
			{
				if (v2.type == __Variable::_int || v2.type == __Variable::_float)
				{
					double x = v2.type == __Variable::_int ? *(int*)v2.value : *(double*)v2.value;
					switch (opty)
					{
					case OptionType::error_option:
						v.type = __Variable::_error;
						return _v1;
					case OptionType::plusis:
						v.type = __Variable::_float;
						*(double*)v.value += x;
						break;
					case OptionType::minusis:
						v.type = __Variable::_float;
						*(double*)v.value -= x;
						break;
					case OptionType::timesis:
						v.type = __Variable::_float;
						*(double*)v.value *= x;
						break;
					case OptionType::divisionis:
						v.type = __Variable::_float;
						if (x == 0)v.type = __Variable::_error;
						else *(double*)v.value /= x;
						break;
					default:
						v.type = __Variable::_error;
						return _v1;
					}
				}
				else
				{
					v.type = __Variable::_error;
					return _v1;
				}
			}
			else if (v.type == __Variable::_string)
			{
				if (v2.type == __Variable::_string)
				{
					std::string& x = *(std::string*)v2.value;
					switch (opty)
					{
					case OptionType::error_option:
						v.type = __Variable::_error;
						return _v1;
					case OptionType::plusis:
						v.type = __Variable::_string;
						*(std::string*)v.value += x;
						break;
					default:
						v.type = __Variable::_error;
						return _v1;
					}
				}
				else if (v2.type == __Variable::_int)
				{
					int& x = *(int*)v2.value;
					switch (opty)
					{
					case OptionType::error_option:
						v.type = __Variable::_error;
						return _v1;
					case OptionType::timesis:
						v.type = __Variable::_string;
						*(std::string*)v.value = circstring(*(std::string*)v.value, x);
						break;
					default:
						v.type = __Variable::_error;
						return _v1;
					}
				}
				else
				{
					v.type = __Variable::_error;
					return _v1;
				}
			}
			else if (v.type == __Variable::_bool)
			{
				if (v2.type == __Variable::_int)
				{
					int& x = *(int*)v2.value;
					switch (opty)
					{
					case OptionType::error_option:
						v.type = __Variable::_error;
						return _v1;
					case OptionType::andis:
						v.type = __Variable::_bool;
						*(bool*)v.value &= x ? true : false;
						break;
					case OptionType::oris:
						v.type = __Variable::_bool;
						*(bool*)v.value |= x ? true : false;
						break;
					case OptionType::xoris:
						v.type = __Variable::_bool;
						*(bool*)v.value ^= x ? true : false;
						break;
					default:
						v.type = __Variable::_error;
						return _v1;
					}
				}
				if (v2.type == __Variable::_bool)
				{
					bool& x = *(bool*)v2.value;
					switch (opty)
					{
					case OptionType::error_option:
						v.type = __Variable::_error;
						return _v1;
					case OptionType::andis:
						v.type = __Variable::_bool;
						*(bool*)v.value &= x;
						break;
					case OptionType::oris:
						v.type = __Variable::_bool;
						*(bool*)v.value |= x;
						break;
					case OptionType::xoris:
						v.type = __Variable::_bool;
						*(bool*)v.value ^= x;
						break;
					default:
						v.type = __Variable::_error;
						return _v1;
					}
				}
				else
				{
					v.type = __Variable::_error;
					return _v1;
				}
			}
			else if (v.type == __Variable::_matrix)
			{
				if (v2.type == __Variable::_int || v2.type == __Variable::_float)
				{
					double x = (v2.type == __Variable::_int) ? *(int*)v2.value : *(double*)v2.value;
					switch (opty)
					{
					case OptionType::error_option:
						v.type = __Variable::_error;
						return _v1;
					case OptionType::timesis:
						v.type = __Variable::_matrix;
						*(Matrix*)v.value = x * *(Matrix*)v.value;
						break;
					case OptionType::divisionis:
						v.type = __Variable::_matrix;
						*(Matrix*)v.value = (1 / x) * *(Matrix*)v.value;
						break;
					default:
						v.type = __Variable::_error;
						return _v1;
					}
				}
				else if (v2.type == __Variable::_matrix)
				{
					Matrix& x = *(Matrix*)v2.value;
					switch (opty)
					{
					case OptionType::error_option:
						v.type = __Variable::_error;
						return _v1;
					case OptionType::timesis:
						v.type = __Variable::_int;
						*(Matrix*)v.value = *(Matrix*)v.value * x;
						break;
					case OptionType::plus:
						v.type = __Variable::_int;
						*(Matrix*)v.value = *(Matrix*)v.value + x;
						break;
					default:
						v.type = __Variable::_error;
						return _v1;
					}
				}
				else
				{
					v.type = __Variable::_error;
					return _v1;
				}
			}
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
			if (x2 == 0)v.type = __Variable::_error;
			else v.value = new int(x1 / x2);
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
			v.value = new int(quickpow(x1, x2));
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
	else if (v1.type == __Variable::_int && v2.type == __Variable::_string)
	{
		int& x1 = *(int*)v1.value;
		std::string& x2 = *(std::string*)v2.value;
		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
		case OptionType::times:
			v.type = __Variable::_string;
			v.value = new std::string(circstring(x2, x1));
			break;
		default:
			v.type = __Variable::_error;
			return v;
		}
	}
	else if (v1.type == __Variable::_string && v2.type == __Variable::_int)
	{
		int& x1 = *(int*)v2.value;
		std::string& x2 = *(std::string*)v1.value;
		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
		case OptionType::times:
			v.type = __Variable::_string;
			v.value = new std::string(circstring(x2, x1));
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
	else if (v1.type == __Variable::_matrix && v2.type == __Variable::_int)
	{
		int x2 = *(int*)v2.value;
		Matrix& x1 = *(Matrix*)v1.value;
		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
		case OptionType::times:
			v.type = __Variable::_matrix;
			v.value = new Matrix(x2 * x1);
			break;
		case OptionType::division:
			v.type = __Variable::_matrix;
			v.value = new Matrix((1.0 / x2) * x1);
			break;
		case OptionType::power:
			v.type = __Variable::_matrix;
			v.value = new Matrix(quickpow(x1, x2));
			break;
		default:
			v.type = __Variable::_error;
			return v;
		}
	}
	else if (v1.type == __Variable::_matrix && v2.type == __Variable::_float)
	{
		double x2 = *(double*)v2.value;
		Matrix& x1 = *(Matrix*)v1.value;
		OptionType opty = __Lexer::GetOptType(opt);
		switch (opty)
		{
		case OptionType::error_option:
			v.type = __Variable::_error;
			return v;
		case OptionType::times:
			v.type = __Variable::_matrix;
			v.value = new Matrix(x2 * x1);
			break;
		case OptionType::division:
			v.type = __Variable::_matrix;
			v.value = new Matrix((1.0 / x2) * x1);
			break;
		case OptionType::power:
			v.type = __Variable::_matrix;
			v.value = new Matrix(quickpow(x1, x2));
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
	else if (v1.type == __Variable::_null && v2.type == __Variable::_null)
	{
		v = Variable::nul;
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

Variable& Executer::GetValue(const Variable& v)
{
	if (v.type != __Variable::_varible)return Variable::err;
	return GetValue(*(std::string*)v.value);
}

Function* Executer::GetFunction(const std::string& name)
{
	if (func_map.find(name) != func_map.end())
		return func_map[name];
	else if (father != nullptr)return father->GetFunction(name);
	else return nullptr;
}

Variable Executer::Execute(const std::vector<Token>& tokens)
{
	if (tokens.size() <= 0) return Variable::nul;
	if (funcDefVar.defining)
	{
		if (tokens[0].type == TokenType::Keyword)
		{
			if (tokens[0].value == "end" && funcDefVar.endnum == 0)
			{
				UI::infunc = funcDefVar.defining = false;
				if (funcDefVar.type == FuncDefVar::_func && father == nullptr && !UI::fileoutfig)
					UI::PrintDefFunc(funcDefVar.name);
				else if (funcDefVar.type == FuncDefVar::_if)
				{
					Variable v;
					if (Variable::iftrue(funcDefVar._ifparm))v = func_map["@if"]->run({}, this);
					delete func_map["@if"];
					func_map.erase("@if");
					var_map.erase("@if");
					if (ifreturn)
					{
						ifreturn = false;
						return v;
					}
				}
				else if (funcDefVar.type == FuncDefVar::_else)
				{
					Variable v;
					if (Variable::iftrue(funcDefVar._ifparm))v = func_map["@if"]->run({}, this);
					else v = func_map["@else"]->run({}, this);
					delete func_map["@if"]; delete func_map["@else"];
					func_map.erase("@if"); func_map.erase("@else");
					var_map.erase("@if"); var_map.erase("@else");
					if (ifreturn)
					{
						ifreturn = false;
						if (father != nullptr)
							father->ifreturn = true;
						return v;
					}
				}
				else if (funcDefVar.type == FuncDefVar::_for)
				{
					Variable v;
					v = func_map["@for"]->run({}, this);
					delete func_map["@for"];
					func_map.erase("@for");
					var_map.erase("@for");
					if (ifreturn)
					{
						ifreturn = false;
						if (father != nullptr)
							father->ifreturn = true;
						return v;
					}
				}
			}
			else if (tokens[0].value == "end")
			{
				funcDefVar.endnum--;
				func_map[funcDefVar.name]->pushInstruction(tokens);
			}
			else if (tokens[0].value == "else" && funcDefVar.endnum == 0)
			{
				if (funcDefVar.type == FuncDefVar::_if)
				{
					std::string name = "@else";
					std::vector<Token> ts;
					for (int i = 1; i < tokens.size(); ++i)
						ts.emplace_back(tokens[i]);

					Function* func = new Function(name, {});
					if (!ts.empty())
						func->pushInstruction(ts);

					funcDefVar.set(name, FuncDefVar::_else);
					func_map[name] = func;
					sign_map[name] = _func;
				}
			}
			else if (tokens[0].value == "def" || tokens[0].value == "if" || tokens[0].value == "for")
			{
				funcDefVar.endnum++;
				func_map[funcDefVar.name]->pushInstruction(tokens);
			}
			else
				func_map[funcDefVar.name]->pushInstruction(tokens);
		}
		else
			func_map[funcDefVar.name]->pushInstruction(tokens);
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
			if (tokens[i].type == TokenType::Keyword && tokens[i - 1].type != TokenType::LeftParen)
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
			if (tokens.size() < 3 || (tokens.size() == 3 && tokens[2].type == TokenType::Semicolon))
			{
				sign_map[name] = _var;
				var_map[name] = Variable::nul;
				if (father == nullptr && !UI::fileoutfig)
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
			if (father == nullptr && !UI::fileoutfig)
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
				else if (tokens[i].type == TokenType::Comma) continue;
				else if (tokens[i].type == TokenType::Variable)
					argvs.emplace_back(tokens[i].value);
				else
				{
					UI::PrintDefErr(name, "Illegal formal parameter found in definition");
					return Variable::err;
				}
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
			Function* func = new Function(name, argvs);
			if (!ts.empty())
			{
				func->pushInstruction(ts);
			}
			funcDefVar.set(name, FuncDefVar::_func);
			func_map[name] = func;
			sign_map[name] = _func;
			UI::infunc = true;
			return Variable::nul;
		}
		else if (tokens[0].value == "return")
		{
			if (ifreturn)
			{
				ifreturn = false;
			}
			if (father != nullptr)
				father->ifreturn = true;
			return Calculate(tokens, 1);
		}
		else if (tokens[0].value == "if")
		{
			if (tokens[1].type != TokenType::LeftParen)
				return Variable::err;
			//处理if
			int i = 2;
			std::vector<Token>bool_tokens;
			std::string name = "@if";
			for (; i < tokens.size(); ++i)
			{
				if (tokens[i].type == TokenType::RightParen) break;
				bool_tokens.emplace_back(tokens[i]);
			}
			if (i != tokens.size() - 1 || tokens[i].type != TokenType::RightParen)
			{
				UI::PrintErr("The \'IF\' Cannot find \')\'");
				return Variable::err;
			}
			++i;
			std::vector<Token> ts;
			for (; i < tokens.size(); ++i)
				ts.emplace_back(tokens[i]);

			Function* func = new Function(name, {});
			if (!ts.empty())
				func->pushInstruction(ts);

			funcDefVar.set(name, FuncDefVar::_if);
			funcDefVar._ifparm = Calculate(bool_tokens, 0);
			func_map[name] = func;
			sign_map[name] = _func;
			if (father == nullptr)
				UI::infunc = true;
			return Variable::nul;
		}
		else if (tokens[0].value == "for")
		{
			if (tokens[1].type != TokenType::LeftParen)
				return Variable::err;
			//处理if
			int i = 2;
			std::string name = "@for";
			int parmtokensnum = 0;
			std::vector<std::vector<Token>> parmtokens(3);
			for (; i < tokens.size(); ++i)
			{
				if (tokens[i].type == TokenType::RightParen) break;
				if (tokens[i].type == TokenType::Semicolon)
				{
					parmtokensnum++;
					if (parmtokensnum >= 3)
					{
						UI::PrintErr("More parms for \'for\' Keyword");
						return Variable::err;
					}
				}
				else
				{
					parmtokens[parmtokensnum].emplace_back(tokens[i]);
				}
			}
			if (parmtokensnum != 2)
			{
				UI::PrintErr("Less parms for \'for\' Keyword");
				return Variable::err;
			}

			if (i != tokens.size() - 1 || tokens[i].type != TokenType::RightParen)
			{

				UI::PrintErr("The \'for\' Cannot find \')\'");
				return Variable::err;
			}
			++i;
			std::vector<Token> ts;
			for (; i < tokens.size(); ++i)
				ts.emplace_back(tokens[i]);

			Function* func = new ForFunc(parmtokens[0], parmtokens[1], parmtokens[2]);
			if (!ts.empty())
				func->pushInstruction(ts);

			funcDefVar.set(name, FuncDefVar::_for);
			func_map[name] = func;
			sign_map[name] = _func;
			if (father == nullptr)
				UI::infunc = true;
			return Variable::nul;
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
	Variable out;
	if (var.type == __Variable::_varible)out = GetValue(var);
	else out = var;
	if (out.type != __Variable::_null && father == nullptr && !UI::fileoutfig)
	{
		UI::Print(out);
	}
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

void Executer::FuncDefVar::set(const std::string& n, FuncDefType t)
{
	name = n;
	type = t;
	endnum = 0;
	defining = true;
}

void Executer::FuncDefVar::del()
{
	defining = false;
}
