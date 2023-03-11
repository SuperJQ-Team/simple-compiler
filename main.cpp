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
		vector<Token> tokens = Lexer::GetTokens(s);
		UI::PrintTokens(tokens);
		ext.Execute(tokens);
		cout << "\n";
	}
	return 0;
}