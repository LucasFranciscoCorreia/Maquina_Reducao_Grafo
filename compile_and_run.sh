#!/bin/bash

yacc -d gramatica.y
lex lexico.l
gcc lex.yy.c y.tab.c -o main -lm
./main
