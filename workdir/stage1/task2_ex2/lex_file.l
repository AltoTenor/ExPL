%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "y.tab.h"
    #include "exprtree.h"
    int number;
    void yyerror(char const *);

%}

%%

[0-9]+  {   number = atoi(yytext); yylval.no = makeLeafNode(number); return NUM;}
[+-/*]  {   return *yytext;}
[ \t]   {   }
[()]    {   return *yytext;}
[\n]    {   return END;}
.       {   yyerror("unknown character\n");exit(1);}

%%

int yywrap(void) {
    return 1;
}