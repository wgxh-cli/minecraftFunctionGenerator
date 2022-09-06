#pragma once
#include <iostream>
#include <string>
#include <lexer.hpp>
#include <vector>
using namespace std;
string t="";
lexer LEXER=lexer("");

class parser
{
public:

	struct Variable
	{
            int line_num;
		    string name;
            int type;
      //  string value;

	};
	struct Assignment {
        int line_num;
		Variable var;
        Variable asgnvar;
        string value;
        int value_type;//0: value 1: variable 2: null
	};
    struct Statement
{
   parser:: Variable varstm;
    parser::Assignment asgstm;
    int type;//0 asg 1 var
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
            Lexer->LookAheadAndSkip(lexer::KONG_GE);
            switch (Lexer->LookAhead())
            {
                case lexer::TOKEN_NUM:
                    pair<Assignment,string> pagn=parseAssignment(Lexer,lexer::TOKEN_NUM);
                    Statement stm;
                    stm.asgstm=pagn.first;
                    stm.type=0;
                    return make_pair(stm,pagn.second);
            }
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
        pair<Src,string> src=parseSourceCode(&Lexer);
		Lexer.NextTokenIs(LEXER.TOKEN_EOF);
        cout<<"123"<<endl;
        return src;

	}
    void parseVariable(lexer *Lexer,Variable *var,int token){
          switch(token){
            case lexer::TOKEN_NUM:
                Lexer->NextTokenIs(lexer::TOKEN_NUM);
                   
              //  Lexer->LookAheadAndSkip(lexer::KONG_GE);
                    
            //    cout<<"ok"<<endl;
           
            //    cout<<"123";
              //  Lexer->NextTokenIs(lexer::TOKEN_VAR);
             // asign.var.name
          
             var->line_num=Lexer->GetLineNum();
            // cout<<"123";
             var->type=lexer::TOKEN_NUM;
             var->name=parseName(Lexer);
          //   cout<<var->type<<endl;

           //     Lexer->LookAheadAndSkip(lexer::KONG_GE);
                //if(Lexer->LookAhead()==lexer::TOKEN_SMT_END) asign
        }

    }
    string parseName(lexer *Lexer){
        pair<int,string> varname=Lexer->NextTokenIs(lexer::TOKEN_VAR);
    //    cout<<varname.second<<endl;
        return varname.second;
    }
    string parseNumber(lexer *Lexer){
         pair<int,string> varname=Lexer->NextTokenIs(lexer::TOKEN_NUMBER);
       //  cout<<varname.second<<endl;
         return varname.second;
    }
    pair<Assignment,string>parseAssignment(lexer *Lexer,int token){
        Assignment asign;
        asign.line_num=Lexer->GetLineNum();
        parseVariable(Lexer,&asign.var,token);
       
       // cout<<"123";
     //   cout<<"1"<<endl;
        Lexer->LookAheadAndSkip(lexer::KONG_GE);
        Lexer->NextTokenIs(lexer::TOKEN_DENG);
      // cout<<"1"<<endl;
        Lexer->LookAheadAndSkip(lexer::KONG_GE );
        
     //   cout<<"1"<<endl;
        asign.value_type=0;
        if(asign.var.type==lexer::TOKEN_NUM) asign.value=parseNumber(Lexer);
      //  cout<<"1"<<endl;
        Lexer->NextTokenIs(lexer::TOKEN_SMT_END);
         
        return make_pair(asign,t);
    }



private:
	bool isSourceCodeEnd(int token)
        {
            if (token == LEXER.TOKEN_EOF)
                return true;
            return false;
        }
};

