#pragma once

#include <string>

#include "UI.h"

class Matrix
{
	bool is_valid;
	int row, col;
	double* buffer;
public:
	Matrix(const std::string& token);
	Matrix(int row, int col);
	Matrix(const Matrix&);
	
	Matrix& operator = (const Matrix& m);
	
	friend Matrix operator * (const Matrix& ma, const Matrix& mb);
	friend Matrix operator * (double k, const Matrix& m);
	friend Matrix operator + (const Matrix& ma, const Matrix& mb);

	static friend void UI::Print(const Matrix& matrix);
};

Matrix operator * (const Matrix& ma, const Matrix& mb);
Matrix operator * (double k, const Matrix& m);
Matrix operator + (const Matrix& ma, const Matrix& mb);
