#define  _CRT_SECURE_NO_WARNINGS

//#define DEBUG

#ifndef DEBUG

#ifndef TODLL

#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "Matrix.h"
#include "Parser.h"
#include "Executer.h"
#include "ASTNode.h"
#include "UI.h"
#include "Function.h"

#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	ifstream fin;
	string s;
	Executer ext;
	Function::RegisterGlobalFunc(ext);
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
			UI::fileoutfig = true;
			while (1)
			{
				s = UI::GetFileLine(fin);
				if (s[0] == EOF)break;
				vector<Token> tokens = Lexer::GetTokens(s);
				auto ans = ext.Execute(tokens);
				if (tokens.back().type == TokenType::Error || ans.type == __Variable::_error)break;
				UI::PrintLog("\n");
			}
		}
	}
	else
	{
		ext.GetFunction("welcome")->run({});
		while (1)
		{
			s = UI::GetInputLine();
			if (s[0] == EOF)break;
			vector<Token> tokens = Lexer::GetTokens(s);
			//UI::PrintTokens(tokens);
			//Parser parser(tokens);
			//ASTNode* node = parser.parse();
			//DFSAST(node, 0);
			ext.Execute(tokens);
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

#else

#include "Function.h"
#include "Var.h"
#include <string>
#include <vector>
using namespace std;

int main()
{
	WelcomeFunc welfunc;
	welfunc.run({});
	string s;
	PrintFunc print;
	while (1)
	{
		s = UI::GetInputLine();
		if (s[0] == EOF)break;
		vector<Token> tokens = Lexer::GetTokens(s);
		UI::PrintTokens(tokens);
		vector<Variable> t;
		for (auto& token : tokens)
		{
			if (token.type == TokenType::Number) t.push_back(Variable(token));
		}
		print.run(t);
		//ext.Execute(tokens);
		UI::PrintLog("\n");
	}
}

#endif