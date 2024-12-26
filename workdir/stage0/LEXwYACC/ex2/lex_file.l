%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
%}

%%

[a-zA-Z]+ {
            // printf("Char: %c\n", (*yytext));
            yylval.str = (char *)malloc(sizeof(char*)*100);
            strcpy(yylval.str, yytext);
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