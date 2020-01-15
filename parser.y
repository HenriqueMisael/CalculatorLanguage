%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "symbols.h"
#include "binary_tree.h"

int yylex();
void yyerror (char *s);

int line = 1;
int column;

extern FILE *yyin;
%}

%start File

%union {float num; int integer; char *id; struct TREE_NODE *node;}

%token PRINT
%token END
%token ASSIGN PLUS MINUS TIMES DIVIDE POWER
%token LEFT RIGHT
%token <integer> INTEGER
%token <num> NUMBER
%token <id> IDENTIFIER

%right ASSIGN
%left PLUS MINUS
%left TIMES DIVIDE
%right POWER

%type <node> Expression
%type <id> Assignment

%%

File:
File: File Line

Line: END
Line: Command END
Line: Assignment END

Command: PRINT Expression { add_print_node(line, column, $2); }
Assignment: IDENTIFIER ASSIGN Expression  { add_assignment_node(line, column, $1,$3); }

Expression: NUMBER {$$ = create_float_node(line, column, &$1);}
	| INTEGER {$$ = create_integer_node(line, column, &$1);}
        | LEFT Expression RIGHT {$$ = $2;}
       	| Expression PLUS Expression {$$ = create_sum_node(line, column, $1, $3);}
       	| Expression MINUS Expression {$$ = create_minus_node(line, column, $1, $3);}
       	| Expression TIMES Expression {$$ = create_times_node(line, column, $1, $3);}
       	| Expression DIVIDE Expression {$$ = create_divide_node(line, column, $1, $3);}
       	| Expression POWER Expression {$$ = create_pow_node(line, column, $1,$3);}
       	| IDENTIFIER {$$ = create_symbol_node(line, column, $1);}

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

	initialize_semantic_tree();
	initialize_symbol_table();
	yyin = f;
	yyparse ();

	execute_all();
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);}
