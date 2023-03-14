#include "ASTNode.h"

void DFSAST(ASTNode* node, int deepth)
{
	if (deepth == 0)
		printf("\nASTNode dfs begin:\n");

	int bs = deepth * 4;
	if (node == nullptr)
	{
		printf("%*snullptr\n", bs, "");
		return;
	}
	switch (node->GetType())
	{
	case NodeType::Base:
	{
		printf("%*s{\n", bs, "");
		printf("%*sType:base\n", bs, "");
		printf("%*s}\n", bs, "");
	}
	break;
	case NodeType::Number:
	{
		NumberNode& n = *(NumberNode*)node;
		printf("%*s{\n", bs, "");
		printf("%*sType: number\n", bs, "");
		printf("%*svalue: %d\n", bs, "", n.getValue());
		printf("%*s}\n", bs, "");
	}
	break;
	case NodeType::Identifier:
	{
		IdentifierNode& n = *(IdentifierNode*)node;
		printf("%*s{\n", bs, "");
		printf("%*sType: identifier\n", bs, "");
		printf("%*sname: %s\n", bs, "", n.getName().c_str());
		printf("%*s}\n", bs, "");
	}
	break;
	case NodeType::Operator:
	{
		OperatorNode& n = *(OperatorNode*)node;
		printf("%*s{\n", bs, "");
		printf("%*sType: operator\n", bs, "");
		printf("%*sop: %s\n", bs, "", n.getOp().c_str());
		printf("%*sleft:\n", bs, "");
		DFSAST(n.getLeft(), bs + 1);
		printf("%*sright:\n", bs, "");
		DFSAST(n.getRight(), bs + 1);
		printf("%*s}\n", bs, "");
	}
	break;
	case NodeType::FuncCall:
	{
		CallNode& n = *(CallNode*)node;
		printf("%*s{\n", bs, "");
		printf("%*sType: function call\n", bs, "");
		printf("%*sname: %s\n", bs, "", n.getName().c_str());
		for (int i = 0; i < n.getArgs().size(); ++i)
		{
			printf("%*sargs %d:\n", bs, "", i);
			DFSAST(n.getArgs()[i], bs + 1);
		}
		printf("%*s}\n", bs, "");
	}
	break;
	case NodeType::FuncDecl:
	{
		FuncDeclNode& n = *(FuncDeclNode*)node;
		printf("%*s{\n", bs, "");
		printf("%*sType: function decl\n", bs, "");
		printf("%*sname: %s\n", bs, "", n.getName().c_str());
		for (int i = 0; i < n.getParams().size(); ++i)
		{
			printf("%*sparams %d: %s\n", bs, "", i, n.getParams()[i].c_str());
		}
		printf("%*sbody:\n", bs, "");
		DFSAST(n.getBody(), bs + 1);
		printf("%*s}\n", bs, "");
	}
	break;
	case NodeType::VarDecl:
	{
		VarDeclNode& n = *(VarDeclNode*)node;
		printf("%*s{\n", bs, "");
		printf("%*sType: variable decl\n", bs, "");
		printf("%*sname: %s\n", bs, "", n.getName().c_str());
		printf("%*sinit:\n", bs, "");
		DFSAST(n.getInit(), bs + 1);
		printf("%*s}\n", bs, "");
	}
	break;
	case NodeType::ExprStmt:
	{
		ExprStmtNode& n = *(ExprStmtNode*)node;
		printf("%*s{\n", bs, "");
		printf("%*sType: expr stmt\n", bs, "");
		printf("%*sexprstmt:\n", bs, "");
		DFSAST(n.getExpr(), bs + 1);
		printf("%*s}\n", bs, "");
	}
	break;
	case NodeType::IfStmt:
	{
		IfStmtNode& n = *(IfStmtNode*)node;
		printf("%*s{\n", bs, "");
		printf("%*sType: if stmt\n", bs, "");
		printf("%*scond:\n", bs, "");
		DFSAST(n.getCond(), deepth + 1);
		printf("%*sthen branch:\n", bs, "");
		DFSAST(n.getThenBranch(), deepth + 1);
		printf("%*selse branch:\n", bs, "");
		DFSAST(n.getElseBranch(), deepth + 1);
		printf("%*s}\n", bs, "");
	}
	break;
	case NodeType::ForStmt:
	{
		ForStmtNode& n = *(ForStmtNode*)node;
		printf("%*s{\n", bs, "");
		printf("%*sType: for stmt\n", bs, "");
		printf("%*sinit:\n", bs, "");
		DFSAST(n.getInit(), deepth + 1);
		printf("%*scond:\n", bs, "");
		DFSAST(n.getCond(), deepth + 1);
		printf("%*supdate:\n", bs, "");
		DFSAST(n.getUpdate(), deepth + 1);
		printf("%*sbody:\n", bs, "");
		DFSAST(n.getBody(), deepth + 1);
		printf("%*s}\n", bs, "");
	}
	break;
	default:
		printf("%*sunknowing node", bs, "");
		break;
	}
}
