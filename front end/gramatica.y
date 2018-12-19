%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "backend.c"
typedef struct{
	char args;
	char *expr;
	char* nome;
}Funcao;
Funcao *funcoes;
int i;

char* compilar(char *s);
void salvar_cond(char *op, char* expr1, char* expr2);
void salvar_op(char *op, char* then, char* Else);
void salvarFuncaoVar(char *fun, char *expr);
void salvarFuncaoExpr(char *fun, char *expr);
void buscarFuncao(char *fun);
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
%token equals
%token IF THEN ELSE
%type <str> alphanumerico
%%

programa	:	programa expr quebra_linha				{compilar($<str>2);}
		|	programa func quebra_linha				{;}
		|	programa ifthenelse quebra_linha			{;}
		|	programa condicao quebra_linha				{;}
		|	%empty	
		;

op		:	logico							{$<str>$ = $<str>1;}
		;

condicao	:	op expr	expr						{salvar_cond($<str>1,$<str>2, $<str>3);}
		;

ifthenelse	:	IF condicao THEN expr ELSE expr				{salvar_op($<str>2, $<str>4, $<str>6);}    
		;

expr		:	operador expr expr					{$<str>$ = salvar_expr($<valor>1, $<str>2,$<str>3);}
		|	numero							{$<str>$ = salvar_numero($<str>1);}
		|	alphanumerico						{$<str>$ = $<str>1;}
		;

func		:	alphanumerico atribuidor expr				{salvarFuncaoVar($<str>1,$<str>3);}
		|	alphanumerico argumento atribuidor ifthenelse		{salvarFuncaoExpr($<str>1, $<str>4);}
		| 	alphanumerico ask					{buscarFuncao($<str>1);}
		;
%%

void salvar_cond(char* op, char* expr1, char* expr2){
	int res1, res2;
	switch(*op){
		case '<':
			res1 = iniciar(expr1);
			res2 = iniciar(expr2);
			if(res1 < res2)
				printf("TRUE");
			else
				printf("FALSE");
			break;
		case '>':
			res1 = iniciar(expr1);
			res2 = iniciar(expr2);
			if(res1 > res2)
				printf("TRUE");
			else
				printf("FALSE");
			break;
		case '=':	
			res1 = iniciar(expr1);
			res2 = iniciar(expr2);
			if(res1 == res2)
				printf("TRUE");
			else
				printf("FALSE");
			break;
	}
}

void salvar_op(char* op, char *then, char* Else){
	printf("ifthenelse");
}

void buscarFuncao(char* fun){
	int j;
	for(j = 0; j < i;j++){
		if(!strcmp(fun, funcoes[j].nome)){
			printf("%s = %s\n", funcoes[j].nome, funcoes[j].expr);
			return;
		}
	}
}
void salvarFuncaoVar(char* fun, char* expr){
	funcoes[i].nome = fun;
	funcoes[i].expr = expr;
	funcoes[i].args = 0;
	i++;
}

void salvarFuncaoExpr(char* fun, char* expr){
	funcoes[i].nome = fun;
	funcoes[i].expr = expr;
	funcoes[i].args = 1;
}

char* compilar(char* s){
	//printf("%s\n", s);
	int res = iniciar(s);
	free(s);
	printf("%d\n", res);
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
	funcoes = malloc(100*sizeof(Funcao));
	i = 0;
	alocar_memoria();
	return yyparse();
}

void yyerror(const char* s){
	fprintf(stderr, "%s\n", s);
}
