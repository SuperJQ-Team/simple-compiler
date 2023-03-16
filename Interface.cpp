#define _CRT_SECURE_NO_WARNINGS

#include"Interface.h"
#include <iostream>
#include <sstream>
#include "Lexer.h"
#include "Matrix.h"
#include "Executer.h"
#include "UI.h"
#include "Function.h"
#include <string.h>
#include <fstream>

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

const char* __stdcall FileOpen(const char* file)
{
	stringstream ss;
	UI::SetOS(ss);
	Executer ext;
	ifstream fin(file);
	std::string s;
	if (!fin)UI::PrintErr("cannot find file!\n");
	else
	{
		{
			char cs[256];
			sprintf(cs, "runing { %s }\n", file);
			UI::PrintLog(cs);
		}
		UI::fileoutfig = true;
		while (1)
		{
			s = UI::GetFileLine(fin);
			if (s[0] == EOF)break;
			vector<Token> tokens = Lexer::GetTokens(s);
			//UI::PrintTokens(tokens);
			auto ans = ext.Execute(tokens);
			if ((!tokens.empty() && tokens.back().type == TokenType::Error) || ans.type == __Variable::_error)
			{
				UI::PrintLog("\n");
				UI::PrintErr("unknowing error!");
				break;
			}
		}
	}
	if (ss.str().size() < 256)
		strcpy(c, ss.str().c_str());
	else
	{
		int i = 0;
		for (; i < 251; ++i)c[i] = ss.str()[i];
		c[i] = '.'; c[i + 1] = '.'; c[i + 2] = '.';
		c[i + 3] = '\0';
	}
	UI::fileoutfig = false;
	UI::SetOS(cout);
	return c;
}