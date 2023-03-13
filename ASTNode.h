#pragma once
#include <vector>
#include "Token.h"

class ASTNode
{
public:
    enum class Type
    {
        Number,
        Keyword,
        BinaryOperator,
        UnaryOperator,
        FunctionCall
    };

    ASTNode(double value);
    ASTNode(const Token& token);
    ASTNode(Type type, const std::string& op, ASTNode* left, ASTNode* right);
    ASTNode(const std::string& name, const std::vector<ASTNode*>& arguments);
    ASTNode(ASTNode* function, const std::vector<ASTNode*>& arguments);

    ~ASTNode();

    Type GetType() const;
    double GetValue() const;
    const std::string& GetName() const;
    const std::string& GetOp() const;
    ASTNode* GetLeft() const;
    ASTNode* GetRight() const;
    const std::vector<ASTNode*>& GetArguments() const;

    friend void ASTDFS(ASTNode* cur, int depth);

private:
    Type type;
    double value;
    std::string name;
    std::string op;
    ASTNode* left = nullptr;
    ASTNode* right = nullptr;
    std::vector<ASTNode*> arguments;
};

//class ASTNode
//{
//public:
//	enum class NodeType
//	{
//		Undefine = -2,
//		Error = -1,
//		variable = 0,
//		Keyword,
//		binaryOperator,
//		unaryOperator,
//		FunctionCall
//	};
//
//	NodeType type;
//	Token value;
//	std::vector<ASTNode*> childs;
//
//	ASTNode();
//
//	void CreatFromExpression(std::vector<Token> tokens);
//};