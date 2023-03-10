#include "Var.h"
#include "Matrix.h"
#include "Executer.h"

using namespace __Variable;

Variable Variable::err(_error);
Variable Variable::nul(_null);

Variable::Variable()
{
	_type =  _null;
	_val = nullptr;
}

Variable::Variable(Type type) :_type(type)
{
	if (type == _null || type == _error)
		_val = nullptr;
}

Variable::Variable(const Token& token)
{
	set(token);
}

Variable::Variable(const Variable& var)
{
	_type = var._type;
	if (_type == _null)
		_val = nullptr;
	if (_type == _int)
		_val = new int(*(int*)(var._val));
	if (_type == _float)
		_val = new double(*(double*)(var._val));
	if (_type == _string || _type == _definelog || _type == _varible)
		_val = new std::string(*(std::string*)(var._val));
	if (_type == _matrix)
		_val = new Matrix(*(Matrix*)(var._val));
}

Variable& Variable::operator=(const Variable& var)
{
	if (this == &var) return *this;
	_type = var._type;
	if (_type == _null)
		_val = nullptr;
	if (_type == _int)
		_val = new int(*(int*)(var._val));
	if (_type == _float)
		_val = new double(*(double*)(var._val));
	if (_type == _string || _type == _definelog || _type == _varible)
		_val = new std::string(*(std::string*)(var._val));
	if (_type == _matrix)
		_val = new Matrix(*(Matrix*)(var._val));

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
			_type = _int;
			_val = new int((int)val);
		}
		else
		{
			_type = _float;
			_val = new double(val / ws);
		}
	}
	else if (token.type == __Parser::string)
	{
		_type = _string;
		_val = new std::string(token.value);
	}
	else if (token.type == __Parser::matrix)
	{
		_type = _matrix;
		_val = new Matrix(token.value);
	}
	else if (token.type == __Parser::variable)
	{
		_type = _varible;
		_val = new std::string(token.value);
	}
	else
	{
		_type = _null;
		_val = nullptr;
	}
}

Variable Variable::deflog(const std::string& s)
{
	auto v = Variable();
	v._type = _definelog;
	v._val = new std::string(s);
	return v;
}


