#pragma once

#include <vector>

#include "ASTNode.h"
#include "Token.h"

// Parser类
class Parser
{
public:
	Parser(const std::vector<Token>&);

	// 解析所有的声明
	std::vector<DeclNode*> parseDecls();

	ASTNode* parse();

private:
	std::vector<Token> tokens;
	int current;

	// 检查是否到达输入末尾
	bool isEnd();

	// 获取当前位置的token
	Token peek();

	// 获取下一个位置的token
	Token next();

	// 检查当前位置的token是否匹配给定类型
	bool match(TokenType type);

	// 检查当前位置的token是否匹配给定类型，如果不匹配则抛出异常
	void expect(TokenType type);

	// 解析一个声明
	DeclNode* parseDecl();

	// 解析一个变量声明
	VarDeclNode* parseVarDecl();


	// 解析一个函数声明
	FuncDeclNode* parseFuncDecl();

	// 解析一个语句
	StmtNode* parseStmt();

	// 解析一个表达式语句
	ExprStmtNode* parseExprStmt();


	// 解析一个if语句
	IfStmtNode* parseIfStmt();


	// 解析一个for语句
	ForStmtNode* parseForStmt();

	// 解析一个表达式
	ExprNode* parseExpr();

	// 解析一个项
	ExprNode* parseTerm();
};