#define _CRT_SECURE_NO_WARNINGS

#include"Interface.h"
#include <iostream>
#include <sstream>
#include "Lexer.h"
#include "Matrix.h"
#include "Executer.h"
#include "UI.h"
#include <string.h>

using namespace std;

Executer ext;
stringstream ss;
char c[256];

const char* __stdcall LineInterpreter(const char* input)
{
	ss.clear();
	UI::SetOS(ss);
	vector<Token> tokens = Lexer::GetTokens(input);
	ext.Execute(tokens);
	strcpy(c, ss.str().c_str());
	return c;
}