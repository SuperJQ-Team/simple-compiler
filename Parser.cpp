#if 1

#include "Parser.h"

#include <stdexcept>

/*

define -> (def FUNCNAME)| (let VARNAME ("=" value)*)
expr -> value | (expr (("+" | "-" | "*" | "/"...) expr)*) | ("-" value)
value -> NUMBER | MATRIX | VARIABLE | STRING | funccall

funccall -> FUNCNAME "(" (NUMBER ("," value("," value...))) ")"

*/

std::string tokenTypeToString(TokenType token)
{
	if (token == TokenType::Error) return "error";
	if (token == TokenType::Number) return "number";
	if (token == TokenType::Operator) return "option";
	if (token == TokenType::Keyword) return "keyword";
	if (token == TokenType::Variable) return "variable";
	if (token == TokenType::String) return "string";
	if (token == TokenType::Matrix) return "matrix";
	if (token == TokenType::Function)return "function";
	if (token == TokenType::End)return "end";
	return "unknown";

}

// Parser��
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

// �������е�����
std::vector<DeclNode*> Parser::parseDecls()
{
	std::vector<DeclNode*> decls;
	while (!isEnd())
	{
		decls.push_back(parseDecl());
	}
	return decls;
}

ASTNode* Parser::parse()
{
	while (!isEnd())
	{
		std::string s = peek().value;
		if (s == "for") return parseForStmt();
		else if (s == "if") return parseIfStmt();
		else if (s == "let") return parseVarDecl();
		else if (s == "def") return parseFuncDecl();
		else return parseStmt();
		next();
	}
	return nullptr;
}

// ����Ƿ񵽴�����ĩβ
bool Parser::isEnd()
{
	return current >= tokens.size();
}

// ��ȡ��ǰλ�õ�token
Token Parser::peek()
{
	if (isEnd())
	{
		throw std::runtime_error("Unexpected end of input");
	}
	return tokens[current];
}

// ��ȡ��һ��λ�õ�token
Token Parser::next()
{
	if (isEnd())
	{
		throw std::runtime_error("Unexpected end of input");
	}
	return tokens[++current];
}

// ��鵱ǰλ�õ�token�Ƿ�ƥ���������
bool Parser::match(TokenType type)
{
	if (peek().type == type)
	{
		next();
		return true;
	}
	return false;
}

// ��鵱ǰλ�õ�token�Ƿ�ƥ��������ͣ������ƥ�����׳��쳣
void Parser::expect(TokenType type)
{
	if (!match(type))
	{
		//throw std::runtime_error("Expected " + tokenTypeToString(type) + ", got " + tokenTypeToString(peek().type));
	}
}

// ����һ������
DeclNode* Parser::parseDecl()
{
	if (match(TokenType::Variable))
	{
		return parseVarDecl();
	}
	if (match(TokenType::Function))
	{
		return parseFuncDecl();
	}
	throw std::runtime_error("Expected var or func declaration, got " + tokenTypeToString(peek().type));
}

// ����һ����������
VarDeclNode* Parser::parseVarDecl()
{
	expect(TokenType::Keyword);
	std::string name = peek().value;
	ExprNode* init = nullptr;
	if (match(TokenType::Operator))
	{
		init = parseExpr();
	}
	//expect(TokenType::SEMICOLON);
	return new VarDeclNode(name, init);
}


// ����һ����������
FuncDeclNode* Parser::parseFuncDecl()
{
	expect(TokenType::Keyword);
	std::string name = peek().value;
	expect(TokenType::LeftParen);
	std::vector<std::string> params;
	while (!match(TokenType::RightParen))
	{
		expect(TokenType::Variable);
		params.push_back(peek().value);
		if (!match(TokenType::Comma))
		{
			expect(TokenType::RightParen);
			break;
		}
	}
	ASTNode* body = parseStmt();
	return new FuncDeclNode(name, params, body);
}

// ����һ�����
StmtNode* Parser::parseStmt()
{
	if (match(TokenType::If))
	{
		return parseIfStmt();
	}
	if (match(TokenType::For))
	{
		return parseForStmt();
	}
	return parseExprStmt();
}

// ����һ�����ʽ���
ExprStmtNode* Parser::parseExprStmt()
{
	ExprNode* expr = parseExpr();
	expect(TokenType::End);
	return new ExprStmtNode(expr);
}


// ����һ��if���
IfStmtNode* Parser::parseIfStmt()
{
	expect(TokenType::LeftParen);
	ExprNode* cond = parseExpr();
	expect(TokenType::RightParen);
	ASTNode* thenBranch = parseStmt();
	ASTNode* elseBranch = nullptr;
	if (match(TokenType::Else))
	{
		elseBranch = parseStmt();
	}
	return new IfStmtNode(cond, thenBranch, elseBranch);
}


// ����һ��for���
ForStmtNode* Parser::parseForStmt()
{
	expect(TokenType::LeftParen);
	ExprNode* init = nullptr;
	if (!match(TokenType::Semicolon))
	{
		init = parseExpr();
		expect(TokenType::Semicolon);
	}
	ExprNode* cond = nullptr;
	if (!match(TokenType::Semicolon))
	{
		cond = parseExpr();
		expect(TokenType::Semicolon);
	}
	ExprNode* update = nullptr;
	//if (!match(TokenType::Rparen))
	//{
	//	update = parseExpr();
	//	expect(TokenType::Rparen);
	//}
	ASTNode* body = parseStmt();
	return new ForStmtNode(init, cond, update, body);
}

// ����һ�����ʽ
ExprNode* Parser::parseExpr()
{
	ExprNode* result = parseTerm();
	while (peek().type == TokenType::Operator && (peek().value == "+" || peek().value == "-"))
	{
		std::string op = peek().value;
		next();
		result = new OperatorNode(op, result, parseExpr());
	}
	return result;
}

//// ����һ����
ExprNode* Parser::parseTerm()
{
	ExprNode* result=nullptr;// = parseFactor();
	while (peek().type == TokenType::Operator && (peek().value == "*" || peek().value == "/"))
	{
		std::string op = peek().value;
		next();
		result = new OperatorNode(op, result, parseTerm());
	}
	return result;
}

#endif