#pragma once

#include <vector>

#include "ASTNode.h"
#include "Token.h"

// Parser��
class Parser
{
public:
	Parser(const std::vector<Token>&);

	// �������е�����
	std::vector<DeclNode*> parseDecls();

	ASTNode* parse();

private:
	std::vector<Token> tokens;
	int current;

	// ����Ƿ񵽴�����ĩβ
	bool isEnd();

	// ��ȡ��ǰλ�õ�token
	Token peek();

	// ��ȡ��һ��λ�õ�token
	Token next();

	// ��鵱ǰλ�õ�token�Ƿ�ƥ���������
	bool match(TokenType type);

	// ��鵱ǰλ�õ�token�Ƿ�ƥ��������ͣ������ƥ�����׳��쳣
	void expect(TokenType type);

	// ����һ������
	DeclNode* parseDecl();

	// ����һ����������
	VarDeclNode* parseVarDecl();


	// ����һ����������
	FuncDeclNode* parseFuncDecl();

	// ����һ�����
	StmtNode* parseStmt();

	// ����һ�����ʽ���
	ExprStmtNode* parseExprStmt();


	// ����һ��if���
	IfStmtNode* parseIfStmt();


	// ����һ��for���
	ForStmtNode* parseForStmt();

	// ����һ�����ʽ
	ExprNode* parseExpr();

	// ����һ����
	ExprNode* parseTerm();
};