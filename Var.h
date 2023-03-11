#pragma once
#include "Lexer.h"
#include "UI.h"

namespace __Executer {
	class Executer;
}

namespace __Variable {
	enum Type
	{
		_error = -1,
		_null = 0,
		_int = 1,
		_float = 2,
		_string = 3,
		_matrix = 4,
		_definelog = 5,
		_varible = 6,
		_bool = 7
	};
}

class Variable
{
public:

	void* value = nullptr;
	__Variable::Type type = __Variable::_null;
	inline ~Variable() { if (value != nullptr)delete value; }

	Variable();
	Variable(__Variable::Type type);
	Variable(const Token& token);
	Variable(const Variable& var);

	Variable& operator= (const Variable& var);

	void set(const Token& token);

	static Variable err;
	static Variable nul;
	static Variable deflog(const std::string&);

	static friend void UI::Print(const Variable& var);
};