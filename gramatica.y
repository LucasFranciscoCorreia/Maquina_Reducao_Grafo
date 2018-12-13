%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
typedef struct Iden{
	char nome[256];
	int valor;
}Iden;
Iden *atribuicoes;
int max = 10;
int qntAtribuicoes;
void atribuir(char* s, int d);
void buscarAtribuicao(char *s);
int buscarValor(char *s);
int calcula(char op, int a, int b);
void yyerror(const char* s);
int yylex(void);
%}

%union{ char *str; int valor;}
%start programa
%token identificador
%token numero
%token operador
%token atribuidor
%token ask
%token print
%type <str> identificador
%type <str> print
%%

programa	:	programa expr				{printf("%d\n", $<valor>2);}
		|	programa identificador ask		{buscarAtribuicao($<str>2);}
		|	programa identificador atribuidor expr 	{atribuir($<str>2, $<valor>4);}
		|	%empty	
		;
		;
expr		:	operador expr expr 			{$<valor>$ = calcula($<valor>1, $<valor>2, $<valor>3);}
		|	operador identificador expr		{if(buscarValor($<str>2) >= 0) {
									$<valor>$ = calcula($<valor>1, buscarValor($<str>2), $<valor>3);
								} else { 
									yyerror("%s não existe", $<str>2);
									operador;
								}
								}
		| 	operador expr identificador		{if(buscarValor($<str>3) >= 0){
									$<valor>$ = calcula($<valor>1, $<valor>2, buscarValor($<str>3));
								} else {
									yyerror("%s não existe", $<str>3);
									operador;
								}
								}
		|	operador identificador identificador	{if
(buscarValor($<str>2) >= 0 && buscarValor($<str>3) >= 0){
									$<valor>$ = calcula($<valor>1, buscarValor($<str>2), buscarValor($<str>3));
								} else { 
									yyerror("%s não existe", $<str>2);
									operador;
								}
								}
		|	numero					{$<valor>$ = $<valor>1;}
		;

%%

void buscarAtribuicao(char *s){
	int i;
	for(i = 0; i < qntAtribuicoes;i++){
		if(!strcmp(s,atribuicoes[i].nome)){
			printf("%s = %d\n", atribuicoes[i].nome, atribuicoes[i].valor);
			return;		
		}
	}
}

int buscarValor(char *s){
	int i;
	for(i = 0; i < qntAtribuicoes;i++){
		if(!strcmp(s, atribuicoes[i].nome)){
			return atribuicoes[i].valor;
		}
	}
	return -1;
}

void atribuir(char *s, int d){
		strcpy(atribuicoes[qntAtribuicoes].nome, s);
		atribuicoes[qntAtribuicoes].valor = d;
		qntAtribuicoes++;
}

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
	qntAtribuicoes = 0;
	atribuicoes = malloc(max*sizeof(Iden));
	return yyparse();
}

void yyerror(const char* s){
	fprintf(stderr, "%s\n", s);
}
