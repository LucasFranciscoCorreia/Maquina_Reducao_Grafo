%{
#include <string.h>
#include "y.tab.h"
%}
%option noinput nounput
%%

[a-zA-Z]+[a-zA-Z0-9]*	{int tam = strlen(yytext);yylval.str = malloc(tam+1);strcpy(yylval.str,yytext);yylval.str[tam] = '\0';return identificador;}
[0-9]+			{yylval.valor = atoi(yytext);return numero;}
[ \t\n]			{;}
[-+/*=;]		{yylval.valor = yytext[0];return operador;}
:=			{return atribuidor;}
[?]			{return ask;}
print			{return print;}
%%
int yywrap (void) {return 1;}

