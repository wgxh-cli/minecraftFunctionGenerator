#define DEF_TOKEN(token_name, token_type) class ##token_name: public Parse {\
public:\
  pair<string, ##token_type> parse() {\
  }\
};
#include <iostream>
#include "lexer.hpp"
using namespace std;

