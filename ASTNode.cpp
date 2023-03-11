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