#!/bin/bash

yacc -d gramatica.y
lex lexico.l
gcc lex.yy.c y.tab.c maquina_grafo_dinamica\(Cheney\).c -o main -lm
./main
