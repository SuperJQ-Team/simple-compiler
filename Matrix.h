#pragma once

#include <string>

#include "UI.h"

class Matrix
{
	bool is_valid;
	int row, col;
	int* buffer;
public:
	Matrix(const std::string& token);
	Matrix(int row, int col);
	Matrix(const Matrix&);
	
	static friend void UI::Print(const Matrix& matrix);
};

