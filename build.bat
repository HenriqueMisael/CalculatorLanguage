del calclang.exe
bison -d parser.y
flex tokens.l
gcc -o calclang list.h list.c symbols.h symbols.c binary_tree.h binary_tree.c semantic_tree.h semantic_tree.c parser.tab.h parser.tab.c lex.yy.c -lfl -lm -w
del parser.tab.h parser.tab.c lex.yy.c