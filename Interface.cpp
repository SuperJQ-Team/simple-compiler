#include"Interface.h"
#include <iostream>
#include <sstream>
#include "Lexer.h"
#include "Matrix.h"
#include "Executer.h"
#include "UI.h"

using namespace std;
Executer ext;
const char* __stdcall LineInterpreter(const char* input)
{
	stringstream ss;
	UI::osp = &ss;
	vector<Token> tokens = Lexer::GetTokens(input);
	ext.Execute(tokens);
	ss << "\n";

	return ss.str().c_str();
}