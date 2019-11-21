%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "symbols.h"

int yylex();
void yyerror (char *s);

extern FILE *yyin;
%}

%start File

%union {float num; char *id;}

%token PRINT
%token END
%token ASSIGN PLUS MINUS TIMES DIVIDE POWER
%token LEFT RIGHT
%token <num> NUMBER
%left PLUS MINUS
%left TIMES DIVIDE
%right POWER
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

Command: PRINT Expression { printf("%f\n", $2); }
Assignment: IDENTIFIER ASSIGN Expression  { update_symbol_value($1,$3); }

Expression: Term {$$ = $1;}
        | LEFT Expression RIGHT {$$ = $2;}
       	| Expression PLUS Expression {$$ = $1 + $3;}
       	| Expression MINUS Expression {$$ = $1 - $3;}
       	| Expression TIMES Expression {$$ = $1 * $3;}
       	| Expression DIVIDE Expression {$$ = $1 / $3;}
       	| Expression POWER Expression {$$ = pow($1,$3);}

Term: NUMBER {$$ = $1;}
	| IDENTIFIER {$$ = get_symbol_value($1);}

%%

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

	initialize_symbol_table();
	yyin = f;
	return yyparse ( );
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}
