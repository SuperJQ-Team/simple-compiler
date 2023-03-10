#pragma once
#include "Parser.h"

namespace __Executer {
	class Executer;
}

namespace __Varible {
	enum Type
	{
		_error = -1,
		_null = 0,
		_int = 1,
		_float = 2,
		_string = 3,
		_matrix = 4,
		_definelog = 5,
		_varible = 6
	};

	class Varible
	{
	public:

		void* _val = nullptr;
		Type _type = _null;
		inline ~Varible() { if (_val != nullptr)delete _val; }

		Varible();
		Varible(Type type);
		Varible(const __Parser::Token& token);
		Varible(const Varible& var);

		Varible operator= (const Varible& var);

		void set(const __Parser::Token& token);
		void output(std::ostream& os, __Executer::Executer&);

		static Varible err;
		static Varible nul;
		static Varible deflog(const std::string&);
	};
}

