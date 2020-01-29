%{
#include <string.h>
#include "parser.tab.h"
void yyerror (char *s);
int yylex();

extern int line;
extern int column;
%}

line_break  [\n]
white       [ \t]+
alpha       [a-zA-Z]+
integer     [0-9]+
real        {integer}(\.{integer})

%%

{white}     { }

"print"     {column += 5; return PRINT;}
";"         {column++; return END;}
"("         {column++; return LEFT;}
")"         {column++; return RIGHT;}
"="         {column++; return ASSIGN;}
"+"         {column++; return PLUS;}
"-"         {column++; return MINUS;}
"*"         {column++; return TIMES;}
"/"         {column++; return DIVIDE;}
"^"         {column++; return POWER;}

{alpha}         {column += strlen(yytext); yylval.id = strdup(yytext); return IDENTIFIER;}
{real}          {column += strlen(yytext); yylval.num = atof(yytext); return NUMBER;}
{integer}       {column += strlen(yytext); yylval.integer = atoi(yytext); return INTEGER;}
{line_break}    {line++; column = 0;}
.               {column++; yyerror ("Unexpected token");}

%%

int yywrap (void) {return 1;}
