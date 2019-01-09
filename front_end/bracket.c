#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define T 100000

void casa_parenteses(char *teste, int *end) {
    int parem = 1;
    (*end)++;
    while(parem){
        char c = teste[(*end)++];
        if(c == '('){
            parem++;
        }else if(c == ')'){
            parem--;
        }
    }
    (*end)--;
}

void acha_argumento(char *teste, int a, int *p) {
    if(teste[a] == '\0'){
        *p = -1;
        return;
    }
    *p = a;
    if(teste[a] == '('){
        casa_parenteses(teste, p);
    }else{
        *p = a;
    }
}

void associatividade_esquerda(char *teste, int *beg) {
    int fim = 0;
    while(teste[++fim] != '\0');
    int ini = 0;
    while(teste[ini] == '['){
        while(teste[ini] != ']'){
            ini++;
        }
        ini++;
    }

    int a, nA, b, nB, c, nC;
    nC = ini;

    a = ini;
    acha_argumento(teste, a, &nA);
    if(a != nA){
        char aux[T];
        char save[T];
        char *vis = save;
        int i,j = 0;
        for(i =a+1; i < nA;i++){
            aux[j++] = teste[i];
        }
        aux[j] = '\0';
        for(i = 0; teste[i] != '\0';i++){
            save[i] = teste[i];
        }
        save[i] = '\0';
        int aux1 = 0;
        associatividade_esquerda(aux, &aux1);
        j = a;
        teste[j++] ='(';
        for(i = 0;aux[i]!= '\0';i++){
            teste[j++] = aux[i];
        }
        teste[j++] = ')';
        for(i = nA+1; save[i] != '\0';i++){
            teste[j++] = save[i];
        }
        acha_argumento(teste, a, &nA);
        fim = j;
    }
    b = nA+1;
    acha_argumento(teste, b, &nB);
    c = nB+1;
    acha_argumento(teste, c, &nC);
    while(nB != -1 && nC != -1){
        fim+=2;
        teste[fim] = '\0';
        int i = fim-3, end = fim-1;
        for(;i >= c;i--){
            teste[end--] = teste[i];
        }
        i++;
        teste[end--] = ')';
        for(;i > a;i--){
            teste[end--] = teste[i-1];
        }
        teste[i] = '(';
        a = ini;
        acha_argumento(teste, a, &nA);
        b = nA+1;
        acha_argumento(teste, b, &nB);
        c = nB+1;
        acha_argumento(teste, c, &nC);
    }
    *beg = ini;
}

char contains(char *teste, char i, int i1, int i2) {
    char res = 0;
    for(;i1 <= i2;i1++){
        if(teste[i1] == i)
            res = 1;
    }
    return res;
}
void logica_combinatorial(char *teste);

void avaliar(char* str){
    int fim = 0;
    while(str[++fim] != '\0');
    if(fim == 4){
        if(str[3] == str[1]){
            str[0] = 'I';
            str[1] = '\0';
        }else {
            char res = str[1];
            str[0] = 'K';
            str[1] = res;
            str[2] = '\0';
        }
    }else{
        int i = 0, ini;
        while(str[i] != '\0'){
            if(str[i] == ']'){
                ini = i+1;
            }
            i++;
        }
        if(str[ini] == '('){
            int j = ini;
            for(i = ini;str[i+1] != '\0';i++){
                str[j++] = str[i+1];
            }
            str[i-1] = '\0';
        }
        logica_combinatorial(str);
    }

}

char tem(char *str, char busca){
    int i = 0;
    char res = 0;
    for(;str[i] != '\0';i++){
        if(str[i] == busca)
            res = 1;
    }
    return res;
}

void bracket_C(char *teste, int ini, int a, int nA, int c, int nC) {
    char new_a[T];
    char old_c[T];
    new_a[0] = '[';
    new_a[1] = teste[ini - 2];
    new_a[2] = ']';
    int j = 3, i;
    for (i = a; i <= nA; i++) {
        new_a[j++] = teste[i];
    }
    new_a[j] = '\0';
    avaliar(new_a);
    j = 0;
    for (i = c; i <= nC; i++) {
        old_c[j++] = teste[i];
    }
    old_c[j] = '\0';
    ini -= 3;
    j = 0;
    teste[ini++] = 'C';
    teste[ini++] = '(';
    for (i = ini; new_a[j] != '\0'; i++) {
        teste[i] = new_a[j++];
    }
    teste[i++] = ')';
    j = 0;
    for (; old_c[j] != '\0'; i++) {
        teste[i] = old_c[j++];
    }
    teste[i] = '\0';
}

void bracket_S(char *teste, int ini, int a, int nA, int c, int nC) {
    char new_a[T];
    char new_c[T];
    new_a[0] = new_c[0] = teste[ini - 3];
    new_a[1] = new_c[1] = teste[ini - 2];
    new_a[2] = new_c[2] = teste[ini - 1];

    int j = 3;
    int i;
    for (i = a; i <= nA; i++) {
        new_a[j++] = teste[i];
    }
    new_a[j] = '\0';
    j = 3;
    for (i = c; i <= nC; i++) {
        new_c[j++] = teste[i];
    }
    new_c[j] = '\0';
    avaliar(new_a);
    avaliar(new_c);
    ini-=3;
    teste[ini++] = 'S';
    teste[ini++] = '(';
    j = 0;
    for(i = ini;new_a[j] != '\0';i++ ){
        teste[i] = new_a[j++];
    }
    teste[i++] = ')';
    teste[i++] = '(';
    j = 0;
    for(;new_c[j] != '\0';i++){
        teste[i] = new_c[j++];
    }
    teste[i++] = ')';
    teste[i] = '\0';
}

void bracket_D(char *teste, int ini, int a, int nA, int b, int nB, int c, int nC) {
    char old_a[T];
    char new_b[T];
    char new_c[T];
    new_b[0] = new_c[0] = '[';
    new_b[1] = new_c[1] = teste[ini-2];
    new_b[2] = new_c[2] = ']';
    int i, j = 0;
    for(i = a; i <= nA;i++){
        old_a[j++]=teste[i];
    }
    old_a[j] = '\0';
    j = 3;
    for(i = b;i <=nB;i++){
        new_b[j++] = teste[i];
    }
    new_b[j] = '\0';
    j = 3;
    for(i = c;i <=nC;i++){
        new_c[j++] = teste[i];
    }
    new_c[j] = '\0';
    avaliar(new_b);
    avaliar(new_c);
    ini-=3;
    j = ini;
    teste[j++] = 'D';
    for(i = 0;old_a[i] != '\0';i++){
        teste[j++] = old_a[i];
    }
    teste[j++] = '(';
    for(i = 0; new_b[i] != '\0';i++){
        teste[j++] = new_b[i];
    }
    teste[j++] = ')';
    teste[j++] = '(';
    for(i = 0; new_c[i] != '\0';i++){
        teste[j++] = new_c[i];
    }
    teste[j++] = ')';
    teste[j] = '\0';

}

void bracket_B(char *teste, int ini, int a, int nA, int c, int nC) {
    char new_c[T];
    char old_a[T];
    new_c[0] = '[';
    new_c[1] = teste[ini - 2];
    new_c[2] = ']';
    int j = 3, i;
    for (i = c; i <= nC; i++) {
        new_c[j++] = teste[i];
    }
    new_c[j] = '\0';
    avaliar(new_c);
    j = 0;
    for (i = a; i <= nA; i++) {
        old_a[j++] = teste[i];
    }
    old_a[j++] = '\0';
    ini -= 3;
    j = 0;
    teste[ini++] = 'B';
    for (; old_a[j] != '\0'; ini++) {
        teste[ini] = old_a[j++];
    }
    j = 0;
    teste[ini++] = '(';
    for (i = ini; new_c[j] != '\0'; i++) {
        teste[i] = new_c[j++];
    }
    teste[i++] = ')';
    teste[i++] = '\0';
}

void bracket_F(char *teste, int ini, int a, int nA, int b, int nB, int c, int nC) {
    char old_a[T];
    char old_c[T];
    char new_b[T];
    new_b[0] = '[';
    new_b[1] = teste[ini-2];
    new_b[2] = ']';
    int i, j = 0;
    for(i = a; i <=nA;i++){
        old_a[j++] = teste[i];
    }
    old_a[j] = '\0';
    j = 3;
    for(i = b;i <= nB;i++){
        new_b[j++] = teste[i];
    }
    new_b[j] = '\0';
    j = 0;
    for(i = c;i <=nC;i++){
        old_c[j++] = teste[i];
    }
    old_c[j] = '\0';
    avaliar(new_b);
    ini-=3;
    j = ini;
    teste[j++] = 'F';
    for(i = 0; old_a[i] != '\0';i++){
        teste[j++] = old_a[i];
    }
    teste[j++] = '(';
    for(i = 0; new_b[i] != '\0';i++){
        teste[j++] = new_b[i];
    }
    teste[j++] = ')';
    for(i = 0; old_c[i] != '\0';i++){
        teste[j++] = old_c[i];
    }
    teste[j] = '\0';
}

void bracket_E(char *teste, int ini, int a, int nA, int b, int nB, int c, int nC) {
    char old_a[T];
    char old_b[T];
    char new_c[T];
    new_c[0] = '[';
    new_c[1] = teste[ini-2];
    new_c[2] = ']';
    int i, j = 0;
    for(i = a;i <=nA;i++){
        old_a[j++] = teste[i];
    }
    old_a[j] = '\0';
    j = 0;
    for(i = b;i <=nB;i++){
        old_b[j++] = teste[i];
    }
    old_b[j] ='\0';
    j = 3;
    for(i = c;i <=nC;i++){
        new_c[j++] = teste[i];
    }
    new_c[j] = '\0';
    avaliar(new_c);
    ini-=3;
    j = ini;
    teste[j++] = 'E';
    for(i = 0; old_a[i] != '\0';i++){
        teste[j++] = old_a[i];
    }
    for(i = 0; old_b[i] != '\0';i++){
        teste[j++] = old_b[i];
    }
    teste[j++] = '(';
    for(i = 0; new_c[i] != '\0';i++){
        teste[j++] = new_c[i];
    }
    teste[j++] = ')';
    teste[j] = '\0';
}

void logica_combinatorial(char *teste) {
    while(tem(teste, '[')) {
        int ini;
        associatividade_esquerda(teste, &ini);
        int a, nA, b, nB = -1, c, nC;
        a = ini;
        acha_argumento(teste, a, &nA);
        c = nA + 1;
        acha_argumento(teste, c, &nC);
        char aHasBracket = contains(teste, teste[ini - 2], a, nA);
        char cHasBracket = contains(teste, teste[ini - 2], c, nC);

        if(aHasBracket){
            if(a == nA){
                if(cHasBracket){
                    bracket_S(teste, ini, a, nA, c, nC);
                }else{
                    bracket_C(teste, ini, a, nA, c, nC);
                }
            }else{
                a = a+1;
                acha_argumento(teste, a, &nA);
                aHasBracket = contains(teste, teste[ini-2], a, nA);
                if(aHasBracket){
                    if(cHasBracket){
                        a = a-1;
                        acha_argumento(teste, a, &nA);
                        bracket_S(teste, ini, a, nA, c, nC);
                    }else{
                        a = a-1;
                        acha_argumento(teste, a, &nA);
                        bracket_C(teste, ini, a, nA, c, nC);
                    }
                }else{
                    b = nA+1;
                    acha_argumento(teste, b, &nB);
                    char bHasBracket = contains(teste,teste[ini-2],b, nB);
                    if(bHasBracket){
                        if(cHasBracket){
                            bracket_D(teste, ini, a, nA, b, nB, c, nC);
                        }else{
                            bracket_F(teste, ini, a, nA, b, nB, c, nC);
                        }
                    }else{
                        if(cHasBracket){
                            bracket_E(teste, ini, a, nA, b, nB, c, nC);
                        }else{
                            a = a-1;
                            acha_argumento(teste, a, &nA);
                            bracket_C(teste, ini, a, nA, c, nC);
                        }
                    }
                }
            }
        }else{
            if(cHasBracket){
                if(a == nA) {
                    bracket_B(teste, ini, a, nA, c, nC);
                }else {
                    a = a+1;
                    acha_argumento(teste, a, &nA);
                    b = nA+1;
                    acha_argumento(teste, b, &nB);
                    bracket_E(teste, ini,a, nA, b, nB, c, nC);
                }
            }else{
                printf("Erro");
                exit(1);
            }
        }
    }
}

void remove_parenteses_redundante(char *teste) {
    int i = 0;
    int j = 0;
    while(teste[j] != '\0'){
        teste[i] = teste[j];
        if(teste[j] == '(' && teste[j+2] == ')'){
            teste[i] = teste[j+1];
            j+=2;
        }
        i++;
        j++;
    }
    teste[i] = '\0';
}

void remove_condicionais(char* str){
    int i,j;
    for(i = 0; str[i] != '\0';i++){
        if(str[i] == 'i' && str[i+1] == 'f'){
            for(j = i; str[j+2] != '\0';j++){
                str[j] = str[j+2];
            }
            str[j] = '\0';
        }
        if(str[i] == 't' && str[i+1] == 'h' && str[i+2] == 'e' && str[i+3] == 'n'){
            for(j = i; str[j+4] != '\0';j++){
                str[j] = str[j+4];
            }
            str[j] = '\0';
        }
        if(str[i] == 'e' && str[i+1] == 'l' && str[i+2] == 's' && str[i+3] == 'e'){
            for(j = i; str[j+4] != '\0';j++){
                str[j] = str[j+4];
            }
            str[j] = '\0';
        }
    }
    str[i] = '\0';
}
char bate(char* fun, char *str){
    int i;
    for(i= 0; fun[i] != '\0';i++){
        if(fun[i] != str[i])
            return 0;
    }
    return 1;
}

void alpha_conversao(char *fun, char* str){
    int tam = strlen(fun);
    int i, j;
    for(i = 0; str[i] != '\0';i++){
        if(bate(fun, str+i)){
            j = i;
            str[j++] = 'A';
            for(; str[j+tam-1]!= '\0';j++){
                str[j] = str[j+tam-1];
            }
            str[j] = '\0';
        }
    }
}

void apply_bracket(char* str){
    int tam = strlen(str)+7;
    int i;
    str[tam-1] = '\0';
    for(i = tam-2; i >= 6;i--){
        str[i] = str[i-6];
    }
    int j;
    for(j = i;j < tam ; j++){
        if(str[j] >= 'a' && str[j] <= 'z'){
            break;
        }
    }
    char var = str[j];
    str[0] = '[';
    str[1] = 'A';
    str[2] = ']';
    str[3] = '[';
    str[4] = var;
    str[5] = ']';
}

char* converter_para_bracket(char* fun, char *str, char *valor){
    char string[T];
    strcpy(string, str);
//    printf("\n%s\n",string);
    remove_condicionais(string);
//    printf("\n%s\n",string);
    alpha_conversao(fun, string);
//    printf("\n%s\n",string);
    apply_bracket(string);
//    printf("\n%s\n",string);
    remove_parenteses_redundante(string);
//    printf("\n%s\n",string);
    logica_combinatorial(string);
//    printf("\n%s\n",string);
    remove_parenteses_redundante(string);
//    printf("\n%s\n",string);
    int tam = strlen(string);
    int i;
    for(i = tam+2;i >= 2;i--){
        string[i] = string[i-2];
    }
    string[tam+3] = '\0';
    string[0] = 'Y';
    string[1] = '(';
    tam = strlen(string);
    string[tam++] = ')';
//    printf("\n%s\n",string);
    for(int i = 0; i < strlen(valor);i++){
        if(valor[i] < '0' || valor[i] > '9'){
            valor[i] = '\0';
            break;
        }

    }
    strcpy(string+tam,valor);
//    printf("\n%s\n",string);
    tam = strlen(string);
    char* res = malloc(strlen(string)+1);
    strcpy(res, string);
    res[tam] = '\0';
    return res;
}