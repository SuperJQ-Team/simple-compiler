//#define TODLL
#define  _CRT_SECURE_NO_WARNINGS

#ifndef TODLL

#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "Matrix.h"
#include "Executer.h"
#include "UI.h"

#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{

	ifstream fin;
	string s;
	Executer ext;
	if (argc >= 2)
	{
		string file(argv[argc - 1]);
		fin.open(file);
		if (!fin)UI::PrintErr("cannot find file!\n");
		else
		{
			char cs[256];
			sprintf(cs, "runing { %s }\n", argv[argc - 1]);
			UI::PrintLog(cs);
			while (1)
			{
				s = UI::GetFileLine(fin);
				if (s[0] == EOF)break;
				vector<Token> tokens = Lexer::GetTokens(s);
				//UI::PrintTokens(tokens);
				ext.Execute(tokens);
				if (tokens.back().type == TokenType::Error)break;
				UI::PrintLog("\n");
			}
		}
	}
	else
	{
		while (1)
		{
			s = UI::GetInputLine();
			if (s[0] == EOF)break;
			vector<Token> tokens = Lexer::GetTokens(s);
			UI::PrintTokens(tokens);
			ext.Execute(tokens);
			UI::PrintLog("\n");
		}
	}
	return 0;
}

#else

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif