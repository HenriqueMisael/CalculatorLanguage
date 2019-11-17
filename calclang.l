%{
#include "parser.tab.h"
void yyerror (char *s);
int yylex();
%}

white     [ \t]+
alpha     [a-zA-Z]
digit     [0-9]+

%%

{white}     { }

"print"     return PRINT;
"\n"        return END;
"+"         return PLUS;
"-"         return MINUS;
"="         return ASSIGN;

{alpha}     {yylval.id = yytext[0]; return IDENTIFIER;}
{digit}     {yylval.num = atoi(yytext); return NUMBER;}
.           {yyerror ("Unexpected token");}

%%

int yywrap (void) {return 1;}
