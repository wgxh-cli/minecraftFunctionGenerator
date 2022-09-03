// Minecraft-function-generator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
//#include <lexer.hpp>
#include <string>
#include <parser.hpp>
//#include <stdio.h>
#include <fstream>
using namespace std;

int main()
{
  string code="";
  ifstream f;
  f.open("t.in");
  char c[1024]={0};
  while(f>>c) code=code+c;
   // freopen("t.in","r",stdin);
    
  //  cin>>code;
    parser Parser;
    parser::Src ast;
    
    pair<parser::Src,string> astande= Parser.parse(code);
    ast=astande.first;
    
    vector<parser::Statement> stms=ast.statements;
   // cout<<typeid(stms[0].asgstm).name();
   for(int i=0;i<stms.size();i++) if(stms[i].type==0) cout<<"Assignment:\nvar_type:"<<stms[i].asgstm.var.type<<"\nvar_name:"<<stms[i].asgstm.var.name<<"\nassignment_type:"<<stms[i].asgstm.value_type<<"\nvalue"<<stms[i].asgstm.value<<endl;
   
  /*
    string tokentext="%=====";
   // cin>>tokentext;
   // std::cout << tokentext;
    lexer Lexer(tokentext);
    cout<<Lexer.sourceCode<<endl;
    cout<<Lexer.LookAhead()<<endl;
    Lexer.NextTokenIs(Lexer.LookAhead());
   // Lexer.LookAheadAndSkip(6);
    cout<<Lexer.sourceCode<<endl;
    cout<<Lexer.LookAhead()<<endl;

    cout<<Lexer.sourceCode<<endl;*/
    //cout<<Lexer.LookAhead();
   // Lexer.NextTokenIs(0);
   // while(Lexer.LookAhead()!=6) cout<<Lexer.LookAhead()<<endl,Lexer.NextTokenIs(Lexer.LookAhead());
 // cout<<Lexer.LookAhead()<<endl;
  //Lexer.NextTokenIs(Lexer.LookAhead());

  //cout<<Lexer.nextTokenType<<endl;
//  Lexer.skipsrc(0);
 // cout<<Lexer.sourceCode<<endl;

    //cout<<Lexer.LookAhead()<<endl;



}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
