#pragma once

#include <string>

class Matrix
{
	bool is_valid;
	int row, col;
	int* buffer;
public:
	Matrix(const std::string& token);
	Matrix(int row, int col);
	Matrix(const Matrix&);
	void print(std::ostream& os);
};

