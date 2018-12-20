%{
#include <string.h>
#include "y.tab.h"
%}
%option noinput nounput

%%
[0-9]+			{yylval.str = yytext;return numero;}
[ \t]			{;}
[-+/*]			{yylval.valor = yytext[0];return operador;}
[<>]			{yylval.valor = yytext[0];return logico;}
[=]			{return atribuidor;}
"=="        		{yylval.valor = '=';return logico;}
[?]			{return ask;}
\n			{return quebra_linha;}
"if"		        {return (IF);}
"then"      		{return (THEN);}
"else"      		{return (ELSE);}
"("			{return AP;}
")"			{return FP;}
[a-zA-Z]+[a-zA-Z0-9]*	{int tam = strlen(yytext);yylval.str = malloc(tam+1);strcpy(yylval.str,yytext);yylval.str[tam] = '\0';return alphanumerico;}
"("[a-z]")"		{return argumento;}
%%

int yywrap (void) {return 1;}

