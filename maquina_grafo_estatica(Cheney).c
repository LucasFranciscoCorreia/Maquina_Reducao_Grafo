#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define TAM 534
#define TAM_STACK 5000
#define TAM_STRING 160000

struct Celula{
    int tipo;
    struct Celula *direita, *esquerda;
};

struct Argumento{
    char *tipo;
    struct Argumento *direita, *esquerda, *proximo;
};

struct Stack{
    struct Argumento *cell;
};

struct Pilha{
    struct Celula *cell;
};

struct Celula heap[TAM];
struct Celula heap2[TAM];
struct Celula *hp;
struct Celula *raiz;
struct Celula *cp;
struct Celula *end_heap;
struct Stack pilha_p[TAM_STACK];
struct Stack *pilha;
struct Stack *end_stack;
struct Pilha redex[TAM];
struct Pilha *p;
int celulas;
int garbage_calls = 0;

void alocar_memoria(){
    hp = heap;
    pilha = pilha_p;
    end_heap = heap+TAM-1;
    end_stack = pilha + (TAM_STACK - 1);
    raiz = NULL;
    celulas = TAM;
}


struct Celula* alocar_celula(int tipo){
    if(hp > end_heap){
        printf("sem memoria");
        exit(0);
    }
    struct Celula *alocado = hp;
    hp++;
    alocado->direita = 0;
    alocado->esquerda = 0;
    alocado->tipo = tipo;
    celulas--;
    return alocado;
}


void arrumar_hp_heap2(){
    end_heap = heap2+TAM-1;
    heap2[TAM-1].direita = 0;
    hp = heap2;
}

void arrumar_hp_heap1(){
    end_heap = heap+TAM-1;
    heap[TAM-1].direita = 0;
    hp = heap;
}

struct Celula* copiar_celulas_cheney(){
    struct Celula* res = alocar_celula(raiz->tipo);
    res->esquerda = raiz->esquerda;
    res->direita = raiz->direita;
    raiz->esquerda = res;
    raiz->direita = 0;
    raiz->tipo = -1;
    struct Celula *aux;
    while(cp < hp) {
        struct Celula *esq = cp->esquerda;
        struct Celula *dir = cp->direita;
        if(esq && esq->tipo == -1) {
            cp->esquerda = esq->esquerda;
        }else if(esq){
            aux = alocar_celula(esq->tipo);
            aux->esquerda = esq->esquerda;
            aux->direita = esq->direita;
            esq->tipo = -1;
            esq->esquerda = aux;
            esq->direita = 0;
            cp->esquerda = aux;
        }
        if(dir && dir->tipo == -1){
            cp->direita = dir->esquerda;
        }else if(dir){
            aux = alocar_celula(dir->tipo);
            aux->esquerda = dir->esquerda;
            aux->direita = dir->direita;
            dir->tipo = -1;
            dir->esquerda = aux;
            dir->direita = 0;
            cp->direita = aux;
        }
        cp++;

    }

    return res;
}

void cheney(){
    static char choose_heap = 1;
    if(choose_heap){
        arrumar_hp_heap2();
    }else{
        arrumar_hp_heap1();
    }
    celulas = TAM;
    choose_heap = !choose_heap;
    cp = hp;
    raiz = copiar_celulas_cheney();
}

void garbage_collection(int type){
    garbage_calls++;
    switch (type){
        case 3:
            cheney();
            break;
    }

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
char entrada[TAM_STRING] = "S(K(SII))(S(S(KS)K)(K(SII)))(S(K(S(S(S(S(K<)I)(K2))I)))(S(S(KS)(S(K(S(K+)))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K2))))))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K1))))))X\0";
//char entrada[TAM_STRING] = "H(T(T(T((:1(:2(:3(:4(:5(:6[]))))))))))\0";
//char entrada[TAM_STRING] = "M(S(K(SII))(S(S(KS)K)(K(SII)))(S(K(S(S(S(S(K<)I)(K2))I)))(S(S(KS)(S(K(S(K+)))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K2))))))(S(S(KS)(S(KK)I))(K(S(S(K-)I)(K1)))))))(:21(:22(:23(:24(:25(:26(:27(:28(:29(:30[]))))))))))\0";
//char lista[TAM_STRING] = "(:0(:1(:2(:3(:4(:5(:6(:7(:8(:9[]))))))))))";
//char lista[TAM_STRING] = "(:1(:2(:3(:4(:5(:6[]))))))\0";
//char lista = "[3*8, 7*(5+2), aaa, (8/4)**(2+1), (8/4)**(2+1), bbb]";
unsigned int X = 26;
int pcont = 0;

void addPilha(struct Argumento *elem){
    if(!pilha->cell) {
        pilha->cell = elem;
    }else {
        struct Stack *aux = pilha;
        while(aux->cell)
            aux++;
        aux->cell = elem;
        if(aux == end_stack){
            printf("stack pequena");
            exit(0);
        }
    }
    pcont++;
}

void removePilha(struct Stack *aux){
    pilha++;
}

char* acha_argumento(char* entrada, int *ini, int *end){
    if(entrada[*end] == '('){
        (*end)++;
        int parenteses = 1;
        while(parenteses){
            if(entrada[*end] == '('){
                parenteses++;
            }else if(entrada[*end] == ')'){
                parenteses--;
            }
            (*end)++;
        }
        (*end)--;
    }
    (*end)++;
    char *cpy;
    if(*end - *ini == 1){
        cpy = calloc(2,sizeof(char));
        cpy[0] = entrada[*ini];
        cpy[1] = '\0';
    }else{
        cpy = entrada+(*ini)+1;
        entrada[*end-1] = '\0';
    }
    *ini = *end;
    return cpy;
}

struct Argumento* transformar_entrada_grafo(char* entrada){
    struct Argumento *grafo = calloc(1, sizeof(struct Argumento));
    int ini = 0;
    int end = 0;
    char* argumento = acha_argumento(entrada, &ini, &end);
    grafo->tipo = argumento;
    while(1){
        struct Argumento *aplicacao = calloc(1, sizeof(struct Argumento));
        char *auxc = calloc(2,sizeof(char));
        auxc[0] = ~0-1;
        auxc[1] = '\0';
        aplicacao->tipo = auxc;
        aplicacao->esquerda = grafo;
        argumento = acha_argumento(entrada, &ini, &end);
        if(argumento[0] == '\0')
            break;
        aplicacao->direita = calloc(1, sizeof(struct Argumento));
        struct Argumento *aux = aplicacao->direita;
        aux->tipo = argumento;
        grafo = aplicacao;
        if(strlen(argumento) > 1){
            addPilha(grafo->direita);
        }
        aux = grafo->esquerda;
        argumento = aux->tipo;
        if(strlen(argumento) > 1){
            addPilha(grafo->esquerda);
        }
    }
    return grafo;
}
struct Argumento *res;

void resolver_argumentos(){
    while(pilha->cell){
        struct Stack *aux = pilha;
        char *nova_entrada = aux->cell->tipo;
        if(*nova_entrada != ':') {
            struct Argumento *dir = transformar_entrada_grafo(nova_entrada);
            (*aux).cell->tipo = (*dir).tipo;
            (*aux).cell->esquerda = (*dir).esquerda;
            (*aux).cell->direita = (*dir).direita;
        }
        removePilha(aux);
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

struct Celula* converter_para_celula(struct Argumento *res){
    char *tipo = res->tipo;
    struct Celula *grafo;
    switch (*tipo){
        case ~0-1:
            grafo = alocar_celula(~0-1);
            break;
        case 'K':
            grafo = alocar_celula(~0-3);
            break;
        case 'S':
            grafo = alocar_celula(~0-2);
            break;
        case 'I':
            grafo = alocar_celula(~0-4);
            break;
        case 'B':
            grafo = alocar_celula(~0-5);
            break;
        case 'C':
            grafo = alocar_celula(~0-6);
            break;
        case 'D':
            grafo = alocar_celula(~0-7);
            break;
        case 'E':
            grafo = alocar_celula(~0-8);
            break;
        case 'F':
            grafo = alocar_celula(~0-9);
            break;
        case '>':
            grafo = alocar_celula(~0-12);
            break;
        case '\v'://Ver caracter para >=
            grafo = alocar_celula(~0-13);
            break;
        case '<':
            grafo = alocar_celula(~0-14);
            break;
        case '\t'://Ver caracter para <=
            grafo = alocar_celula(~0-15);
            break;
        case '=':
            grafo = alocar_celula(~0-16);
            break;
        case '+':
            grafo = alocar_celula(~0-17);
            break;
        case '-':
            grafo = alocar_celula(~0-18);
            break;
        case '*':
            grafo = alocar_celula(~0-19);
            break;
        case '/':
            grafo = alocar_celula(~0-20);
            break;
        case 'Y':
            grafo = alocar_celula(~0-21);
            break;
        case 'H':
            grafo = alocar_celula(~0-22);
            break;
        case 'T':
            grafo = alocar_celula(~0-23);
            break;
        case 'M':
            grafo = alocar_celula(-28);
            break;
        case 'X':
            grafo = alocar_celula(X);
            break;
        case ':':
            grafo = transformar_lista_grafo(tipo);
            break;
        default:
            grafo = alocar_celula(*tipo-48);
    }
    if(res->esquerda)
        grafo->esquerda = converter_para_celula(res->esquerda);
    if(res->direita)
        grafo->direita = converter_para_celula(res->direita);
    return grafo;
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
        imprime_arvore(no->direita);
        if(lista)
            printf("]");
        else
            printf(")");
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
    struct Pilha *aux1 = p-1;
    struct Pilha *aux2 = p-2;
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
    if(pai->cell){
        pai->cell->esquerda = a;
        pai->cell->direita = pai->cell;
    }else{
        p++;
        p->cell = 0;
        raiz->esquerda = a;
        raiz->direita = raiz;
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
            garbage_collection(3);
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
            garbage_collection(3);
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
    alocar_memoria();
    res = transformar_entrada_grafo(entrada);
    resolver_argumentos();
    raiz = converter_para_celula(res);
    double tempo = CLOCKS_PER_SEC;
    printf("%lf segundos para realizacao do grafo\n", clock()/tempo);
}

int main(void)
{
    compilacao();
    execucao();
    imprime_arvore(raiz);
}

