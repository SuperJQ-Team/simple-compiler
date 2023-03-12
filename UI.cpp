#include "UI.h"
#include "Matrix.h"
#include "Var.h"
#include "Lexer.h"
#include "Token.h"

#include <iostream>

std::string UI::GetInputLine()
{
	printf(">>> ");
	static std::string temp;
	std::getline(std::cin, temp);
	return temp;
}

void UI::Print(int number)
{
	printf("%d", number);
}
void UI::Print(double number)
{
	printf("%lf", number);
}
void UI::Print(const Matrix& matrix)
{
	if (!matrix.is_valid)
	{
		PrintErr("This matrix is invalid");
		return;
	}
	printf("Matrix %d * %d: \n", matrix.row, matrix.col);
	for (int i = 0; i < matrix.row; ++i)
	{
		for (int j = 0; j < matrix.col; ++j)
		{
			printf("%10lf", matrix.buffer[i * matrix.col + j]);
		}
		printf("\n");
	}
	printf("\n");
}
void UI::Print(const std::string& str)
{
	printf("\"%s\"", str.c_str());
}

void UI::Print(bool boolen)
{
	if (boolen)printf("True");
	else printf("False");
}

void UI::Print(const Variable& var)
{
	switch (var.type)
	{
	case __Variable::_error:
		printf("Error");
		break;
	case __Variable::_null:
		printf("null");
		break;
	case __Variable::_int:
		printf("%d", *(int*)var.value);
		break;
	case __Variable::_float:
		printf("%lf", *(double*)var.value);
		break;
	case __Variable::_string:
		printf("\"%s\"", (*(std::string*)var.value).c_str());
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
		// if (t == TokenType::BeforeOp)return "beforpot";
		return "unknown";
	};
	printf("%s, %s\n", getType(token.type), token.value.c_str());
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
	printf("Variable %s is defined.", var_name.c_str());
}
void UI::PrintDefFunc(const std::string& func_name)
{
	printf("Function %s is defined.", func_name.c_str());
}
void UI::PrintDefErr(const std::string& def_name, const std::string& reason)
{
	printf("Error: cannot define %s. %s.", def_name.c_str(), reason.c_str());
}
void UI::PrintErr(const std::string& reason)
{
	printf("Error: %s.", reason.c_str());
}