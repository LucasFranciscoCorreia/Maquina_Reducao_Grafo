%{
#include "y.tab.h"
%}
%option noinput nounput
%%

[a-zA-Z]	{return identificador;}
[0-9]+		{yylval = atoi(yytext);return numero;}
[ \t\n]		{;}
[-+/*=;]	{yylval = yytext[0];return operador;}

%%
int yywrap (void) {return 1;}

