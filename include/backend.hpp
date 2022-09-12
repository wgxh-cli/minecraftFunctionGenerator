#pragma once
#include <iostream>
#include <string>
#include <parser.hpp>
#include <vector>
using namespace std;

class backend{
    public:
    vector<parser::Variable> v;
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
         string init="";
        if(v.empty()) init="scoreboard objective add var dummy\n";
     //   string value=stm->asgstm.value;
     if(stm->asgstm.value_type==0){
      string value=stm->asgstm.value;
       
        v.push_back(stm->asgstm.var);
        return init + "scoreboard players set "+scoreborad+" var "+value+"\n";
     }
     else{
      string asignv=stm->asgstm.asgnvar.name;
     // cout<<"123"<<endl;
      if(!vnameexist(asignv)) {cout<<"var not found at line "<<stm->asgstm.line_num;return "123";}
      for(int i=0;i<v.size();i++) {
        if(v[i].name==asignv) {
          if(v[i].type!=stm->asgstm.var.type) {cout<<"var not found at line "<<stm->asgstm.line_num;return "123";}
          return init + "scoreboard players set "+scoreborad+" var "+v[i].value+"\n";
        }
      }
      
     }
    }
    private:
    bool vnameexist(string name){
      for(int i=0;i<v.size();i++){
        if(v[i].name==name) return true;
      //  cout<<v[i].name<<endl;
      }
      return false;
    }
};
