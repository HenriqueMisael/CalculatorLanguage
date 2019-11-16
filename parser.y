%{
#include <math.h>
#include <stdio.h>
#define YYSTYPE double
int yyerror (char const *s);
extern int yylex (void);
%}

%token PRINT
%token NUMBER
%token PLUS MINUS TIMES DIVIDE POWER
%token LEFT RIGHT
%token END

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG
%right POWER

%start File
%%

File: ;
File: File Line;

Line: END
Line: Expression END
Line: Command END

Expression: NUMBER { $$=$1; };
Expression: Expression PLUS Expression { $$ =$1 + $3; };
Expression: Expression MINUS Expression { $$ = $1 - $3; };
Expression: Expression TIMES Expression { $$ = $1 * $3; };
Expression: Expression DIVIDE Expression { $$ = $1 / $3; };
Expression: MINUS Expression %prec NEG { $$ = -$2; };
Expression: PLUS Expression { $$ =$2 }
Expression: Expression POWER Expression { $$ = pow($1, $3); };
Expression: LEFT Expression RIGHT { $$ = $2; };

Command: PRINT Expression { printf("%f\n", $2); }

%%

int yyerror(char const *s) {
  printf("%s\n", s);
}

int main(int argc, char *argv[]) {

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

    int ret = yyparse();
    
    if (ret){
	fprintf(stderr, "%d error found.\n",ret);
    }
    
    return 0;
}

