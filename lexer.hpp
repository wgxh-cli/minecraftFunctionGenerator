#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>
using namespace std;
const int TOKEN_NUM = 0;
int TOKEN_EOF = 1;
int TOKEN_VAR = 2;// [_A-Za-z][_0-9A-Za-z]*
int TOKEN_DENG = 3;
int TOKEN_NUMBER = 4;//^[0-9]*$
int KUO = 5;
const int KONG_GE = 6;
int TOKEN_YINHAO = 7;
int TOKEN_STR = 8;
int TOKEN_DUOYINHAO = 9;
int TOKEN_RIGHT_KUOHAO = 10;


map<int, string> tokenNameMap = {
		{TOKEN_NUM,"%"},
		{TOKEN_EOF,"EOF"},
		{TOKEN_VAR,"var"},
		{TOKEN_DENG,"="},
		{TOKEN_NUMBER,"number"},
		{KUO,"("},
		{KONG_GE," "},
		{TOKEN_YINHAO,"\""},
		{TOKEN_STR,"str"},
		{TOKEN_DUOYINHAO,"\"\""}
};
class lexer
{
public:
	struct Lexer
	{
		 string sourceCode;
		 int lineNum;
		 string nextToken;
		 int nextTokenType;
		 int nextTokenLineNum;

		 pair<int, string> NextTokenIs(int tokenType) {

		 }
		 int LookAhead() {

		 }
		 tuple<int, int, string> MatchToken() {
				 //check ignored
			 if (isIgnored()) return make_tuple( lineNum,KONG_GE , "Ignored" );
				 //finish
			 if (sourceCode.length() == 0) return make_tuple(lineNum, TOKEN_EOF, tokenNameMap[TOKEN_EOF]);
				 //check token
				 switch (sourceCode[0])
				 {
				 case '%':
					 skipsrc(1);
					 return make_tuple(lineNum, TOKEN_NUM, "%");
				 case '(':
					 skipsrc(1);
					 return make_tuple(lineNum, KUO, "(");
				 case ')':
					 skipsrc(1);
					 return make_tuple(lineNum, TOKEN_RIGHT_KUOHAO, ")");
				 case '=':
					 skipsrc(1);
					 return make_tuple(lineNum, TOKEN_DENG, "=");
				 case '"':
					 if (next_code("\"\""))
					 {
						 skipsrc(2);
						 return make_tuple(lineNum, TOKEN_DUOYINHAO, "\"\"");
					 }
					 skipsrc(1);
					 return make_tuple(lineNum, TOKEN_YINHAO, "\"");
				 }

				 // check multiple character token
				 if (sourceCode[0] == '_' || isletter(sourceCode[0]))
				 {
					 string token = scan_name();
					 return make_tuple(lineNum, TOKEN_VAR, token);
				 }

				 //unexpected symbol
				 cout<<"error at matchtoken"<<endl;
				 throw "error at matchtoken";

		 }
		 bool next_code(string str) {
			 int len = str.length() - 1;
			 if (str.length() < sourceCode.length() && str.compare(sourceCode.substr(0, str.length())) == 0) {
				 sourceCode.erase(0, 2);
				 return true;
			 }
			 else
			 {
				 if (str.length() == sourceCode.length()) return true;
			 }
			 return false;
		 }
		 string scan_before_token(string token)
		 {
			 vector<string> s = split(sourceCode, token);
			 if (s.size() < 2)
			 {
				 cout<<"unreachable!";
				 throw "Exception";
			 }
			 skipsrc(s[0].length());
			 return s[0];
		 }
		 void skipsrc(int n) {
			 sourceCode = sourceCode.substr(n);
		 }
		 string scan_name() {
			 return regexscantoken(regex("^[_\d\w]+"));
		 }
		 string regexscantoken(regex regx) {
			 smatch res;
			 if (regex_match(sourceCode, res, regx)) {
				 skipsrc(res.length());
				 return res[0];
			 }
			 cout << "error at regexscantoken" << endl;
			 throw "Error at regexscantoken";
		 }
		 int GetLineNum()
		 {
			 return lineNum;
		 }
		 bool isIgnored() {
			 bool isIgnored = false;
			 auto isWhiteSpace = [](char c) -> bool {
				 switch (c)
				 {
				 case '\t':
				 case '\n':
				 case '\v':
				 case '\f':
				 case '\r':
				 case ' ':
					 return true;
				 default:
					 return false;
				 }
			 };
			 while (sourceCode.length() > 0)
			 {
				 {
					 //isNewLine
					 if (next_code("\r\n") || next_code("\n\r"))
					 {
						 skipsrc(2);
						 lineNum++;
						 isIgnored = true;
					 }
					 else if (sourceCode[0] == '\r' || sourceCode[0] == '\n')
					 {
						 skipsrc(1);
						 lineNum++;
						 isIgnored = true;
					 }
					 //isWhiteSpace
					 else if (isWhiteSpace(sourceCode[0]))
					 {
						 skipsrc(1);
						 isIgnored = true;
					 }
					 else
					 {
						 break;
				 }
				 return isIgnored;
			 }
			 }
		 }

	};



private:
	static bool isletter(char ch) {
		if (ch <= 'z' && ch >= 'a' || ch <= 'Z' && ch >= 'a') return true;
		return false;
	}
	static vector<string> split(string& str, string& pattern)
	{
		vector<string> res;
		if (str == "")
			return res;
		//在字符串末尾也加入分隔符，方便截取最后一段
		string strs = str + pattern;
		size_t pos = strs.find(pattern);

		while (pos != strs.npos)
		{
			string temp = strs.substr(0, pos);
			res.push_back(temp);
			//去掉已分割的字符串,在剩下的字符串中进行分割
			strs = strs.substr(pos + 1, strs.size());
			pos = strs.find(pattern);
		}

		return res;
	}
	
};


