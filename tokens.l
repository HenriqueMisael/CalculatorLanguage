%{
#include "parser.tab.h"
void yyerror (char *s);
int yylex();
%}

white   [ \t]+
alpha   [a-zA-Z]+
integer [0-9]+
real    {integer}(\.{integer})?

%%

{white}     { }

"print"     return PRINT;
"\n"        return END;
"("         return LEFT;
")"         return RIGHT;
"="         return ASSIGN;
"+"         return PLUS;
"-"         return MINUS;
"*"         return TIMES;
"/"         return DIVIDE;
"^"         return POWER;

{alpha}     {yylval.id = strdup(yytext); return IDENTIFIER;}
{real}      {yylval.num = atof(yytext); return NUMBER;}
.           {yyerror ("Unexpected token");}

%%

int yywrap (void) {return 1;}
