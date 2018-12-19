%{
#include <string.h>
#include "y.tab.h"
%}
%option noinput nounput

%%
[a-zA-Z]+[a-zA-Z0-9]*	{int tam = strlen(yytext);yylval.str = malloc(tam+1);strcpy(yylval.str,yytext);yylval.str[tam] = '\0';return alphanumerico;}
[0-9]+			{yylval.str = yytext;return numero;}
[ \t]			{;}
[-+/*]			{yylval.valor = yytext[0];return operador;}
[<>]			{yylval.valor = yytext[0];return logico;}
[=]			{return atribuidor;}
"=="        		{yylval.valor = '=';return equals;}
[?]			{return ask;}
\n			{return quebra_linha;}
"if"		        {return IF;}
"then"      		{return THEN;}
"else"      		{return ELSE;}
"("[a-z]")"		{return argumento;}
%%

int yywrap (void) {return 1;}

