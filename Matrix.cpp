#include <cstdio>
#include <cstring>

#include "Matrix.h"

Matrix::Matrix(int row, int col) : is_valid(true), row(row), col(col)
{
	buffer = new int[row * col];
	memset(buffer, 0, sizeof(int) * row * col);
}

Matrix::Matrix(const Matrix& m) :col(m.col), row(m.row)
{
	buffer = new int[col * row];
	for (int i = 0; i < col * row; ++i)
		buffer[i] = m.buffer[i];
}

Matrix::Matrix(const std::string& token)
{
	is_valid = true;
	int left_brack_num = 0;
	for (char t : token)
	{
		if (t == '{')
		{
			if (left_brack_num < 2) ++left_brack_num;
			else is_valid = false;
		}
		else if (t == '}')
		{
			if (left_brack_num > 0) --left_brack_num;
			else is_valid = false;
		}
	}
	if (!is_valid) return;

	int now_digit = 0;
	left_brack_num = 0;
	for (int i = 0; i < token.length(); ++i)
	{
		if (token[i] == '{')
		{
			++left_brack_num;
			if (left_brack_num == 2)
			{
				int col = 0;
				int inner_digit = 0;
				while (token[i] != '}')
				{
					if (token[i] == ',')
					{
						++col;
						inner_digit = 0;
					}
					else
					{
						inner_digit = inner_digit * 10 + token[i] - '0';
					}
					++i;
				}
				this->col = std::max(this->col, col);
				--i;
			}
		}
		else if (token[i] == '}') --left_brack_num;
		else if (token[i] == ',')
		{
			++row;
			now_digit = 0;
		}
		else now_digit = now_digit * 10 + token[i] - '0';
	}
	++row;
	++col;
	buffer = new int[row * col];
	memset(buffer, 0, sizeof(int) * row * col);

	int row_num = 0;
	now_digit = 0;
	left_brack_num = 0;
	int ptr = 0;
	for (int i = 0; i < token.size(); ++i)
	{
		if (token[i] == '{') ++left_brack_num;
		else if (token[i] == '}')
		{
			if (left_brack_num == 2)
			{
				buffer[ptr++] = now_digit;
				now_digit = 0;
				++row_num;
				for (int i = row_num; i < col; ++i) buffer[ptr++] = 0;
				row_num = 0;
				if (token[i + 1] == ',') ++i;
			}
			else
			{
				buffer[ptr++] = now_digit;
				now_digit = 0;
			}
			--left_brack_num;
		}
		else if (token[i] == ',')
		{
			buffer[ptr++] = now_digit;
			if (left_brack_num == 1)
			{
				for (int i = 1; i < col; ++i) buffer[ptr++] = 0;
			}
			else
			{
				++row_num;
			}
			now_digit = 0;
		}
		else now_digit = now_digit * 10 + token[i] - '0';
		if (left_brack_num == 0) break;
	}
}

void Matrix::print(std::ostream& os)
{
	printf("Matrix %d * %d: \n", row, col);
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