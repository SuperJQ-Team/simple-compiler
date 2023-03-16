#include <algorithm>

#include "Function.h"
#include "Executer.h"
#include "Matrix.h"


void Function::pushInstruction(const std::vector<Token>& tokens)
{
	instructions.emplace_back(tokens);
}

Variable Function::run(const std::stack<Variable>& _args, Executer* parent)
{
	// 先创建instruction的备份，然后往备份里放各个argument的定义
	// 然后ext逐tokens执行备份内容
	std::stack<Variable> args = _args;
	Executer ext(parent);

	if (args.size() != arguments.size())
	{
		// raise error
		return Variable::err;
	}
	for (int i = 0; i < arguments.size(); ++i, args.pop())
	{
		ext.RegisterVarialbe(arguments[i], args.top());
	}
	for (auto& tokens : instructions)
	{
		//printf("function running\n");
		auto v = ext.Execute(tokens);
		if (v.type != __Variable::_error && v.type != __Variable::_null)return v;
	}
	//printf("function stopped\n");
	return Variable::nul;
}

void Function::RegisterGlobalFunc(Executer& executer)
{
	executer.RegisterFunction("print", new PrintFunc());
	executer.RegisterFunction("println", new PrintLnFunc());
	executer.RegisterFunction("welcome", new WelcomeFunc());
	executer.RegisterFunction("help", new HelpFunc());
	executer.RegisterFunction("det", new DetFunc());
	executer.RegisterFunction("E", new EFunc());
	executer.RegisterFunction("T", new TFunc());
}

Variable WelcomeFunc::run(const std::stack<Variable>& args, Executer* parent)
{
	UI::PrintLog("Calculator v1.1. Licensed by GPL v4.0.\n\n");
	UI::PrintLog("Type \"help()\" for more info.\n");
	return Variable::nul;
}

Variable PrintFunc::run(const std::stack<Variable>& _args, Executer* parent)
{
	std::stack<Variable> args = _args;
	bool first = true;
	while (!args.empty())
	{
		if (!first) UI::PrintLog(" ");
		first = false;
		UI::Print(args.top());
		args.pop();
	}
	return Variable::nul;
}

static const std::string help_info = " \
Help:\n \
\n \
Global Functions: \n \
help(): Show this info\n \
print([args]): print something...\n \
println([args]): print something with enter(s)... \n \
welcome(): print welcome-info\n \
det(Matrix): calculate matrix determinant \n \
\n \
Keywords: \n \
let VAR_NAME (= VALUE)*: define variable as VAR_NAME (and initialized as VALUE)\n \
def FUNC_NAME '(' ARG1 (, ARG2(, ARG3...)) ')': define function as FUNC_NAME\n \
\n \
";

Variable HelpFunc::run(const std::stack<Variable>& args, Executer* parent)
{
	UI::PrintLog(help_info);
	return Variable::nul;
}

Variable DetFunc::run(const std::stack<Variable>& args, Executer* parent)
{
	if (args.size() != 1) return Variable::err;
	auto _args = args;
	Matrix mat = *((Matrix*)(_args.top().value));
	if (mat.row != mat.col) return Variable::err;

	std::vector<int> index;
	for (int i = 0; i < mat.row; ++i) index.emplace_back(i);
	int sign = 1;
	double result = 0;
	do
	{
		double tmp = 1;
		for (int i = 0; i < mat.row; ++i)
		{
			tmp = tmp * mat.buffer[i * mat.col + index[i]];
		}
		if (sign) result += tmp;
		else result -= tmp;
		sign ^= 1;
	} while (std::next_permutation(index.begin(), index.end()));
	Variable res(__Variable::_float);
	res.value = new double(result);
	return res;
}

ForFunc::ForFunc(const std::vector<Token>& init, const std::vector<Token>& judge, const std::vector<Token>& pass) :init(init), judge(judge), pass(pass)
{
	name = "@for";
	arguments = {};
}

Variable ForFunc::run(const std::stack<Variable>& args, Executer* parent)
{
	Executer ext(parent);
	ext.Execute(init);
	while (Variable::iftrue(ext.Calculate(judge, 0)))
	{
		for (auto& tokens : instructions)
		{
			auto v = ext.Execute(tokens);
			if (v.type != __Variable::_error && ext.ifreturn)
			{
				parent->ifreturn = ext.ifreturn;
				return v;
			}
		}
		ext.Execute(pass);
	}
	return Variable::nul;
}

Variable PrintLnFunc::run(const std::stack<Variable>& _args, Executer* parent)
{
	std::stack<Variable> args = _args;
	if (args.empty())UI::PrintLog("\n");
	while (!args.empty())
	{
		UI::PrintLog("\n");
		UI::Print(args.top());
		args.pop();
	}
	return Variable::nul;
}

Variable EFunc::run(const std::stack<Variable>& _args, Executer* parent)
{
	std::stack<Variable> args = _args;
	if (args.empty() || args.top().type != __Variable::_int) return Variable::err;
	int k = *((int*)(args.top().value));
	args.pop();
	if (args.size() != 0 && args.size() != 2) return Variable::err;
	int ia = -1, ib = -1;
	if (args.size() == 2)
	{
		if (args.top().type != __Variable::_int) return Variable::err;
		ia = *((int*)args.top().value) - 1;
		args.pop();
		if (args.top().type != __Variable::_int) return Variable::err;
		ib = *((int*)args.top().value) - 1;
		args.pop();
		if (ia >= k || ib >= k) return Variable::err;
	}
	Variable var(__Variable::_matrix);
	var.value = new Matrix(k, k);
	Matrix* m = (Matrix*)var.value;
	for (int i = 0; i < k; ++i)
	{
		m->buffer[i * k + i] = 1;
	}
	if (ia != -1 && ib != -1)
	{
		m->buffer[ia * k + ia] = 0;
		m->buffer[ia * k + ib] = 1;
		m->buffer[ib * k + ib] = 0;
		m->buffer[ib * k + ia] = 1;
	}
	return var;
}

Variable TFunc::run(const std::stack<Variable>& _args, Executer* parent)
{
	std::stack<Variable> args = _args;
	if (args.empty() || args.top().type != __Variable::_matrix) return Variable::err;
	Matrix m = *((Matrix*)(args.top().value));
	if (m.GetCol() != m.GetRow())return Variable::err;

	for (int i = 0; i < m.GetCol(); ++i)
	{
		for (int j = i; j < m.GetRow(); ++j)
		{
			double temp = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = temp;
		}
	}
	Variable var(__Variable::_matrix);
	var.value = new Matrix(m);
	return var;
}