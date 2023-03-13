#include "UI.h"
#include "Matrix.h"
#include "Var.h"
#include "Lexer.h"
#include "Token.h"

#include <iostream>
#include <sstream>

std::ostream* UI::osp = &std::cout;
std::ostream& UI::os = *osp;
bool UI::fileoutfig = false;

char s[256];

void UI::SetOS(std::ostream& o)
{
	osp = &o;
}

std::string UI::GetInputLine()
{
	os << ">>> ";
	static std::string temp;
	if (std::getline(std::cin, temp))
		return temp;
	else
		return "\xFF";
}

std::string UI::GetFileLine(std::istream& os)
{
	static std::string temp;
	if (std::getline(os, temp))
		return temp;
	else
		return "\xFF";
}

void UI::Print(int number)
{
	os << number;
}
void UI::Print(double number)
{

	if (sprintf(s, "%lf", number))os << s;
}
void UI::Print(const Matrix& matrix)
{

	if (!matrix.is_valid)
	{
		PrintErr("This matrix is invalid");
		return;
	}
	if (sprintf(s, "Matrix %d * %d: \n", matrix.row, matrix.col))os << s;
	for (int i = 0; i < matrix.row; ++i)
	{
		for (int j = 0; j < matrix.col; ++j)
		{
			if (sprintf(s, "%10lf", matrix.buffer[i * matrix.col + j]))os << s;
		}
		os << "\n";
	}
	os << "\n";
}
void UI::Print(const std::string& str)
{
	if (sprintf(s, "\"%s\"", str.c_str()))os << s; \
}

void UI::Print(bool boolen)
{
	os << (boolen ? "True" : "False");
}

void UI::Print(const Variable& var)
{

	switch (var.type)
	{
	case __Variable::_error:
		os << ("Error");
		break;
	case __Variable::_null:
		os << ("null");
		break;
	case __Variable::_int:
		if (sprintf(s, "%d", *(int*)var.value))os << s;
		break;
	case __Variable::_float:
		if (sprintf(s, "%lf", *(double*)var.value))os << s;
		break;
	case __Variable::_string:
		if (sprintf(s, "\"%s\"", (*(std::string*)var.value).c_str()))os << s;
		break;
	case __Variable::_matrix:
		Print(*(Matrix*)var.value);
		break;
	case __Variable::_bool:
		Print(*(bool*)var.value);
		break;
		//case __Variable::_definelog:
		//	os << "varible " << *(std::string*)value << " has been creat";
		//	break;
		//case __Variable::_varible:
		//	os << "varible " << *(std::string*)value << " = "; ex.GetValue(*(std::string*)this->value).output(os, ex);
		//	break;
	default:
		break;
	}
}
void UI::PrintToken(const Token& token)
{
	using namespace __Lexer;
	auto getType = [](TokenType t) {
		if (t == TokenType::Error) return "error";
		if (t == TokenType::Number) return "number";
		if (t == TokenType::Operator) return "option";
		if (t == TokenType::Keyword) return "keyword";
		if (t == TokenType::Variable) return "variable";
		if (t == TokenType::String) return "string";
		if (t == TokenType::Matrix) return "matrix";
		if (t == TokenType::Function)return "function";
		if (t == TokenType::End)return "end";
		return "unknown";
	};

	if (sprintf(s, "%s, %s\n", getType(token.type), token.value.c_str()))os << s;
}
void UI::PrintTokens(const std::vector<Token>& tokens)
{
	for (Token token : tokens)
	{
		PrintToken(token);
	}
}

void UI::PrintDefVar(const std::string& var_name)
{

	if (sprintf(s, "Variable %s is defined.", var_name.c_str()))os << s;
}
void UI::PrintDefFunc(const std::string& func_name)
{

	if (sprintf(s, "Function %s is defined.", func_name.c_str()))os << s;
}
void UI::PrintDefErr(const std::string& def_name, const std::string& reason)
{

	if (sprintf(s, "Error: cannot define %s. %s.", def_name.c_str(), reason.c_str()))os << s;
}
void UI::PrintErr(const std::string& reason)
{

	if (sprintf(s, "Error: %s.", reason.c_str()))os << s;
}

void UI::PrintLog(const std::string& log)
{
	os << log;
}
