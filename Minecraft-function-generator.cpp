// Minecraft-function-generator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
//#include <lexer.hpp>
#include <string>
#include <parser.hpp>
//#include <stdio.h>
#include <fstream>
#include <backend.hpp>
using namespace std;

int main()
{
  string code="";
  ifstream f;
  f.open("t.in");
  char c = f.get();
  while(f.good()) {
    code=code+c;
    c = f.get();
  }
  cout << code << endl;
  string codee;
  backend b;
  codee=b.Execute(code);
  
  cout<<codee;
}
