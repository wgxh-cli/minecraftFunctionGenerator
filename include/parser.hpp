#pragma once
#include <iostream>
#include <string>
#include <lexer.hpp>
#include <vector>
using namespace std;
string t="";
lexer LEXER=lexer("");
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
		vector<Statement> statements;
	};
	 pair<vector<Statement>, string> parseStatements(lexer *Lexer)
        {
            vector<Statement> statements = vector<Statement>();
			//string t="";
            while (!isSourceCodeEnd(Lexer ->LookAhead()))
            {
                pair<Statement, string> statementst = parseStatement(Lexer);
                if (statementst.second!=t)
                    return make_pair(statements, statementst.second);
                statements.push_back(statementst.first);
            }
            return make_pair(statements, t);
        }

        pair<Statement, string> parseStatement(lexer *Lexer)
        {
            Lexer->LookAheadAndSkip(Lexer->KONG_GE);
         /*   switch (Lexer->LookAhead())
            {
                case lexeTOKEN_PRINT:
                    return parsePrint(ref Lexer);
                case lexer.TOKEN_VAR_PREFIX:
                    return parseAssignment(ref Lexer);
                default:
                    return (null, "parseStatement(): unknown Statement.");
            }*/
        }

        

        //SourceCode ::= Statement+ 
        pair<const Src, string> parseSourceCode(lexer *Lexer)
        {
            auto src = Src();
            string error;
            src.line_num = Lexer->GetLineNum();
            pair<vector<Statement>, string> statements = parseStatements(Lexer);
            src.statements=statements.first;
			if (statements.second != t)
                return make_pair(src, statements.second);
            return make_pair(src, t);
        }

	pair<Src,string>parse(string code){
		lexer Lexer=lexer(code);
		Lexer.NextTokenIs(LEXER.TOKEN_EOF);

	}



private:
	bool isSourceCodeEnd(int token)
        {
            if (token == LEXER.TOKEN_EOF)
                return true;
            return false;
        }
};

