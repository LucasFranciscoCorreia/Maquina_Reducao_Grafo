%{
#include <string.h>
#include "y.tab.h"
%}
%option noinput nounput

%%
[0-9]+			{yylval.str = yytext;return numero;}          //definicao de tokens numericos
[ \t]			{;}                                          //definicao de token para tabulacao
[-+/*]			{yylval.valor = yytext[0];return operador;} // definição de tokens para operador
[<>]			{yylval.valor = yytext[0];return logico;}   // definicao de tokes para operadores logicos maior e menor que
[=]			    {return atribuidor;}                        //definicao de token para atribuicao
"=="        	{yylval.valor = '=';return logico;}          //definicao de token de comparacao de igualdade, lógico
[?]			    {return ask;}             // definição de token aks, para imprimir a expressão que define a fução
\n			    {return quebra_linha;}   //definição de token para quebra de linha
"if"		    {return (IF);}           // definicao do token if
"then"      	{return (THEN);}         // definicao do token then
"else"      	{return (ELSE);}         // definicao do token else
"("			    {return AP;}             //definicao de token abre parenteses
")"			    {return FP;}             //definicao de token fecha parenteses
[a-zA-Z]+[a-zA-Z0-9]*	{int tam = strlen(yytext);
                        yylval.str = malloc(tam+1);
                        strcpy(yylval.str,yytext);
                        yylval.str[tam] = '\0';return alphanumerico;} //Definicao de token alphanumerico
"("[a-z]")"		{return argumento;}                                   //definicao de token para argumento
%%

int yywrap (void) {return 1;}

