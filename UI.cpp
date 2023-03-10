#include "UI.h"
#include "Matrix.h"
#include "Var.h"

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
	printf("Matrix %d * %d: \n", matrix.row, matrix.col);
	for (int i = 0; i < matrix.row; ++i)
	{
		for (int j = 0; j < matrix.col; ++j)
		{
			printf("%10d", matrix.buffer[i * matrix.col + j]);
		}
		printf("\n");
	}
	printf("\n");
}
void UI::Print(const Variable& var)
{
	switch (var._type)
	{
	case __Variable::_error:
		printf("Error\n");
		break;
	case __Variable::_null:
		printf("null\n");
		break;
	case __Variable::_int:
		printf("%d\n", *(int*)var._val);
		break;
	case __Variable::_float:
		printf("%lf\n", *(double*)var._val);
		break;
	case __Variable::_string:
		printf("%s\n", *(std::string*)var._val);
		break;
	case __Variable::_matrix:
		Print(*(Matrix*)var._val);
		break;
	/*case __Variable::_definelog:
		os << "varible " << *(std::string*)_val << " has been creat";
		break;
	case __Variable::_varible:
		os << "varible " << *(std::string*)_val << " = "; ex.GetValue(*(std::string*)this->_val).output(os, ex);
		break;*/
	default:
		break;
	}
}