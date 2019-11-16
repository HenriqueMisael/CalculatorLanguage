bison -d parser.y
flex calclang.l
gcc -o calclang parser.tab.h parser.tab.c lex.yy.c -lfl -lm -w