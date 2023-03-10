#pragma once

#include <string>

class Matrix;


class UI
{
public:
	static std::string GetInputLine();
	static void Print(int number);
	static void Print(double number);
	static void Print(const Matrix& matrix);
};

