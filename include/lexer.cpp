#include <map>
#include <sys/types.h>
#include "lexer.hpp"
using namespace std;

template<typename K, typename V>
map<V, K> swap_k_v(map<K, V> source) {
  map<V, K> swaped_map;
  for (const auto& [key, value] : source) {
    swaped_map.insert(pair<V, K>(value, key));
  }
  return swaped_map;
}

static const map<TokenType, char> token_char = {
  {TOKEN_EQUALS, '='},
  {TOKEN_POINT, '.'},
  {TOKEN_LARROW, '<'},
  {TOKEN_RARROW, '>'},
  {TOKEN_COMMA, ','},
  {TOKEN_BACK_SLASH, '\\'},
  {TOKEN_COLON, ':'},
  {TOKEN_SEMICOLON, ';'},
  {TOKEN_QUOTE, '"'},
  {TOKEN_SINGLE_QUOTE, '\''},
  {TOKEN_ASTERISK, '*'},
  {TOKEN_AND, '&'},
  {TOKEN_OR, '|'},
  {TOKEN_BRACE, '{'},
  {TOKEN_PARENTHESIS, '('},
  {TOKEN_DOLLAR, '$'},
  {TOKEN_TILDE, '~'},
};
static const map<char, TokenType> char_token = swap_k_v(token_char);

TokenType from_token(char source) {
  return char_token.find(source)->second;
}

char from_token_type(TokenType token_type) {
  return token_char.find(token_type)->second;
}
