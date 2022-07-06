#pragma once
#include <iostream>
#include <string>
#include <map>
using namespace std;
class lexer
{
public:
	int TOKEN_NUM = 0;
	int TOKEN_EOF = 1;
	int TOKEN_VAR = 2;// [_A-Za-z][_0-9A-Za-z]*
	int TOKEN_DENG = 3;
	int TOKEN_NUMBER = 4;//^[0-9]*$
	int KUO = 5;
	int KONG_GE = 6;
	int TOKEN_YINHAO = 7;
	int TOKEN_STR = 8;
	int TOKEN_DUOYINHAO = 9;
	struct Lexer
	{
		 string sourceCode;
		 int lineNum;
		 string nextToken;
		 int nextTokenType;
		 int nextTokenLineNum;

		 map<int, string> NextTokenIs(int tokenType) {

		 }
		 int LookAhead() {

		 }
		 map<int, int, string> MatchToken() {
			 if (isIgnored()) {

			 }
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

			 }
		 }

	};



private:
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
};


