del calclang.exe
bison -d parser.y
flex calclang.l
gcc -o calclang symbols.h symbols.c parser.tab.h parser.tab.c lex.yy.c -lfl -lm -w
del parser.tab.h parser.tab.c lex.yy.c