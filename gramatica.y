%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int qntVariaveis;
int qntFuncoes;
char* compilar(char *s);
char* salvar_numero(char *s);
char* salvar_expr(char op, char *a, char *b);
void yyerror(const char* s);
int yylex(void);
%}

%nonassoc IFX
%nonassoc ELSE
%union{ char *str; int valor;}
%start programa
%token numero
%token operador
%token atribuidor
%token alphanumerico
%token argumento
%token logico
%token ask
%token quebra_linha
%token blank
%type <str> alphanumerico
%%

programa	:	programa expr quebra_linha	{compilar($<str>2);}
		|	programa func			{;}
		|	%empty	
		;

expr		:	operador expr expr			{$<str>$ = salvar_expr($<valor>1, $<str>2,$<str>3);}
		|	numero					{$<str>$ = salvar_numero($<str>1);}
		;

func		:	alphanumerico atribuidor expr			{;}
		|	alphanumerico argumento atribuidor expr		{;}
		| 	alphanumerico ask				{;}
		;
%%

char* compilar(char* s){
	printf("%s\n", s);
	//iniciar(s);
	free(s);
	return s;
}
char* salvar_expr(char op, char *a, char *b){
	int tam1 = strlen(a);
	int tam2 = strlen(b);
	char *aux = malloc(tam1+tam2+7);
	aux[0] = op;
	aux[1] = '(';
	strcpy(aux+2, a);
	aux[tam1+2] = ')';
	aux[tam1+3] = '(';
	strcat(aux, b);
	aux[tam1+tam2+4] = ')';
	aux[tam1+tam2+5] = '\0';
	free(a);
	free(b);
	int i= 0, j = 0;
	while(aux[j]){
		if(aux[j] == ' '){
			j++;
		}else{
			aux[i] = aux[j];
			i++;
			j++;
		}
	}
	aux[i] = '\0';
	return aux;
}
char* salvar_numero(char *s){
	int tam = strlen(yylval.str);
	char *aux = malloc(tam+1);
	strcpy(aux, s);
	aux[tam] = '\0';
	return aux;
}

int main(void){
	qntVariaveis = 0;
	qntFuncoes = 0;
	return yyparse();
}

void yyerror(const char* s){
	fprintf(stderr, "%s\n", s);
}
