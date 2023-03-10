#include "Var.h"
#include "Matrix.h"
#include "Executer.h"

__Varible::Varible __Varible::Varible::err(__Varible::_error);
__Varible::Varible __Varible::Varible::nul(__Varible::_null);

__Varible::Varible::Varible()
{
	_type = _null;
	_val = nullptr;
}

__Varible::Varible::Varible(Type type) :_type(type)
{
	if (type == _null || type == _error)
		_val = nullptr;
}

__Varible::Varible::Varible(const __Parser::Token& token)
{
	set(token);
}

__Varible::Varible::Varible(const Varible& var)
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

__Varible::Varible __Varible::Varible::operator=(const Varible& var)
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

	return *this;
}

void __Varible::Varible::set(const __Parser::Token& token)
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
					*this = Varible::err;
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


void __Varible::Varible::output(std::ostream& os, __Executer::Executer& ex)
{
	switch (_type)
	{
	case __Varible::_error:
		os << "Error!";
		break;
	case __Varible::_null:
		os << "null";
		break;
	case __Varible::_int:
		os << *(int*)_val;
		break;
	case __Varible::_float:
		os << *(double*)_val;
		break;
	case __Varible::_string:
		os << *(std::string*)_val;
		break;
	case __Varible::_matrix:
		(*(Matrix*)_val);
		break;
	case __Varible::_definelog:
		os << "varible " << *(std::string*)_val << " has been creat";
		break;
	case __Varible::_varible:
		os << "varible " << *(std::string*)_val << " = "; ex.GetValue(*(std::string*)this->_val).output(os, ex);
		break;
	default:
		break;
	}
}

__Varible::Varible __Varible::Varible::deflog(const std::string& s)
{
	auto v = Varible();
	v._type = __Varible::_definelog;
	v._val = new std::string(s);
	return v;
}


