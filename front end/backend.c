#include <stdio.h>
#include "backend.h"

struct Celula{
    int tipo;
    struct Celula *direita, *esquerda;
};

struct Pilha{
    struct Celula *cell;
};

struct Celula *heap;
struct Celula *heap2;
struct Celula *end_heap;
struct Celula *hp;
struct Celula *cp;
struct Celula *raiz;
struct Celula *S, *K, *I, *B, *C, *D, *E, *F, *TRUE, *FALSE, *GT, *GET, *LT, *LET, *EQ, *PLUS, *SUB, *MULT, *DIV, *Y, *HD, *TL, *MAP, *POW, *EMPTY_LIST;
struct Pilha redex[TAM/1000];
struct Pilha *p;
int celulas;
int garbage_calls = 0;

char* entr;

void imprime_arvore(struct Celula* no){
    static int deph = 0;
    static int lista = 0;
    if(deph == 0 && no->tipo == -25)
        lista = 1;
    if(lista && no->tipo == -25)
        printf("[");
    if(no->esquerda) {
        deph++;
        imprime_arvore(no->esquerda);
    }
    switch (no->tipo){
        case -1:
            break;
        case -2:
            printf("(");
            break;
        case -3:
            printf("S");
            break;
        case -4:
            printf("K");
            break;
        case -5:
            printf("I");
            break;
        case -6:
            printf("B");
            break;
        case -7:
            printf("C");
            break;
        case -8:
            printf("D");
            break;
        case -9:
            printf("E");
            break;
        case -10:
            printf("F");
            break;
        case -11:
            printf("TRUE");
            break;
        case -12:
            printf("FALSE");
            break;
        case -13:
            printf(">");
            break;
        case -14:
            printf(">=");
            break;
        case -15:
            printf("<");
            break;
        case -16:
            printf("<=");
            break;
        case -17:
            printf("==");
            break;
        case -18:
            printf("+");
            break;
        case -19:
            printf("-");
            break;
        case -20:
            printf("*");
            break;
        case -21:
            printf("/");
            break;
        case -22:
            printf("Y");
            break;
        case -23:
            printf("Hd");
            break;
        case -24:
            printf("Tl");
            break;
        case -25:
            break;
        case -26:
            printf("[]");
            break;
        case -27:
            printf("**");
            break;
        case -28:
            printf("MAP");
            break;
        default:
            printf("%u", no->tipo);
    }
    //printf("%d\n", no->tipo);
    if(no->direita) {
        deph++;
        if(no->direita != no) {
            imprime_arvore(no->direita);
            if (lista)
                printf("]");
            else
                printf(")");
        }else{
            printf("Y(");
            imprime_arvore(no->esquerda);
            printf(")");
            return;
        }
    }
    if(deph == 0)
        lista = 0;
}

void alocar_memoria(){
    S = calloc(1, sizeof(struct Celula));
    K = calloc(1, sizeof(struct Celula));
    I = calloc(1, sizeof(struct Celula));
    B = calloc(1, sizeof(struct Celula));
    C = calloc(1, sizeof(struct Celula));
    D = calloc(1, sizeof(struct Celula));
    E = calloc(1, sizeof(struct Celula));
    F = calloc(1, sizeof(struct Celula));
    Y = calloc(1, sizeof(struct Celula));
    PLUS = calloc(1, sizeof(struct Celula));
    SUB = calloc(1, sizeof(struct Celula));
    MULT = calloc(1, sizeof(struct Celula));
    DIV = calloc(1, sizeof(struct Celula));
    POW = calloc(1, sizeof(struct Celula));
    GT = calloc(1, sizeof(struct Celula));
    GET = calloc(1, sizeof(struct Celula));
    LT = calloc(1, sizeof(struct Celula));
    LET = calloc(1, sizeof(struct Celula));
    EQ = calloc(1, sizeof(struct Celula));
    TRUE = calloc(1, sizeof(struct Celula));
    FALSE = calloc(1, sizeof(struct Celula));
    HD = calloc(1, sizeof(struct Celula));
    TL = calloc(1, sizeof(struct Celula));
    MAP = calloc(1, sizeof(struct Celula));
    EMPTY_LIST = calloc(1, sizeof(struct Celula));

    S->tipo = -3;
    K->tipo = -4;
    I->tipo = -5;
    B->tipo = -6;
    C->tipo = -7;
    D->tipo = -8;
    E->tipo = -9;
    F->tipo = -10;
    TRUE->tipo = -11;
    FALSE->tipo = -12;
    GT->tipo = -13;
    GET->tipo = -14;
    LT->tipo = -15;
    LET->tipo = -16;
    EQ->tipo = -17;
    PLUS->tipo = -18;
    SUB->tipo = -19;
    MULT->tipo = -20;
    DIV->tipo = -21;
    Y->tipo = -22;
    HD->tipo = -23;
    TL->tipo = -24;
    EMPTY_LIST->tipo = -26;
    POW->tipo = -27;
    MAP->tipo = -28;

    heap = (struct Celula*) malloc(TAM*sizeof(struct Celula));
    heap2 = (struct Celula*) malloc(TAM*sizeof(struct Celula));
    end_heap = heap+TAM+1;
    hp = heap;
    raiz = NULL;
    celulas = TAM;
}
int converter_char_int(char c){
    switch (c){
        case '@':
            return -2;
        case 'K':
            return -4;
        case 'S':
            return -3;
        case 'I':
            return -5;
        case 'B':
            return -6;
        case 'C':
            return -7;
        case 'D':
            return -8;
        case 'E':
            return -9;
        case 'F':
            return -10;
        case '>':
            return -13;
        case '\v'://Ver caracter para >=
            return -14;
        case '<':
            return -15;
        case '\t'://Ver caracter para <=
            return -16;
        case '=':
            return -17;
        case '+':
            return -18;
        case '-':
            return -19;
        case '*':
            return -20;
        case '/':
            return -21;
        case 'Y':
            return -22;
        case 'H':
            return -23;

        case 'T':
            return -24;
        case '^':
            return -27;
        case 'M':
            return -28;
        case 'X':
            return -20;
        case ':':
            return -25;
        default:
            return c-48;
    }
}

struct Celula* alocar_celula(int tipo){
    if(hp > end_heap){
        printf("sem memoria");
        exit(0);
    }
    struct Celula* alocado = 0;
    switch (tipo) {
        case -4:
            return K;
        case -3:
            return S;
        case -5:
            return I;
        case -6:
            return B;
        case -7:
            return C;
        case -8:
            return D;
        case -9:
            return E;
        case -10:
            return F;
        case -11:
            return TRUE;
        case -12:
            return FALSE;
        case -13:
            return GT;
        case -14:
            return GET;
        case -15:
            return LT;
        case -16:
            return LET;
        case -17:
            return EQ;
        case -18:
            return PLUS;
        case -19:
            return SUB;
        case -20:
            return MULT;
        case -21:
            return DIV;
        case -22:
            return Y;
        case -23:
            return HD;
        case -24:
            return TL;
        case -27:
            return POW;
        case -28:
            return MAP;
        default:
            alocado = hp++;
            alocado->direita = 0;
            alocado->esquerda = 0;
            alocado->tipo = tipo;
            celulas--;
            return alocado;
    }
}


struct Celula* transformar_entrada_grafo(){
    struct Celula *raiz = alocar_celula(converter_char_int(entr[0]));
    entr++;
    while(entr[1]){
        if(entr[0] == '('){
            struct Celula *nova = alocar_celula(-2);
            nova->esquerda = raiz;
            raiz = nova;
            entr++;
            raiz->direita = transformar_entrada_grafo();
        }else if(entr[0] == ')'){
            return raiz;
        }else if(entr[0] >= '0' && entr[0] <= '9') {
            int res = 0;
            while(entr[0] >= '0' && entr[0] <= '9' && entr[1] != '\0'){
                res = res*10 + entr[0]-48;
                entr++;
            }
            struct Celula *nova = alocar_celula(-2);
            nova->direita = alocar_celula(res);
            nova->esquerda = raiz;
            raiz = nova;
            entr--;
        }else{
            struct Celula *nova = alocar_celula(-2);
            nova->esquerda = raiz;
            raiz = nova;
            raiz->direita = alocar_celula(converter_char_int(entr[0]));
        }
        entr++;
    }
    if(entr[0] >= '0' && entr[0] <= '9' && raiz->direita->tipo > 0){
        raiz->direita->tipo = raiz->direita->tipo*10 + entr[0]-48;
        return raiz;
    }else {
        if(entr[0] >= '0' && entr[0] <= '9') {
            int res = entr[0] - 48;
            struct Celula *nova = alocar_celula(-2);
            nova->direita = alocar_celula(res);
            nova->esquerda = raiz;
            raiz = nova;
            entr--;
            return raiz;
        }else if(entr[0] != '\0' && entr[0] != ')'){
            struct Celula *nova = alocar_celula(-2);
            nova->esquerda = raiz;
            nova->direita = alocar_celula(converter_char_int(entr[0]));
            return nova;
        }else{
            return raiz;
        }
    }
}

void push(struct Celula *elem){
    if(p->cell){
        p++;
        p->cell = elem;
    }else{
        p->cell = elem;
    }
}

void buscar_reduz(struct Celula *grafo){
    while(grafo){
        push(grafo);
        grafo = grafo->esquerda;
    }
}

struct Celula* eval(struct Celula *aux);


void reduz_soma(){
    struct Celula *a = (p-1)->cell;
    struct Celula *b = (p-2)->cell;
    struct Pilha *pai = (p-3);
    a->direita = eval(a->direita);
    if(a->direita->tipo == -2)
        return;
    b->direita = eval(b->direita);
    if(b->direita->tipo == -2)
        return;
    a = a->direita;
    b = b->direita;
    p = pai;
    if(pai->cell){
        pai->cell->esquerda = alocar_celula(a->tipo+b->tipo);
    }else{
        p++;
        p->cell = 0;
        raiz = alocar_celula(a->tipo+b->tipo);
        push(raiz);
    }
}

void reduz_subtracao(){
    struct Celula *a = (p-1)->cell;
    struct Celula *b = (p-2)->cell;
    struct Pilha *pai = p-3;
    a->direita = eval(a->direita);
    if(celulas <= 10)
        return;
    b->direita = eval(b->direita);
    if(celulas <= 10)
        return;
    a = a->direita;
    b = b->direita;
    p = pai;
    if(pai->cell){
        pai->cell->esquerda = alocar_celula(a->tipo-b->tipo);
    }else{
        p++;
        p->cell = 0;
        raiz = alocar_celula(a->tipo-b->tipo);
        push(raiz);
    }
}

void reduz_multiplicacao(){
    struct Celula *a = (p-1)->cell;
    struct Celula *b = (p-2)->cell;
    struct Pilha *pai = p-3;
    a->direita = eval(a->direita);
    if(celulas <= 10)
        return;
    b->direita = eval(b->direita);
    if(celulas <= 10)
        return;
    a = a->direita;
    b = b->direita;
    p = pai;
    if(pai->cell){
        pai->cell->esquerda = alocar_celula(a->tipo*b->tipo);
    }else{
        p++;
        p->cell = 0;
        raiz = alocar_celula(a->tipo*b->tipo);
        push(raiz);
    }
}

void reduz_divisao(){
    struct Celula *a = (p-1)->cell;
    struct Celula *b = (p-2)->cell;
    struct Pilha *pai = p-3;
    a->direita = eval(a->direita);
    if(celulas <= 10)
        return;
    b->direita = eval(b->direita);
    if(celulas <= 10)
        return;
    a = a->direita;
    b = b->direita;
    p = pai;
    if(pai->cell){
        pai->cell->esquerda = alocar_celula(a->tipo/b->tipo);
    }else{
        p++;
        p->cell = 0;
        raiz = alocar_celula(a->tipo/b->tipo);
        push(raiz);
    }
}
void reduz_menor_que(){
    struct Celula *a = (p-1)->cell;
    struct Celula *b = (p-2)->cell;
    struct Pilha *pai = p-3;
    struct Celula *res;
    a->direita = eval(a->direita);
    if(a->direita->tipo == -2)
        return;
    b->direita = eval(b->direita);
    if(b->direita->tipo == -2)
        return;
    a = a->direita;

    b = b->direita;
    p = pai;
    if(a->tipo < b->tipo){
        res = alocar_celula(~0-10);
    }else{
        res = alocar_celula(~0-11);
    }
    if(pai->cell){
        pai->cell->esquerda = res;
    }else{
        p++;
        p->cell = 0;
        raiz = res;
        push(raiz);
    }
}

void reduz_maior_que(){
    struct Celula *a = (p-1)->cell;
    struct Celula *b = (p-2)->cell;
    struct Pilha *pai = p-3;
    struct Celula *res;
    a->direita = eval(a->direita);
    if(celulas <= 10)
        return;
    b->direita = eval(b->direita);
    if(celulas <= 10)
        return;
    a = a->direita;
    b = b->direita;
    p = pai;
    if(a->tipo > b->tipo){
        res = alocar_celula(~0-10);
    }else{
        res = alocar_celula(~0-11);
    }
    if(pai->cell){
        pai->cell->esquerda = res;
    }else{
        p++;
        p->cell = 0;
        raiz = res;
        push(raiz);
    }

}

struct Celula* eval(struct Celula *aux){
    struct Pilha *p2 = p++;
    p->cell = 0;
    p++;
    p->cell = 0;
    struct Celula *raiz_aux = raiz;
    raiz = aux;
    buscar_reduz(aux);
    while(raiz->tipo == -2) {
	//imprime_arvore(raiz);
	//printf("\n");
        switch (p->cell->tipo) {
            case -4:
//                reduz_K();
                break;
            case -3:
//                reduz_S();
                break;
            case -5:
//                reduz_I();
                break;
            case -6:
//                reduz_B();
                break;
            case -7:
//                reduz_C();
                break;
            case -8:
//                reduz_D();
                break;
            case -9:
//                reduz_E();
                break;
            case -10:
//                reduz_F();
                break;
            case -11:
//                reduz_TRUE();
                break;
            case -12:
//                reduz_FALSE();
                break;
            case -13:
//                reduz_maior_que();
                break;
            case -14:
//                reduz_maior_igual_que();
                break;
            case -15:
//                reduz_menor_que();
                break;
            case -16:
//                reduz_menor_igual_que();
                break;
            case -17:
//                reduz_igual();
                break;
            case -18:
                reduz_soma();
                break;
            case -19:
                reduz_subtracao();
                break;
            case -20:
                reduz_multiplicacao();
                break;
            case -21:
                reduz_divisao();
                break;
            case -22:
//                reduz_Y();
                break;
            case -23:
//                reduz_Hd();
                break;
            case -24:
//                reduz_Tl();
                break;
            case -27:
//                reduz_potencia();
                break;
            case -28:
//                raiz = reduz_MAP();
                break;
        }
        if(celulas <= 10){
            break;
        }
        if (p->cell)
            buscar_reduz(p->cell->esquerda);
    }
    //imprime_arvore(raiz);
	//printf("\n");
    p = p2;
    aux = raiz;
    raiz = raiz_aux;
    return aux;
}

void execucao() {
    p = redex + 1;
    buscar_reduz(raiz);
    int i = 0;
    while (raiz->tipo == -2) {
        if (celulas <= 10) {
//            cheney();
            p = redex + 1;
            p->cell = 0;
            buscar_reduz(raiz);
        }
        switch (p->cell->tipo) {
            case -4:
//                reduz_K();
                break;
            case -3:
//                reduz_S();
                break;
            case -5:
//                reduz_I();
                break;
            case -6:
//                reduz_B();
                break;
            case -7:
//                reduz_C();
                break;
            case -8:
//                reduz_D();
                break;
            case -9:
//                reduz_E();
                break;
            case -10:
//                reduz_F();
                break;
            case -11:
//                reduz_TRUE();
                break;
            case -12:
//                reduz_FALSE();
                break;
            case -13:
                reduz_maior_que();
                break;
            case -14:
//                reduz_maior_igual_que();
                break;
            case -15:
                reduz_menor_que();
                break;
            case -16:
//                reduz_menor_igual_que();
                break;
            case -17:
//                reduz_igual();
                break;
            case -18:
                reduz_soma();
                break;
            case -19:
                reduz_subtracao();
                break;
            case -20:
                reduz_multiplicacao();
                break;;
            case -21:
                reduz_divisao();
                break;
            case -22:
//                reduz_Y();
                break;
            case -23:
//                reduz_Hd();
                break;
            case -24:
//                reduz_Tl();
                break;
            case -27:
//                reduz_potencia();
                break;
            case -28:
//                raiz = reduz_MAP();
                break;
        }
        if (p->cell)
            buscar_reduz(p->cell->esquerda);
        i++;
    }
}

void limpar_redex(){
    int i;
    for(i = 0; i < TAM/1000;i++){
        redex[i].cell = 0;
    }
}

int iniciar(char* entrada){
	entr = entrada;
	raiz = transformar_entrada_grafo();
	execucao();
    limpar_redex();
	return raiz->tipo;
}
