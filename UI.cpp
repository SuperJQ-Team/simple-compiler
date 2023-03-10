#include "UI.h"

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
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			printf("%10d", buffer[i * col + j]);
		}
		printf("\n");
	}
	printf("\n");
}