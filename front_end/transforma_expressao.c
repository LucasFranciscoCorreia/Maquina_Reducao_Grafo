#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1000
char string[T] = "+a b\0";

int main(){

    char *nova_string= calloc(T, sizeof(char));

    int tam = strlen(string);
    int offset=0;
    nova_string[offset++] ='(';//operador
    nova_string[offset++] =string[0];//operador
    for(int i=offset,j = 0; string[j] != '\0'; i++,j++,offset++){
        if(string[j] !=' ') {
            nova_string[i] = string[j];
            nova_string[++i]=')';
        }
        else{
            nova_string[i]='(';
        }
    }

    return 0;
}