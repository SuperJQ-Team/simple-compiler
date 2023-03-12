//#define TODLL

#ifndef TODLL

#include <iostream>
#include "Lexer.h"
#include "Matrix.h"
#include "Executer.h"
#include "UI.h"

using namespace std;

int main(int argc, char* argv[])
{
	string s;
	Executer ext;
	//Executer ext;
	while (1)
	{
		s = UI::GetInputLine();
		if (s[0] == EOF)break;
		vector<Token> tokens = Lexer::GetTokens(s);
		UI::PrintTokens(tokens);
		ext.Execute(tokens);
		cout << "\n";
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