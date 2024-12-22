%{
    #include <stdio.h>
    #include "y.tab.h"
%}

%%

[a-zA-Z] {
            // printf("Char: %c\n", (*yytext));
            yylval.c = *yytext;
            return LETTER;
        }
"+"     return *yytext;
"*"     return *yytext;
[()]    return *yytext;
[\n]    return *yytext;

%%

int yywrap()
{
    return 1;
}