#include <iostream>
#include <utility>
using namespace std;


typedef enum {
  TOKEN_EQUALS,
  TOKEN_POINT,
  TOKEN_LARROW,
  TOKEN_RARROW,
  TOKEN_COMMA,
  TOKEN_BACK_SLASH,
  TOKEN_COLON,
  TOKEN_SEMICOLON,
  TOKEN_QUOTE,
  TOKEN_SINGLE_QUOTE,
  TOKEN_ASTERISK,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_DOLLAR,
  TOKEN_BRACE,
  TOKEN_PARENTHESIS,
  TOKEN_TILDE,
} TokenType;

TokenType from_token(char source);
char from_token_type(TokenType token_type);

struct Token {
  char* value;
  TokenType token_type;
};

class Parse {
public:
  // A function that parse the input and returns the remaining parts of input and parsed token.
  virtual pair<string, Token> parse(string source);
};
