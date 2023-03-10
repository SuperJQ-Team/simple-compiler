#pragma once

#include <string>

#include "Matrix.h"


class UI
{
public:
	static std::string GetInputLine();
	static void Print(int number);
	static void Print(double number);
	static void Print(const Matrix& matrix);
};

