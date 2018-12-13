%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
typedef struct Iden{
	char nome[256];
}Iden;
Iden *atribuicoes;
int qntAtribuicoes;
int calcula(char op, int a, int b);
void yyerror(const char* s);
int yylex(void);
%}

%start programa
%token identificador
%token numero
%token operador
%%

programa	:	programa expr		{printf("%d\n",$2);}
		|	%empty	
		;
		;
expr		:	identificador '=' expr 	{printf("%s = %d", $1, $3);}
		| 	operador expr expr 	{$$ = calcula($1, $2, $3);}
		|	numero			{$$ = $1;}
		;

%%

int calcula(char op, int a, int b){
	switch(op){
		case '+':
			return a+b;
		case '-':
			return a-b;
		case '*':
			return a*b;
		case '/':
			return a/b;
		
	}
	return 0;
}

int main(void){
	return yyparse();
}

void yyerror(const char* s){
	fprintf(stderr, "%s\n", s);
}
