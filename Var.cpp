#include "Var.h"
#include "Matrix.h"
#include "Executer.h"

using namespace __Variable;

Variable Variable::err(_error);
Variable Variable::nul(_null);

Variable::Variable()
{
	type =  _null;
	value = nullptr;
}

Variable::Variable(Type type) :type(type)
{
	if (type == _null || type == _error)
		value = nullptr;
}

Variable::Variable(const Token& token)
{
	set(token);
}

Variable::Variable(const Variable& var)
{
	type = var.type;
	if (type == _null)
		value = nullptr;
	if (type == _int)
		value = new int(*(int*)(var.value));
	if (type == _float)
		value = new double(*(double*)(var.value));
	if (type == _string || type == _definelog || type == _varible)
		value = new std::string(*(std::string*)(var.value));
	if (type == _matrix)
		value = new Matrix(*(Matrix*)(var.value));
	if (type == _bool)
		value = new bool(*(bool*)(var.value));
}

Variable& Variable::operator=(const Variable& var)
{
	if (this == &var) return *this;
	type = var.type;
	if (type == _null)
		value = nullptr;
	if (type == _int)
		value = new int(*(int*)(var.value));
	if (type == _float)
		value = new double(*(double*)(var.value));
	if (type == _string || type == _definelog || type == _varible)
		value = new std::string(*(std::string*)(var.value));
	if (type == _matrix)
		value = new Matrix(*(Matrix*)(var.value));
	if (type == _bool)
		value = new bool(*(bool*)(var.value));

	return *this;
}

void Variable::set(const Token& token)
{
	if (token.type == __Parser::number)
	{
		double val = 0;
		bool ifd = false;
		double ws = 1;
		//TODO
		for (auto i : token.value)
		{
			if (ifd)ws *= 10;
			if (i != '.')
				val = val * 10 + i - '0' + 0;
			else
				if (!ifd)ifd = true;
				else
				{
					*this = Variable::err;
					return;
				}
		}
		if (!ifd)
		{
			type = _int;
			value = new int((int)val);
		}
		else
		{
			type = _float;
			value = new double(val / ws);
		}
	}
	else if (token.type == __Parser::string)
	{
		type = _string;
		value = new std::string(token.value);
	}
	else if (token.type == __Parser::matrix)
	{
		type = _matrix;
		value = new Matrix(token.value);
	}
	else if (token.type == __Parser::variable)
	{
		type = _varible;
		value = new std::string(token.value);
	}
	else
	{
		type = _null;
		value = nullptr;
	}
}

Variable Variable::deflog(const std::string& s)
{
	auto v = Variable();
	v.type = _definelog;
	v.value = new std::string(s);
	return v;
}


