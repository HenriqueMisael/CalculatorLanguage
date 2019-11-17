%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int yylex();
int get_symbol_value(char symbol);
void yyerror (char *s);
void update_symbol_value(char symbol, int val);

int symbols[52];
%}

%start File;

%union {int num; char id;}

%token PRINT
%token END
%token ASSIGN PLUS MINUS TIMES DIVIDE
%token LEFT RIGHT
%token <num> NUMBER
%left PLUS MINUS
%left TIMES DIVIDE
%token <id> IDENTIFIER
%type <num> Expression Term
%type <id> Assignment

%%

File:
File: File Line

Line: END
Line: Expression END
Line: Command END
Line: Assignment END

Command: PRINT Expression { printf("%d\n", $2); }
Assignment: IDENTIFIER ASSIGN Expression  { update_symbol_value($1,$3); }

Expression: Term {$$ = $1;}
        | LEFT Expression RIGHT {$$ = $2;}
       	| Expression PLUS Term {$$ = $1 + $3;}
       	| Expression MINUS Term {$$ = $1 - $3;}
       	| Expression TIMES Term {$$ = $1 * $3;}
       	| Expression DIVIDE Term {$$ = $1 / $3;}

Term: NUMBER {$$ = $1;}
	| IDENTIFIER {$$ = get_symbol_value($1);}

%%

int computeSymbolIndex(char token)
{
	int idx = -1;
	if(islower(token)) {
		idx = token - 'a' + 26;
	} else if(isupper(token)) {
		idx = token - 'A';
	}
	return idx;
}

int get_symbol_value(char symbol)
{
	int bucket = computeSymbolIndex(symbol);
	return symbols[bucket];
}

void update_symbol_value(char symbol, int val)
{
	int bucket = computeSymbolIndex(symbol);
	symbols[bucket] = val;
}

int main(int argc, char *argv[])
{
	if(argc <= 1) {
		printf("Error: inform a file as program argument");
		return 1;
	}

	char *file_name = argv[1];
	FILE *f = fopen(file_name, "r");

	if(f == NULL) {
		printf("Error: unable to open file %s", file_name);
		return 1;
	}

	stdin = f;

	int i;
	for(i=0; i<52; i++) {
		symbols[i] = 0;
	}

	return yyparse ( );
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}
