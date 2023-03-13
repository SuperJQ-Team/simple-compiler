#include "ASTNode.h"

ASTNode::ASTNode(double value) : type(Type::Number), value(value) {}
ASTNode::ASTNode(const Token& token) : type(Type::Keyword), name(token.value) {}

ASTNode::ASTNode(Type type, const std::string& op, ASTNode* left, ASTNode* right) :
	type(type), op(op), left(left), right(right)
{
}

ASTNode::ASTNode(const std::string& name, const std::vector<ASTNode*>& arguments) :
	type(Type::FunctionCall), name(name), arguments(arguments)
{
}

ASTNode::ASTNode(ASTNode* function, const std::vector<ASTNode*>& arguments) :
	type(Type::FunctionCall), arguments(arguments), name(function->name)
{

}

ASTNode::~ASTNode()
{
	if (left) delete left;
	if (right) delete right;
	for (auto argument : arguments)
	{
		delete argument;
	}
}

ASTNode::Type ASTNode::GetType() const { return type; }
double ASTNode::GetValue() const { return value; }
const std::string& ASTNode::GetName() const { return name; }
const std::string& ASTNode::GetOp() const { return op; }
ASTNode* ASTNode::GetLeft() const { return left; }
ASTNode* ASTNode::GetRight() const { return right; }
const std::vector<ASTNode*>& ASTNode::GetArguments() const { return arguments; }

void ASTDFS(ASTNode* cur, int depth)
{
	if (cur == nullptr)
	{
		for (int i = 0; i < depth; ++i) printf(" ");
		printf("nullptr\n");
		return;
	}
	for (int i = 0; i < depth; ++i) printf(" ");
	printf("{\n");
	for (int i = 0; i < depth; ++i) printf(" ");
	printf("Name: %s\n", cur->name.c_str());
	for (int i = 0; i < depth; ++i) printf(" ");
	printf("Left:\n");
	for (int i = 0; i < depth; ++i) printf(" ");
	ASTDFS(cur->left, depth + 1);
	for (int i = 0; i < depth; ++i) printf(" ");
	printf("Right:\n");
	for (int i = 0; i < depth; ++i) printf(" ");
	ASTDFS(cur->left, depth + 1);
	for (int i = 0; i < depth; ++i) printf(" ");
	printf("Arguments:\n");
	for (auto argument : cur->arguments)
	{
		ASTDFS(argument, depth + 1);
	}
	for (int i = 0; i < depth; ++i) printf(" ");
	printf("}\n");
}


//ASTNode::ASTNode() :type(NodeType::Undefine)
//{
//}
//
//void ASTNode::CreatFromExpression(std::vector<Token> tokens)
//{
//	int maxindex = -1;
//	int maxpr = -1;
//	for (int i = 0; i < tokens.size(); ++i)
//	{
//		
//	}
//}
