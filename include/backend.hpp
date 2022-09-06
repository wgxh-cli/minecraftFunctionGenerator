#pragma once
#include <iostream>
#include <string>
#include <parser.hpp>
#include <vector>
using namespace std;

class backend{
    public:
      //    string Execute(string code);
      string Execute(string code){
        parser Parser;
        parser::Src ast;
        pair<parser::Src,string> astt=Parser.parse(code);
        ast=astt.first;
        
        vector<parser::Statement> stm=ast.statements;
       // if(astt.second!="") cout<<astt.second<<endl,throw astt.second;
        //resolve

        return resolveAST(&stm);
    }
      string resolveAST(vector<parser::Statement> *ast){
        string src;

        for(int i=0;i<ast->size();i++){
            //parser::Statement *stm=&ast[i];
            string srct=resolveStatement(&ast->at(i));
            src=src+srct;
        }
        return src;
    }
      string resolveStatement(parser::Statement *stm){
        
        if(stm->type==0) return translangAssignment(stm);
    }
      string translangAssignment(parser::Statement *stm){
        string scoreborad=stm->asgstm.var.name;
        string value=stm->asgstm.value;
        return "scoreboard objective add var dummy\nscoreboard players set "+scoreborad+" var "+value+"\n";
    }
};
