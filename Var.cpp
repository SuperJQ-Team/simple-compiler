#include "Var.h"
#include "Matrix.h"
#include "Executer.h"
#include "Function.h"


Variable Variable::err(__Variable::_error);
Variable Variable::nul(__Variable::_null);

Variable::Variable()
{
	type = __Variable::_null;
	value = nullptr;
}

Variable::Variable(__Variable type) :type(type)
{
	if (type == __Variable::_null || type == __Variable::_error)
		value = nullptr;
}

Variable::Variable(__Variable type, void* ptr) :type(type), value(ptr)
{
	if (type == __Variable::_null || type == __Variable::_error)
		value = nullptr;
}

Variable::Variable(const Token& token)
{
	set(token);
}

Variable::Variable(const Variable& var)
{
	if (value != nullptr)delete value;
	type = var.type;
	if (type == __Variable::_null)
		value = nullptr;
	else if (type == __Variable::_int)
		value = new int(*(int*)(var.value));
	else if (type == __Variable::_float)
		value = new double(*(double*)(var.value));
	else if (type == __Variable::_string || type == __Variable::_definelog || type == __Variable::_varible)
		value = new std::string(*(std::string*)(var.value));
	else if (type == __Variable::_matrix)
		value = new Matrix(*(Matrix*)(var.value));
	else if (type == __Variable::_bool)
		value = new bool(*(bool*)(var.value));
	else if (type == __Variable::_error)
		value = nullptr;
}

Variable& Variable::operator=(const Variable& var)
{
	if (this == &var) return *this;
	del();
	type = var.type;
	if (type == __Variable::_null)
		value = nullptr;
	else if (type == __Variable::_int)
		value = new int(*(int*)(var.value));
	else if (type == __Variable::_float)
		value = new double(*(double*)(var.value));
	else if (type == __Variable::_string || type == __Variable::_definelog || type == __Variable::_varible)
		value = new std::string(*(std::string*)(var.value));
	else if (type == __Variable::_matrix)
		value = new Matrix(*(Matrix*)(var.value));
	else if (type == __Variable::_bool)
		value = new bool(*(bool*)(var.value));
	else if (type == __Variable::_error)
		value = nullptr;
	return *this;
}

void Variable::set(const Token& token)
{
	if (token.type == TokenType::Number)
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
		if (!ifd && val < 2147483647)
		{
			type = __Variable::_int;
			value = new int((int)val);
		}
		else
		{
			type = __Variable::_float;
			value = new double(val / ws);
		}
	}
	else if (token.type == TokenType::String)
	{
		type = __Variable::_string;
		value = new std::string(token.value);
	}
	else if (token.type == TokenType::Matrix)
	{
		type = __Variable::_matrix;
		value = new Matrix(token.value);
	}
	else if (token.type == TokenType::Variable)
	{
		type = __Variable::_varible;
		value = new std::string(token.value);
	}
	else
	{
		type = __Variable::_null;
		value = nullptr;
	}
}

void Variable::del()
{
	if (value == nullptr)return;
	if (type == __Variable::_string || type == __Variable::_definelog || type == __Variable::_varible)
		delete (std::string*)(value);
	else if (type == __Variable::_matrix)
		delete(Matrix*)(value);
	else delete value;
}

Variable Variable::deflog(const std::string& s)
{
	auto v = Variable();
	v.type = __Variable::_definelog;
	v.value = new std::string(s);
	return v;
}

bool Variable::iftrue(const Variable& var)
{
	if (var.type == __Variable::_bool)
		return *(bool*)var.value;
	else if (var.type == __Variable::_int)
		return (bool)*(int*)var.value;
	else return false;
}
