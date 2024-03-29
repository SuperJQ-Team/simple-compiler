#pragma once
#include "Lexer.h"
#include "UI.h"

namespace __Executer {
	class Executer;
}


enum class __Variable
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


class Variable
{
public:

	void* value = nullptr;
	__Variable type = __Variable::_null;
	inline ~Variable() { del(); }

	Variable();
	Variable(__Variable type);
	Variable(__Variable type, void* ptr);
	Variable(const Token& token);
	Variable(const Variable& var);

	Variable& operator= (const Variable& var);

	void set(const Token& token);
	void del();

	static Variable err;
	static Variable nul;
	static Variable deflog(const std::string&);

	static bool iftrue(const Variable& var);

	static friend void UI::Print(const Variable& var);

};