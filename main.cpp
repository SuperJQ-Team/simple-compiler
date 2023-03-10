#include <iostream>
#include "WordAnalyser.h"
#include "Parser.h"
#include "Matrix.h"
#include "Executer.h"

using namespace std;

using __Parser::Token;
using __Parser::Parser;
using __Executer::Executer;

int main()
{/*
	WordAnalyser w;
	string s;
	vector<string> v;
	getline(cin, s);
	w.split(s, v);
	for (int i = 0; i < v.size(); ++i)
	{
		cout <<" [ " << v[i] << " ] ";
	}*/
	string s;
	Executer ext;
	while (1)
	{
		getline(cin, s);
		vector<Token> tokens = Parser::getTokens(s);
		__Parser::PrintTokens(tokens);
		__Varible::Varible var = ext.execute(tokens);
		var.output(cout,ext);
		cout << "\n";
	}
	return 0;
}