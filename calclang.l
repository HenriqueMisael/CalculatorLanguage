%{
#define YYSTYPE double
#include "parser.tab.h"
%}

white [ \t]+
digit [0-9]
integer {digit}+
exponent [eE][+-]?{integer}
real {integer}("."{integer})?{exponent}?

%%

{white} { }
{real} { yylval=atof(yytext); return NUMBER; }

"+" return PLUS;
"-" return MINUS;
"*" return TIMES;
"/" return DIVIDE;
"^" return POWER;
"(" return LEFT;
")" return RIGHT;
"\n" return END;
"print" return PRINT;
