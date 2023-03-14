#pragma once
#include <vector>
#include "Token.h"

enum class NodeType
{
	Base,
	Number,
	Identifier,
	Operator,
	FuncCall,
	Variable,
	FuncDecl,
	VarDecl,
	ExprStmt,
	IfStmt,
	ForStmt,
};

// �����﷨���ڵ����
class ASTNode
{
protected:
	NodeType type;
public:
	ASTNode() :type(NodeType::Base) {}
	ASTNode(NodeType t) :type(t) {}
	NodeType GetType() const { return type; };
	virtual ~ASTNode() {}

	friend void DFSAST(ASTNode* node, int deepth);
};

// ���ʽ�ڵ����
class ExprNode : public ASTNode
{
public:
	virtual ~ExprNode() {}
};

// ���ֽڵ���
class NumberNode : public ExprNode
{
public:
	NumberNode(int value) : value(value) { type = NodeType::Number; }

	int getValue() const { return value; }

private:
	int value;
};

// ��ʶ���ڵ���
class IdentifierNode : public ExprNode
{
public:
	IdentifierNode(std::string name) : name(name) { type = NodeType::Identifier; }

	std::string getName() const { return name; }

private:
	std::string name;
};

// ������ڵ���
class OperatorNode : public ExprNode
{
public:
	OperatorNode(const std::string& op, ExprNode* left, ExprNode* right)
		: op(op), left(left), right(right)
	{
		type = NodeType::Operator;
	}


	std::string getOp() const { return op; }
	ExprNode* getLeft() const { return left; }
	ExprNode* getRight() const { return right; }

private:
	const std::string op;
	ExprNode* left;
	ExprNode* right;
};

// �������ýڵ���
class CallNode : public ExprNode
{
public:
	CallNode(std::string name, std::vector<ExprNode*> args)
		: name(name), args(args)
	{
		type = NodeType::FuncCall;
	}

	std::string getName() const { return name; }
	const std::vector<ExprNode*>& getArgs() { return args; }

private:
	std::string name;
	std::vector<ExprNode*> args;
};

// �����ڵ����
class DeclNode : public ASTNode
{
public:
	virtual ~DeclNode() {}
};

// ���������ڵ���
class VarDeclNode : public DeclNode
{
public:
	VarDeclNode(std::string name, ExprNode* init)
		: name(name), init(init)
	{
		type = NodeType::VarDecl;
	}

	std::string getName() const { return name; }
	ExprNode* getInit() const { return init; }

private:
	std::string name;
	ExprNode* init;
};

// ���������ڵ���
class FuncDeclNode : public DeclNode
{
public:
	FuncDeclNode(std::string name, std::vector<std::string> params, ASTNode* body)
		: name(name), params(params), body(body)
	{
		type = NodeType::FuncDecl;
	}

	std::string getName() const { return name; }
	const std::vector<std::string>& getParams() { return params; }
	ASTNode* getBody() const { return body; }

private:
	std::string name;
	std::vector<std::string> params;
	ASTNode* body;
};

// ���ڵ����
class StmtNode : public ASTNode
{
public:
	virtual ~StmtNode() {}
};

// ���ʽ���ڵ���
class ExprStmtNode : public StmtNode
{
public:
	ExprStmtNode(ExprNode* expr) : expr(expr) { type = NodeType::ExprStmt; }

	ExprNode* getExpr() const { return expr; }

private:
	ExprNode* expr;
};

// if���ڵ���
class IfStmtNode : public StmtNode
{
public:
	IfStmtNode(ExprNode* cond, ASTNode* thenBranch, ASTNode* elseBranch)
		: cond(cond), thenBranch(thenBranch), elseBranch(elseBranch)
	{
		type = NodeType::IfStmt;
	}

	ExprNode* getCond() const { return cond; }
	ASTNode* getThenBranch() const { return thenBranch; }
	ASTNode* getElseBranch() const { return elseBranch; }

private:
	ExprNode* cond;
	ASTNode* thenBranch;
	ASTNode* elseBranch;
};

// for���ڵ���
class ForStmtNode : public StmtNode
{
public:
	ForStmtNode(ExprNode* init, ExprNode* cond, ExprNode* update, ASTNode* body)
		: init(init), cond(cond), update(update), body(body)
	{
		type = NodeType::ForStmt;
	}

	ExprNode* getInit() const { return init; }
	ExprNode* getCond() const { return cond; }
	ExprNode* getUpdate() const { return update; }
	ASTNode* getBody() const { return body; }

private:
	ExprNode* init;
	ExprNode* cond;
	ExprNode* update;
	ASTNode* body;
};