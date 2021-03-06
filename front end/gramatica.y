%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "backend.c"
#include "bracket.c"
typedef struct{
	char args;
	char *expr;
	char* nome;
}Funcao;
Funcao *funcoes;
int i;

char* compilar(char *s);
void avaliar_funcao(char* fun, char* valor);
char* eval_cond(char op, char* expr1, char* expr2);
char* eval_op(char *op, char* then, char* Else);
char* eval_op1(char* op, char* then,char op2, char* alpha, char* expr1, char* expr2);
char* eval_op2(char* op, char* then,char op2, char* alpha1, char* expr1, char* alpha2, char* expr2);
char* salvar_app(char* alpha, char* expr);
void salvarFuncaoVar(char *fun, char *expr);
void salvarFuncaoExpr(char *fun, char *expr);
char* buscarFuncao(char *fun);
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
%token equals
%token IF 
%token THEN 
%token ELSE
%token AP FP
%type <str> alphanumerico

%%

programa	:	programa expr quebra_linha				{compilar($<str>2);}
		|	programa func quebra_linha				{;}
		|	programa ifthenelse quebra_linha			{;}
		| 	programa alphanumerico numero quebra_linha		{avaliar_funcao($<str>2, $<str>3);}
		|	%empty
		;

condicao	:	logico expr expr					{$<str>$ = eval_cond($<valor>1,$<str>2, $<str>3);}
		;

ifthenelse	:	IF condicao THEN expr ELSE expr						{$<str>$ = eval_op($<str>2, $<str>4, $<str>6);}
		|	IF condicao THEN expr ELSE operador alphanumerico AP expr FP expr	
			{$<str>$ = eval_op1($<str>2, $<str>4, $<valor>6, $<str>7, $<str>9,$<str>11);}
		|	IF condicao THEN expr ELSE operador expr alphanumerico AP expr FP	
			{$<str>$ = eval_op1($<str>2, $<str>4, $<valor>6, $<str>8, $<str>10,$<str>7);}
		| 	IF condicao THEN expr ELSE operador alphanumerico AP expr FP alphanumerico AP expr FP 
			{$<str>$ = eval_op2($<str>2, $<str>4, $<valor>6, $<str>7, $<str>9, $<str>11, $<str>13);}
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

void avaliar_funcao(char* fun, char* valor){
	char* res = converter_para_bracket(fun, buscarFuncao(fun), valor);
	int a = iniciar(res);
	printf("%d\n", a);
}

char* eval_cond(char op, char *expr1, char *expr2){
    int tam1 = strlen(expr1);
    int tam2 = strlen(expr2);
    char* eval = malloc(tam1+tam2+5);
    eval[0] = op;
    eval[1] = '(';
    strcpy(eval+2,expr1);
    eval[2+tam1] = ')';
    eval[3+tam1] = '(';
    strcpy(eval+tam1+4, expr2);
    eval[tam1+tam2+4] = ')';
    free(expr1);
    free(expr2);
    return eval;
}


char* eval_op1(char* op, char* then,char op2, char* alpha, char* expr1, char* expr2){
	int tam1 = strlen(op);
	int tam2 = strlen(then);
	int tam3 = strlen(alpha);
	int tam4 = strlen(expr1);
	int tam5 = strlen(expr2);
	char* res = malloc(tam1+tam2+tam3+tam4+tam5+23);
	res[0] = 'i';
	res[1] = 'f';
	res[2] = '(';
	strcpy(res+3,op);
	res[tam1+3] = ')';
	res[tam1+4] = 't';
	res[tam1+5] = 'h';
	res[tam1+6] = 'e';
	res[tam1+7] = 'n';
	res[tam1+8] = '(';
	strcpy(res+tam1+9, then);
	res[tam1+tam2+9] = ')';
	res[tam1+tam2+10] = 'e';
	res[tam1+tam2+11] = 'l';
	res[tam1+tam2+12] = 's';
	res[tam1+tam2+13] = 'e';
	res[tam1+tam2+14] = '(';
	res[tam1+tam2+15] = op2;
	res[tam1+tam2+16] = '(';
	strcpy(res+tam1+tam2+17, alpha);
	res[tam1+tam2+tam3+17] ='(';
	strcpy(res+tam1+tam2+tam3+18, expr1);
	res[tam1+tam2+tam3+tam4+18] = ')';
	res[tam1+tam2+tam3+tam4+19] = ')';
	res[tam1+tam2+tam3+tam4+20] = '(';
	strcpy(res+tam1+tam2+tam3+tam4+21, expr2);
	res[tam1+tam2+tam3+tam4+tam5+21] = ')';
	res[tam1+tam2+tam3+tam4+tam5+22] = ')';
	free(op);
	free(then);
	free(alpha);
	free(expr1);
	free(expr2);
	return res;
}

char* eval_op2(char* op, char* then,char op2, char* alpha1, char* expr1, char* alpha2, char* expr2){
	int tam1 = strlen(op);
	int tam2 = strlen(then);
	int tam3 = strlen(alpha1);
	int tam4 = strlen(expr1);
	int tam5 = strlen(alpha2);
	int tam6 = strlen(expr2);
	char* res = malloc(tam1+tam2+tam3+tam4+tam5+tam6+26);
	res[0] = 'i';
	res[1] = 'f';
	res[2] = '(';
	strcpy(res+3,op);
	res[tam1+3] = ')';
	res[tam1+4] = 't';
	res[tam1+5] = 'h';
	res[tam1+6] = 'e';
	res[tam1+7] = 'n';
	res[tam1+8] = '(';
	strcpy(res+tam1+9,then);
	res[tam1+tam2+9]=')';
	res[tam1+tam2+10]='e';
	res[tam1+tam2+11]='l';
	res[tam1+tam2+12]='s';
	res[tam1+tam2+13]='e';
	res[tam1+tam2+14]='(';
	res[tam1+tam2+15] = op2;	
	res[tam1+tam2+16] = '(';
	strcpy(res+tam1+tam2+17, alpha1);
	res[tam1+tam2+tam3+17] = '(';
	strcpy(res+tam1+tam2+tam3+18, expr1);
	res[tam1+tam2+tam3+tam4+18] = ')';
	res[tam1+tam2+tam3+tam4+19] = ')';
	res[tam1+tam2+tam3+tam4+20] = '(';
	strcpy(res+tam1+tam2+tam3+tam4+21, alpha2);
	res[tam1+tam2+tam3+tam4+tam5+21] = '(';
	strcpy(res+tam1+tam2+tam3+tam4+tam5+22, expr2);
	res[tam1+tam2+tam3+tam4+tam5+tam6+22] = ')';
	res[tam1+tam2+tam3+tam4+tam5+tam6+23] = ')';
	res[tam1+tam2+tam3+tam4+tam5+tam6+24] = ')';
	res[tam1+tam2+tam3+tam4+tam5+tam6+25] = '\0';
	return res;
}

char* eval_op(char* op, char *then, char* Else){
	int tam1 = strlen(op);
	int tam2 = strlen(then);
	int tam3 = strlen(Else);
	char* res = malloc(tam1+tam2+tam3+16);
	res[0] = 'i';
	res[1] = 'f';
	res[2] = '(';
	strcpy(res+3,op);
	res[tam1+3] = ')';
	res[tam1+4] = 't';
	res[tam1+5] = 'h';
	res[tam1+6] = 'e';
	res[tam1+7] = 'n';
	res[tam1+8] = '(';
	strcpy(res+tam1+9,then);
	res[tam1+tam2+9]=')';
	res[tam1+tam2+10]='e';
	res[tam1+tam2+11]='l';
	res[tam1+tam2+12]='s';
	res[tam1+tam2+13]='e';
	res[tam1+tam2+14]='(';
	strcpy(res+tam1+tam2+15,Else);
	res[tam1+tam2+tam3+15] = ')';
	return res;
}

char* buscarFuncao(char* fun){
	int j;
	for(j = 0; j < i;j++){
		if(!strcmp(fun, funcoes[j].nome)){
			return funcoes[j].expr;
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
	i++;
}

char* compilar(char* s){
	printf("%s\n", s);
	int res = iniciar(s);
	printf("%d\n", res);
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
	funcoes = malloc(100*sizeof(Funcao));
	i = 0;
	alocar_memoria();
	return yyparse();
}

void yyerror(const char* s){
	fprintf(stderr, "%s\n", s);
}
