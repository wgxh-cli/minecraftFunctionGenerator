/* word_char_counter_cpp.l */
%option c++
%option noyywrap

%{
#include <cstring>
int chars = 0;
int words = 0;
%}

%%
[a-zA-Z]+ { chars += strlen(yytext); words++; }

. { }
%%

int main(int argc, char **argv) {
    FlexLexer* lexer = new yyFlexLexer();
    lexer->yylex();
    std::cout << "I found " << words << " words of " << chars << " chars." << std::endl;
    return 0;
}
