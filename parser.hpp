#pragma once
#include <iostream>
#include "lexer.hpp";
using namespace std;
class Statement
{};
class parser
{
public:
	struct Variable: public Statement
	{
		int line_num;
		string name;
	};
	struct Assignment : public Statement {
		int line_num;
		Variable* var;
		string Str;
	};
	struct Src
	{
		int line_num;
		Statement statements[];
	};


private:

};

