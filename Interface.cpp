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

char c[256];
Executer ext;

const char* __stdcall LineInterpreter(const char* input)
{
	stringstream ss;
	UI::SetOS(ss);
	vector<Token> tokens = Lexer::GetTokens(input);
	ext.Execute(tokens);
	if (ss.str().size() < 256)
		strcpy(c, ss.str().c_str());
	else
	{
		int i = 0;
		for (; i < 251; ++i)c[i] = ss.str()[i];
		c[i] = '.'; c[i + 1] = '.'; c[i + 2] = '.';
		c[i + 3] = '\0';
	}
	UI::SetOS(cout);
	return c;
}