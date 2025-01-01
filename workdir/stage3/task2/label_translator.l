%{
    #include <stdlib.h>
    #include <stdio.h>
    void yyerror(char const *);
%}

%%
(.)+   {printf("Label Translating... %s\n", yytext);}
%%

int label_translate(){
    yyin = fopen("temp.xsm","r");
    yylex();
    return 0;
}

int yywrap(void) {
    return 1;
}