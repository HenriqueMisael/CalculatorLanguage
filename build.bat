del calclang.exe
bison -d parser.y
flex tokens.l
gcc -o calclang list.h list.c symbols.h symbols.c parser.tab.h parser.tab.c lex.yy.c -lfl -lm -w
del parser.tab.h parser.tab.c lex.yy.c