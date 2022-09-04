#include <iostream>
#include <regex>
#include <string>
using namespace std;
int main(){
	string str;
	cin>>str;
	std::regex r("[a-z]+");
	smatch s;
	bool a=true;
	 a=regex_match(str,s,r);
	cout<<a;
	return 0;
}
