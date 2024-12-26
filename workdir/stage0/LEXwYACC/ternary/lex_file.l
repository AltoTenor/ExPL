%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "y.tab.h"
%}

equality    ()

%%

[0-9]+      {   yylval = atoi(yytext);
                return NUM;
            }
==          {   return EQ; }
[\n+?:]     {   return *yytext;}
[ .]        {}
%%

int yywrap()
{
    return 1;
}