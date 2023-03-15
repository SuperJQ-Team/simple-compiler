#pragma once

#include <stack>
#include <string>

#include "UI.h"
#include "Function.h"

class Executer;
class DetFunc;
class EFunc;

class Matrix
{
	bool is_valid;
	int row, col;
	double* buffer;
public:
	Matrix(const std::string& token);
	Matrix(int row, int col);
	Matrix(const Matrix&);
	~Matrix();
	
	Matrix& operator = (const Matrix& m);

	int GetRow() const { return row; }
	int GetCol() const { return col; }
	
	friend Matrix operator * (const Matrix& ma, const Matrix& mb);
	friend Matrix operator * (double k, const Matrix& m);
	friend Matrix operator + (const Matrix& ma, const Matrix& mb);
	friend Matrix operator - (const Matrix& ma, const Matrix& mb);

	static friend void UI::Print(const Matrix& matrix);
	
	friend Variable DetFunc::run(const std::stack<Variable>& args, Executer* parent);
	friend Variable EFunc::run(const std::stack<Variable>& args, Executer* parent);
};

Matrix operator * (const Matrix& ma, const Matrix& mb);
Matrix operator * (double k, const Matrix& m);
Matrix operator + (const Matrix& ma, const Matrix& mb);
Matrix operator - (const Matrix& ma, const Matrix& mb);