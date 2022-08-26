#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>
using namespace std;

class lexer
{
public:
	static inline const int TOKEN_NUM = 0;
	static inline const int TOKEN_EOF = 1;
	static inline const int TOKEN_VAR = 2; // [_A-Za-z][_0-9A-Za-z]*
	static inline const int TOKEN_DENG = 3;
	static inline const int TOKEN_NUMBER = 4; //^[0-9]*$
	static inline const int KUO = 5;
	static inline const int KONG_GE = 6;
	static inline const int TOKEN_YINHAO = 7;
	static inline const int TOKEN_STR = 8;
	static inline const int TOKEN_DUOYINHAO = 9;
	static inline const int TOKEN_RIGHT_KUOHAO = 10;
	static inline const int TOKEN_SMT_END = 11;

	map<int, string> tokenNameMap = {
		{TOKEN_NUM, "%"},
		{TOKEN_EOF, "EOF"},
		{TOKEN_VAR, "var"},
		{TOKEN_DENG, "="},
		{TOKEN_NUMBER, "number"},
		{KUO, "("},
		{KONG_GE, " "},
		{TOKEN_YINHAO, "\""},
		{TOKEN_STR, "str"},
		{TOKEN_DUOYINHAO, "\"\""}};

public:
	string sourceCode;
	int lineNum;
	string nextToken;
	int nextTokenType;
	int nextTokenLineNum;
	lexer(string src)
	{
		sourceCode = src;
		lineNum = 1;
		nextToken = "";
		nextToken = "";
		nextTokenType = 1;
		nextTokenLineNum = 0;
	}
	pair<int, string> NextTokenIs(int TokenType)
	{
		tuple<int, int, string> res = GetNextToken();
		int nowLineNum, nowTokenType;
		string nowToken;
		tie(nowLineNum, nowTokenType, nowToken) = res;
		// syntax error
		if (TokenType != nowTokenType)
		{
			cout << "error at NextTokenIs" <<nowTokenType<< endl;
			throw "error at NextTokenIs";
		}
		return make_pair(nowLineNum, nowToken);
	}
	int LookAhead()
	{
		// lexer.nextToken* already setted
		if (nextTokenLineNum > 0)
			return nextTokenType;
		// set it
		int nowLineNum = lineNum;
		tuple<int, int, string> res = GetNextToken();
		int ln, tokenType;
		string token;
		tie(ln, tokenType, token) = res;
		lineNum = nowLineNum;
		nextTokenLineNum = ln;
		nextTokenType = tokenType;
		nextToken = token;
		return tokenType;
	}

	void LookAheadAndSkip(int expectedType)
	{
		// get next token
		int nowLineNum = lineNum;
		tuple<int, int, string> res = GetNextToken();
		int ln, tokenType;
		string token;
		std::tie(ln, tokenType, token) = res;
		//	cout<<token<<endl;
		// not is expected type, reverse cursor
		if (tokenType != expectedType)
		{
			lineNum = nowLineNum;
			nextTokenLineNum = ln;
			nextTokenType = tokenType;
			nextToken = token;
		}
	}

	tuple<int, int, string> GetNextToken()
	{
		if (nextTokenLineNum > 0)
		{
			int ln = nextTokenLineNum;
			lineNum = nextTokenLineNum;
			nextTokenLineNum = 0;	
			return make_tuple(ln, nextTokenType, nextToken);
		}
	//	cout<<"need to matchtoken"<<endl;
	tuple<int,int,string> result=MatchToken();
	int ln,tt;
	string t;
	tie(ln,tt,t)=result;
//	cout<<tt<<endl;
		return result;
	}

	tuple<int, int, string> MatchToken()
	{
	//	cout<<sourceCode[0]<<endl;
		// check ignored
		if (isIgnored())
			return make_tuple(lineNum, KONG_GE, "Ignored");
		// finish
		if (sourceCode.length() == 0)
			return make_tuple(lineNum, TOKEN_EOF, tokenNameMap[TOKEN_EOF]);
		//cout<<sourceCode[0]<<endl;
		
		// check token
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
		case ';':
			skipsrc(1);
			return make_tuple(lineNum, TOKEN_SMT_END, ";");
		}
		

		// check multiple character token
		if (sourceCode[0] == '_' || 
		
		
		isletter(sourceCode[0]))
		{
			string token = scan_name();
			//cout<<token<<endl;
			return make_tuple(lineNum, TOKEN_VAR, token);
		}
		if (sourceCode[0] >= '0' && sourceCode[0] <= '9')
		{
			//cout<<sourceCode[0]<<endl;
			string n=scan_number();
		//	cout<<"num: "<<n<<endl;
		//	string a=n+"123";
		//	cout<<a<<endl;
			return make_tuple(lineNum, TOKEN_NUMBER, n);
		}

		// unexpected symbol
		cout << "error at matchtoken" << endl;
		throw "error at matchtoken";
	}
	bool next_code(string str)
	{
		int len = str.length() - 1;
		if (str.length() < sourceCode.length() && str.compare(sourceCode.substr(str.length())) == 0)
		{
		//	sourceCode.erase(0, 2);
			return true;
		}
		else
		{
			if (str	 == sourceCode)
				return true;
		}
		return false;
	}
	string scan_before_token(string token)
	{
		vector<string> s = split(sourceCode, token);
		if (s.size() < 2)
		{
			cout << "unreachable!";
			throw "Exception";
		}
		skipsrc(s[0].length());
		return s[0];
	}
	void skipsrc(int n)
	{
		sourceCode = sourceCode.substr(n);
	}
	string scan_name()
	{
		string name="";
		name=sourceCode[0];
		skipsrc(1);
		while(sourceCode[0]=='_' || isletter(sourceCode[0])){
			name=name+sourceCode[0];
			skipsrc(1);
		}
	//	cout<<name;
		return name;
		//std::regex t("[a-z]+");
		//return regexscantoken(t);
	}
	string scan_number()
	{

		string name="";
		name=sourceCode[0];
		skipsrc(1);
		while(sourceCode[0]>='0' && sourceCode[0]<='9'){
			name=name+sourceCode[0];
			skipsrc(1);
		}
	//	cout<<name;
		return name;
	//	return regexscantoken(regex("^[0-9]*$"));
	}
	string regexscantoken(regex regx)
	{
		std::smatch res;
		if (std::regex_match(sourceCode, res, regx))
		{
			skipsrc(res.length());
			return res[0];
		}
		cout << "error at regexscantoken" << endl;
		cout<<"at "<<lineNum<<endl;
		//cout<<sourceCode<<res[0];
		 throw "Error at regexscantoken";
	}
	int GetLineNum()
	{
		return lineNum;
	}
	bool isIgnored()
    {
        bool iskong = false;
        auto isWhiteSpace=[] (char c) -> bool
        {
                    switch (c)
                    {
                        case '\t':
						return true;
                        case '\n':
						return true;
                        case '\v':
						return true;
                        case '\f':
						return true;
                        case '\r':
							return true;
                        case ' ':
                            return true;
                    }
					return false;
					
                };
                //matching
				int i=0;
                while (sourceCode.length() > 0)
                {

					//i++;
                    //isNewLine
                    if (next_code("\n\r") || next_code("\r\n"))
                    {
					//	cout<<"1231"<<endl;
                        skipsrc(2);
                        lineNum++;
                        iskong = true;
                    }
                    if (sourceCode[0] == '\r' || sourceCode[0] == '\n')
                    {
					//	cout<<"1232"<<endl;
                        skipsrc(1);
                        lineNum++;
                        iskong = true;
                    }
                    //isWhiteSpace
                    else if (isWhiteSpace(sourceCode[0]))
                    {
					//	cout<<"1233"<<endl;
                        skipsrc(1);
                        iskong = true;
                    }
                    else
                    {
                        break;
                    }
                }
				//cout<<"123123"<<i<<endl;
                return iskong;
            }
         

private:
	static bool isletter(char ch)
	{
		if (ch <= 'z' && ch >= 'a' || ch <= 'Z' && ch >= 'a')
			return true;
		return false;
	}
	static vector<string> split(string &str, string &pattern)
	{
		vector<string> res;
		if (str == "")
			return res;
		//���ַ���ĩβҲ����ָ����������ȡ���һ��
		string strs = str + pattern;
		size_t pos = strs.find(pattern);

		while (pos != strs.npos)
		{
			string temp = strs.substr(0, pos);
			res.push_back(temp);
			//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
			strs = strs.substr(pos + 1, strs.size());
			pos = strs.find(pattern);
		}

		return res;
	}
};
