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
char* avaliar_funcao(char* fun, char *valor);
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
char* novo_salvar_expr(char op, char *a, char *b);

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

programa	:	programa expr quebra_linha				             {printf("%s\n", compilar($<str>2));}
		|	programa func quebra_linha				             {;}
		|	programa ifthenelse quebra_linha                                     {;}
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

expr		:	operador expr expr                       {$<str>$ = salvar_expr($<valor>1, $<str>2,$<str>3);}
		|	numero		                         {$<str>$ = salvar_numero($<str>1);}
		| 	alphanumerico				 {;}
		|	alphanumerico AP expr FP                 {$<str>$ = avaliar_funcao($<str>1, compilar($<str>3));}
		;

func    	:	alphanumerico atribuidor expr				                {salvarFuncaoVar($<str>1,$<str>3);}
		|	alphanumerico AP alphanumerico FP atribuidor ifthenelse			{salvarFuncaoExpr($<str>1, $<str>6);}
		|	alphanumerico AP alphanumerico FP atribuidor expr			{salvarFuncaoExpr($<str>1, $<str>6);}
		| 	alphanumerico ask					                        {$<str>$ = buscarFuncao($<str>1);printf("%s", $<str>$);}
		;
%%


/*
  Procedumento utilizado para avaliar a fução transformada em lógica combinatorial.
  Argumentos:
  	char *fun : string contendo a função em associatividade a esquerda, com termos separados em parênteses
  		    if(<termo lógico>)then(<termo para o then>)else(<termo else>) Caso a função tenha if then else
  		    os tokens "if then else" são removidos e a fução é retornada em lógica
  		    combinatorial
  	char *valor: valor a ser aplicado a função
*/
char * avaliar_funcao(char* fun, char* valor){

	//Função é convertida para logica combinatorial
	char* res = converter_para_bracket(fun, buscarFuncao(fun), valor);
	int a = iniciar(res);

	int tam_string=1;

	char *result_eval = calloc(20,sizeof(char));

	sprintf(result_eval,"%d",a);
	return result_eval;
}

/*Procedimento pega uma expressao condicional transforma-a numa
string com associatividade a esquerda e a retorna.
Argumentos:
 	char op: operador codicional.
 	char *expr1: expressao do argumento 1. Já com associatividade a esquerda
 	char *expr2: expressao no argumento 2. Já com associatividade a esquerda
*/
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

/*Procedimento utilizado na clausula if then else
para construir a string if then else a ser transformada
em logica combinatorial.
A string "if then else" e retornada
Argumentos:
	char *op: expressao logica
	char *then: expressao para caso o  termo then precise ser executado.
	char *op2: operador aritemtico a ser avaliado.
	char *alpha: identificador da funcao.
	char *expr1: expressão a ser passada como argumento
	char *expr2: outra expressao a ser utilizado
*/
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

/*Procedimento utilizado na clausula if then else
para construir a string if then else a ser transformada
em logica combinatorial.
A string "if then else" e retornada
Argumentos:
	char *op: expressao logica
	char *then: expressao para caso o  termo then precise ser executado.
	char *op2: operador aritemtico a ser avaliado.
	char *alpha1: identificador da funcao.
	char *expr1: expressao a ser passada como argumento do identificador 1
	char *alpha2: identificador da funcao.
	char *expr2: expressao a ser passada como argumento do identificador 2
*/
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

/*Procedimento utilizado na clausula if then else
para construir a string if then else a ser transformada
em logica combinatorial.
A string "if then else" e retornada
Argumentos:
	char *op: expressao logica
	char *then: expressao para caso o  termo then precise ser executado.
	char *Else: expressao para caso o  termo Else precise ser executado.
*/
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

/*Procedimento efetua uma busca no array de estrutura
funcoes, baseado num alphanumerico(identificador da funcao) digitado pelo
usuario. E retorna a expressao correspondente ao identificador da funcao
Argumentos:
	char *fun : identificador da fucao
*/
char* buscarFuncao(char* fun){
	int j;
	for(j = 0; j < i;j++){
		if(!strcmp(fun, funcoes[j].nome)){
			return funcoes[j].expr;
		}
	}
}

/*Procedimento salva a expressao da funcao junto com o seu identificador
e expressao correspondente no array de estruturas.
Argumentos:
	char *fun: Identificador da funcao,
	char *expr: expressao correspondente ao identifi
*/
void salvarFuncaoVar(char* fun, char* expr){
	funcoes[i].nome = fun;
	funcoes[i].expr = expr;
	funcoes[i].args = 0;
	i++;
}

/*Procedimento salva uma expressao de uma funcao
transformada em associatividade a esquerda bem
como a expressao
Argumentos:
	char *fun: string da fucao
	char *expr: string da expressao
*/
void salvarFuncaoExpr(char* fun, char* expr){
	funcoes[i].nome = fun;
	funcoes[i].expr = expr;
	funcoes[i].args = 1;
	i++;
}

char* compilar(char* s){
	//printf("\nFunção compilar\n expr: %s\n",s);
	int res = iniciar(s);
	char var[100];
	sprintf(var, "%d", res);
	char *resul = malloc(strlen(var)+1);
	strcpy(resul, var);
	free(s);
	return resul;
}

/*Procedimento em que expressão aritmetica e salva em com
associatividade a esquerda, para ser avaliada pelo back end
Argumentos:
	char op : operador aritmetico,
	char *a : primeiro argumento do operador
	char *b : segundo argumento do operador
*/
char* salvar_expr(char op, char *a, char *b){
	int tam1 = strlen(a);
	int tam2 = strlen(b);
	char *aux = calloc(tam1+tam2+7,sizeof(char));
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


/*Procedimento pega o token numero e o copia  para uma variável axiliar e retorna-o
*/
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
