#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>

class Matrix;
class Variable;
class Token;


class UI
{
public:
	static std::ostream* osp;
	static std::ostream& os;

	static std::string GetInputLine();

	static void Print(int number);
	static void Print(double number);
	static void Print(const Matrix& matrix);
	static void Print(const std::string& str);
	static void Print(bool boolen);

	static void Print(const Variable& var);

	static void PrintToken(const Token& token);
	static void PrintTokens(const std::vector<Token>& tokens);

	static void PrintDefVar(const std::string& var_name);
	static void PrintDefFunc(const std::string& func_name);
	static void PrintDefErr(const std::string& def_name, const std::string& reason);

	static void PrintErr(const std::string& reason);
};

