#include <iostream>
using namespace std;
class t{
public:
int a;
};
class tt: public t{
public:
int b;
};
int main(){
	t aa;
	aa.a=1;
	tt bb;
	bb.b=2;
//	aa=static_cast<tt *>(bb);
	(tt)aa=bb;
//	cout<<aa->b;
	cout<<typeid(aa).name();	
	return 0;
}
