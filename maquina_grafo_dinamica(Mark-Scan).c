#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TAM 230000000

struct Celula{
    int tipo;
    struct Celula *direita, *esquerda;
    char garbage;
};

struct Pilha{
    struct Celula *cell;
};

struct Celula *heap;
struct Celula *free_list;
struct Celula *raiz;
struct Celula *S, *K, *I, *B, *C, *D, *E, *F, *TRUE, *FALSE, *GT, *GET, *LT, *LET, *EQ, *PLUS, *SUB, *MULT, *DIV, *Y, *HD, *TL, *MAP, *POW, *EMPTY_LIST;
struct Pilha redex[TAM];
struct Pilha *p;
int celulas;
int garbage_calls = 0;


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

    heap = malloc(TAM*sizeof(struct Celula));
    int i;
    for(i = 0; i < TAM-1;i++) {
        (heap + i)->direita = (heap + i + 1);
    }
    (heap+i)->direita = 0;

    free_list = heap;
    raiz = NULL;
    celulas = TAM;
}


struct Celula* alocar_celula(int tipo){
    if(!free_list){
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
            alocado = free_list;
            free_list = free_list->direita;
            alocado->direita = 0;
            alocado->esquerda = 0;
            alocado->tipo = tipo;
            celulas--;
            return alocado;
    }
}

void mark(struct Celula* no){
    if(!no->garbage) {
        no->garbage = 1;
        celulas--;
        if (no->esquerda)
            mark(no->esquerda);
        if (no->direita)
            mark(no->direita);
    }
}

void scan(){
    int i;
    struct Celula *end_heap;
    for(i = 0; i < TAM;i++){
        if(!heap[i].garbage){
            free_list = heap+i;
            end_heap = free_list;
            end_heap->tipo = 0;
            end_heap->direita = 0;
            end_heap->esquerda = 0;
            i++;
            break;
        }
        heap[i].garbage = 0;
    }
    for(; i < TAM;i++){
        if(!heap[i].garbage) {
            end_heap->direita = heap + i;
            end_heap = end_heap->direita;
            end_heap->tipo = 0;
            end_heap->direita = 0;
            end_heap->esquerda = 0;
        }
        heap[i].garbage = 0;
    }
}
void mark_scan(){
    celulas = TAM;
    mark(raiz);
    //printf("%d\n%d\n", cont, celulas);
    //cont = 0;
    scan();
    garbage_calls++;
    if(celulas <= 10){
        printf("Memoria Insuficiente");
        exit(0);
    }
}



//char entrada[TAM_STRING] = "S(SKK)(SKK)(SKK)S\0";
//char entrada[TAM_STRING] = "S(K(K))(KS)(SK)KK(SK)K\0";
//char entrada[TAM_STRING] = "D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)((D(FBI(SII))I(B(FBI(SII))I)(KK))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K)\0";
//char entrada[TAM_STRING] = "S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))((S(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(S(S(KS)(S(KK)(SKK)))(K(S(SKK)(SKK))))(KK))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K)\0";
//static char entrada[TAM_STRING] = "S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))((S(C(BBI)(SII))(C(BBI)(SII))(KK))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K))K)\0";
/*
    Fab x => if x = 0 then 1 else x+fab(x-1)
*/
//Fab1
//char entrada[TAM_STRING] = "S(K(SII))(S(S(KS)K)(K(SII)))(S(K(S(S(S(S(K=)I)(K0))(K1))))(S(K(S(S(K+)I)))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K1))))))X\0";
//Fab2
//char entrada[TAM_STRING] = "S(C(F=I0)1)(D+I(B(Y(ES(FI(F=I0)1)(E(D+)I(FBI(F-I1)))))(F-I1)))X\0";

/*
    Fib x => if x < 2 than 2, else Fib(x-2) + Fib(x-1)
*/
//Fib1
// char entrada[TAM_STRING] = "S(K(SII))(S(S(KS)K)(K(SII)))(S(K(S(S(S(KI)(S(S(K<)I)(K2)))I)))(S(S(KS)(S(K(S(K+)))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K2))))))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K1))))))X\0";
//Fib2
char entrada[TAM] = "S(K(SII))(S(S(KS)K)(K(SII)))(S(K(S(S(S(S(K<)I)(K2))I)))(S(S(KS)(S(K(S(K+)))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K2))))))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K1))))))25\0";
//char entrada[TAM_STRING] = "H(T(T(T((:1(:2(:3(:4(:5(:6[]))))))))))\0";
//char entrada[TAM_STRING] = "M(S(K(SII))(S(S(KS)K)(K(SII)))(S(K(S(S(S(S(K<)I)(K2))I)))(S(S(KS)(S(K(S(K+)))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K2))))))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K1)))))))(:21(:22(:23(:24(:25(:26(:27(:28(:29(:30[]))))))))))\0";
//char lista[TAM_STRING] = "(:0(:1(:2(:3(:4(:5(:6(:7(:8(:9[]))))))))))";
//char lista[TAM_STRING] = "(:1(:2(:3(:4(:5(:6[]))))))\0";
//char lista = "[3*8, 7*(5+2), aaa, (8/4)**(2+1), (8/4)**(2+1), bbb]";
//char entrada[TAM_STRING] = "S(S(KS)(S(S(KS)(S(KK)(K+)))(KI)))(S(KK)I)76\0";
//char entrada[TAM_STRING] = "B(CI)I9I\0";
//char entrada[TAM] = "Y(ES(S(F<I2)I)(D(D+)(FBI(F-I2))(FBI(F-I1))))30\0";
//char entrada[TAM_STRING] = "ES(S(F<I2)I)(D(D+)(FBI(F-I2))(FBI(F-I1)))(Y(ES(S(F<I2)I)(D(D+)(FBI(F-I2))(FBI(F-I1)))))X\0";
//char entrada[TAM_STRING] = "(S((S((BC)((C((BB)I))I)))((BC)((C((BB)I))I))))IIK";
char *entr;

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
            return -1;
    }
}


struct Celula* transformar_lista_grafo(char *lista){
    struct Celula* raiz = alocar_celula(-25);
    lista++;
    int res = 0;
    do{
        res = res*10 + ((*lista)-48);
        lista++;
    }while(*lista != '(' && *lista != '[');
    raiz->esquerda = alocar_celula(res);
    if(*lista == '('){
        raiz->direita = transformar_lista_grafo(lista+1);
    }else{
        raiz->direita = alocar_celula(-26);
    }
    return raiz;
}

struct Celula* transforma_entrada_grafo(){
    struct Celula *raiz = alocar_celula(converter_char_int(entr[0]));
    entr++;
    while(entr[1]){
        if(entr[0] == '('){
            struct Celula *nova = alocar_celula(-2);
            nova->esquerda = raiz;
            raiz = nova;
            entr++;
            raiz->direita = transforma_entrada_grafo();
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
        }else {
            struct Celula *nova = alocar_celula(-2);
            nova->esquerda = raiz;
            nova->direita = alocar_celula(converter_char_int(entr[0]));
            return nova;
        }
    }
}
/*
 * ~0-26    => '**'
 * ~0-25    =>'[]'
 * ~0-24    =>':'
 * ~0-23    =>'Tl'
 * ~0-22    =>'Hd'
 * ~0-21    =>'Y'
 * ~0-20    =>'/'
 * ~0-19    =>'*'
 * ~0-18    =>'-'
 * ~0-17    =>'+'
 * ~O-16    =>'=='
 * ~0-15    =>'<='
 * ~0-14    =>'<'
 * ~0-13    =>'>='
 * ~0-12    =>'>'
 * ~0-11    =>'FALSE'
 * ~0-10    =>'TRUE'
 * ~0-9     =>'F'
 * ~0-8     =>'E'
 * ~0-7     =>'D'
 * ~0-6     =>'F
 * ~0-5     =>'E
 * ~0-4     =>'I'
 * ~0~3     =>'K'
 * ~0-2     =>'D'
 * ~0-1     =>'@'
 * ~0       =>'\0'
 */
//lista= ":[*3 8]:[*7(+5 2)](:[10](:[^(/8 4)(+2 1)](:[^(/8 4)(+2 1)](:[20][]))))\0";
//lista = ":1(:2(:3(:4(:5(:6(:7(:8(:9(:X[])))))))))";
//int x_lista = 10;

//lista= ":[*3 8]:[*7(+5 2)](:[10](:[^(/8 4)(+2 1)](:[^(/8 4)(+2 1)](:[20][]))))\0";
struct Celula* pegar_subgrafo(int tipo){
    struct Celula *app1 = alocar_celula(-2);
    struct Celula *app2 = alocar_celula(-2);
    struct Celula *app3 = alocar_celula(-2);
    struct Celula *app4 = alocar_celula(-2);
    struct Celula *app5 = alocar_celula(-2);
    struct Celula *app6 = alocar_celula(-2);
    struct Celula *app7 = alocar_celula(-2);
    switch (tipo){
        case 1:
            app2->esquerda = alocar_celula(-20);
            app2->direita = alocar_celula(3);
            app1->esquerda = app2;
            app1->direita = alocar_celula(8);
            break;
        case 2:
            app2 = alocar_celula(-2);
            app2->esquerda = alocar_celula(-20);
            app2->direita = alocar_celula(7);
            app3->esquerda = alocar_celula(-18);
            app3->direita = alocar_celula(5);
            app4->esquerda = app3;
            app4->direita = alocar_celula(2);
            app1->esquerda = app2;
            app1->direita = app4;
            break;
        case 3:
            app1 = alocar_celula(tipo);
            break;
        case 4:
            app2->esquerda = alocar_celula(-27);
            app4->esquerda = alocar_celula(-21);
            app3->esquerda = app4;
            app2->direita = app3;
            app1->esquerda = app2;
            app6->esquerda = app5;
            app5->esquerda = alocar_celula(-18);
            app1->direita = app6;

            app4->direita = alocar_celula(8);
            app3->direita = alocar_celula(4);

            app5->direita = alocar_celula(2);
            app6->direita = alocar_celula(1);

            break;
        case 6:
            app1= alocar_celula(tipo);
            break;
    }
    return app1;
}

/*
 * ~0-27    =>'MAP'
 * ~0-26    =>'**'
 * ~0-25    =>'[]'
 * ~0-24    =>':'
 * ~0-23    =>'Tl'
 * ~0-22    =>'Hd'
 * ~0-21    =>'Y'
 * ~0-20    =>'/'
 * ~0-19    =>'*'
 * ~0-18    =>'-'
 * ~0-17    =>'+'
 * ~O-16    =>'=='
 * ~0-15    =>'<='
 * ~0-14    =>'<'
 * ~0-13    =>'>='
 * ~0-12    =>'>'
 * ~0-11    =>'FALSE'
 * ~0-10    =>'TRUE'
 * ~0-9     =>'F'
 * ~0-8     =>'E'
 * ~0-7     =>'D'
 * ~0-6     =>'C
 * ~0-5     =>'B
 * ~0-4     =>'I'
 * ~0~3     =>'K'
 * ~0-2     =>'S'
 * ~0-1     =>'@'
 * ~0       =>'\0'
 */

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

//Kab = a
void reduz_K(){
    struct Celula *a = (--p)->cell->direita;
    p = p-2;
    struct Pilha *pai = p;
    if(pai->cell)
        pai->cell->esquerda = a;
    else {
        p++;
        p->cell = 0;
        raiz = a;
        push(raiz);
    }
}

//Sabc = ac(bc)
void reduz_S(){
    struct Celula *a = (--p)->cell->direita;
    struct Celula *b = (--p)->cell->direita;
    struct Celula *c = (--p)->cell->direita;
    struct Pilha *pai = --p;
    struct Celula *app1, *app2, *app3;
    app1 = alocar_celula(~0-1);
    app2 = alocar_celula(~0-1);
    app3 = alocar_celula(~0-1);
    app1->esquerda = app2;
    app2->esquerda = a;
    app3->esquerda = b;
    app1->direita = app3;
    app2->direita = app3->direita = c;
    if(pai->cell){
        pai->cell->esquerda = app1;
    }else{
        p++;
        p->cell = 0;
        raiz = app1;
        push(raiz);
    }
}

//Ia = a
void reduz_I(){
    struct Celula *a = (--p)->cell->direita;
    struct Pilha *pai = --p;
    if(pai->cell){
        p->cell->esquerda = a;
    }else{
        p++;
        p->cell = 0;
        raiz = a;
        push(raiz);
    }
}

//Babc = a(bc)
void reduz_B(){
    struct Celula *a = (--p)->cell->direita;
    struct Celula *b = (--p)->cell->direita;
    struct Celula *c = (--p)->cell->direita;
    struct Pilha *pai = --p;
    struct Celula *app1 = alocar_celula(~0-1);
    struct Celula *app2 = alocar_celula(~0-1);
    app1->esquerda = a;
    app1->direita = app2;
    app2->esquerda = b;
    app2->direita = c;
    if(pai->cell){
        pai->cell->esquerda = app1;
    }else{
        p++;
        p->cell = 0;
        raiz = app1;
        push(raiz);
    }
}

//Cabc = acb
void reduz_C(){
    struct Celula *a = (--p)->cell->direita;
    struct Celula *b = (--p)->cell->direita;
    struct Celula *c = (--p)->cell->direita;
    struct Pilha *pai = --p;
    struct Celula *app1 = alocar_celula(~0-1);
    struct Celula *app2 = alocar_celula(~0-1);
    app1->esquerda = a;
    app1->direita = c;
    app2->esquerda = app1;
    app2->direita = b;
    if(pai->cell){
        pai->cell->esquerda = app2;
    }else{
        p++;
        p->cell = 0;
        raiz = app2;
        push(raiz);
    }
}

//Dabcd = a(bd)(cd)
void reduz_D(){
    struct Celula *a = (--p)->cell->direita;
    struct Celula *b = (--p)->cell->direita;
    struct Celula *c = (--p)->cell->direita;
    struct Celula *d = (--p)->cell->direita;
    struct Pilha *pai = (--p);
    struct Celula *app1 = alocar_celula(~0-1);
    struct Celula *app2 = alocar_celula(~0-1);
    struct Celula *app3 = alocar_celula(~0-1);
    struct Celula *app4 = alocar_celula(~0-1);
    app1->esquerda = a;
    app1->direita = app2;
    app2->esquerda = b;
    app2->direita = d;
    app3->esquerda = app1;
    app3->direita = app4;
    app4->esquerda = c;
    app4->direita = d;
    if(pai->cell){
        pai->cell->esquerda = app3;
    }else{
        p++;
        p->cell = 0;
        raiz = app3;
        push(raiz);
    }
}

//Eabcd = ab(cd)
void reduz_E(){
    struct Celula *a = (--p)->cell->direita;
    struct Celula *b = (--p)->cell->direita;
    struct Celula *c = (--p)->cell->direita;
    struct Celula *d = (--p)->cell->direita;
    struct Pilha *pai = (--p);
    struct Celula *app1 = alocar_celula(~0-1);
    struct Celula *app2 = alocar_celula(~0-1);
    struct Celula *app3 = alocar_celula(~0-1);
    app1->esquerda = a;
    app1->direita = b;
    app2->esquerda = app1;
    app2->direita = app3;
    app3->esquerda = c;
    app3->direita = d;
    if(pai->cell){
        pai->cell->esquerda = app2;
    }else{
        p++;
        p->cell = 0;
        raiz = app2;
        push(raiz);
    }
}
//Fabcd = a(bd)c
void reduz_F(){
    struct Celula *a = (--p)->cell->direita;
    struct Celula *b = (--p)->cell->direita;
    struct Celula *c = (--p)->cell->direita;
    struct Celula *d = (--p)->cell->direita;
    struct Pilha *pai = (--p);
    struct Celula *app1 = alocar_celula(~0-1);
    struct Celula *app2 = alocar_celula(~0-1);
    struct Celula *app3 = alocar_celula(~0-1);
    app1->esquerda = a;
    app1->direita = app2;
    app2->esquerda = b;
    app2->direita = d;
    app3->esquerda = app1;
    app3->direita = c;

    if(pai->cell){
        pai->cell->esquerda = app3;
    }else{
        p++;
        p->cell = 0;
        raiz = app3;
        push(raiz);
    }
}

//Tab = a
void reduz_TRUE(){
    struct Celula *a = (--p)->cell->direita;
    --p;
    struct Pilha *pai = --p;
    if(pai->cell)
        pai->cell->esquerda = a;
    else {
        p++;
        p->cell = 0;
        raiz = a;
        push(raiz);
    }
}

//Fab = b;
void reduz_FALSE(){
    --p;
    struct Celula *b = (--p)->cell->direita;
    struct Pilha *pai = --p;
    if(pai->cell){
        pai->cell->esquerda = b;
    }else{
        p++;
        p->cell = 0;
        raiz = b;
        push(raiz);
    }
}
struct Celula* eval(struct Celula *aux);
//>ab ? T : F
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

//>=ab ? T : F
void reduz_maior_igual_que(){
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
    if(a->tipo >= b->tipo){
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
//<ab ? T : F
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

//<=ab ? T : F
void reduz_menor_igual_que(){
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
    if(a->tipo <= b->tipo){
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

void reduz_igual(){
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
    if(a->tipo == b->tipo){
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

void reduz_potencia(){
    struct Celula* a = (p-1)->cell;
    struct Celula* b = (p-2)->cell;
    struct Pilha* pai = p-3;
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
        pai->cell->esquerda = alocar_celula(pow(a->tipo, b->tipo));
    }else{
        p++;
        p->cell = 0;
        raiz = alocar_celula(pow(a->tipo,b->tipo));
        push(raiz);
    }
}

void reduz_Y(){
    struct Celula *a = (--p)->cell->direita;
    struct Pilha *pai = (--p);
    struct Celula *app = alocar_celula(-2);
    app->esquerda = a;
    app->direita = app;
    if(pai->cell){
        pai->cell->esquerda = app;
    }else{
        p++;
        p->cell = 0;
        raiz->esquerda = app;
        push(raiz);
    }
}

void reduz_Hd() {
    struct Celula *a = (p-1)->cell->direita;
    a = eval(a);
    p = p-2;
    raiz =  a->esquerda;
    push(raiz);
}

void reduz_Tl(){
    struct Celula *a = (p-1)->cell->direita;
    a = eval(a);
    p = p-2;
    raiz = a->direita;
    push(raiz);
}

struct Celula* reduz_MAP(){
    static int turn = 0;
    struct Celula* a = (--p)->cell;
    struct Celula* b = (--p)->cell->direita;
    struct Pilha* pai = (--p);
    struct Celula* res = alocar_celula(-25);
    if(celulas <= 10){
        turn = 1;
        if(pai->cell)
            return pai->cell;
        else
            return raiz;
    }
    if(turn){
        printf("Memoria Insuficiente");
        exit(0);
    }
    struct Celula* aux = res;
    while(b->direita->tipo != -26) {
        struct Celula *head = b->esquerda;
        b = b->direita;
        struct Celula *app = alocar_celula(-2);
        app->esquerda = a->direita;
        app->direita = head;
        aux->esquerda = app;
        aux = aux->direita = alocar_celula(-25);
    }
    aux->tipo = -26;
    raiz = res;
    aux = res;
    struct Pilha* p2 = p;
    while(aux->tipo != -26 && aux->esquerda->tipo == -2){
        if(celulas <= 10){
            mark_scan();
            p = p2;
            p->cell = 0;
            aux = aux->esquerda;
            res = res->esquerda;
        }
        aux->esquerda = eval(aux->esquerda);
        if(aux->esquerda->tipo != -2){
            aux = aux->direita;
        }
    }
    return res;
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
        switch (p->cell->tipo) {
            case -4:
                reduz_K();
                break;
            case -3:
                reduz_S();
                break;
            case -5:
                reduz_I();
                break;
            case -6:
                reduz_B();
                break;
            case -7:
                reduz_C();
                break;
            case -8:
                reduz_D();
                break;
            case -9:
                reduz_E();
                break;
            case -10:
                reduz_F();
                break;
            case -11:
                reduz_TRUE();
                break;
            case -12:
                reduz_FALSE();
                break;
            case -13:
                reduz_maior_que();
                break;
            case -14:
                reduz_maior_igual_que();
                break;
            case -15:
                reduz_menor_que();
                break;
            case -16:
                reduz_menor_igual_que();
                break;
            case -17:
                reduz_igual();
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
                reduz_Y();
                break;
            case -23:
                reduz_Hd();
                break;
            case -24:
                reduz_Tl();
                break;
            case -27:
                reduz_potencia();
                break;
            case -28:
                raiz = reduz_MAP();
                break;
        }
        if(celulas <= 10){
            break;
        }
        if (p->cell)
            buscar_reduz(p->cell->esquerda);
    }
    p = p2;
    aux = raiz;
    raiz = raiz_aux;
    return aux;
}
/*
 *
 * ~0-25    =>'[]'
 * ~0-24    =>':'
 * ~0-23    =>'Tl'
 * ~0-22    =>'Hd'
 * ~0-21    =>'Y'
 * ~O-20    =>'/'
 * ~0-19    =>'*'
 * ~0-18    =>'-'
 * ~0-17    =>'+'
 * ~O-16    =>'=='
 * ~0-15    =>'<='
 * ~0-14    =>'<'
 * ~0-13    =>'>='
 * ~0-12    =>'>'
 * ~0-11    =>'FALSE'
 * ~0-10    =>'TRUE'
 * ~0-9     =>'F'
 * ~0-8     =>'E'
 * ~0-7     =>'D'
 * ~0-6     =>'C'
 * ~0-5     =>'B'
 * ~0-4     =>'I'
 * ~0~3     =>'K'
 * ~0-2     =>'S'
 * ~0-1     =>'@'
 * ~0       =>'\0'
 */
void execucao(){
    double tempo = CLOCKS_PER_SEC;
    long long time = clock();
    p = redex+1;
    buscar_reduz(raiz);
    while(raiz->tipo == -2){
        if(celulas <= 10){
            mark_scan();
            p = redex+1;
            p->cell = 0;
            buscar_reduz(raiz);
        }
        switch (p->cell->tipo){
            case -4:
                reduz_K();
                break;
            case -3:
                reduz_S();
                break;
            case -5:
                reduz_I();
                break;
            case -6:
                reduz_B();
                break;
            case -7:
                reduz_C();
                break;
            case -8:
                reduz_D();
                break;
            case -9:
                reduz_E();
                break;
            case -10:
                reduz_F();
                break;
            case -11:
                reduz_TRUE();
                break;
            case -12:
                reduz_FALSE();
                break;
            case -13:
                reduz_maior_que();
                break;
            case -14:
                reduz_maior_igual_que();
                break;
            case -15:
                reduz_menor_que();
                break;
            case -16:
                reduz_menor_igual_que();
                break;
            case -17:
                reduz_igual();
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
                reduz_Y();
                break;
            case -23:
                reduz_Hd();
                break;
            case -24:
                reduz_Tl();
                break;
            case -27:
                reduz_potencia();
                break;
            case -28:
                raiz = reduz_MAP();
                break;
        }
        if(p->cell)
            buscar_reduz(p->cell->esquerda);


    }
    printf("\n%lf segundos de reducao de grafo\n",(clock()-time)/tempo);
    printf("%lf segundos de execucao\n", clock()/tempo);
    printf("Chamadas de Garbage Collection: %d\n", garbage_calls);
}

void compilacao(){
    entr = entrada;
    alocar_memoria();
    raiz = transforma_entrada_grafo();
    double tempo = CLOCKS_PER_SEC;
    printf("%lf segundos para realizacao do grafo\n", clock()/tempo);
}

int main(void)
{
    compilacao();
    execucao();
    imprime_arvore(raiz);
}
