%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "y.tab.h"
    #include "exprtree.h"
    int number;
    void yyerror(char const *);

%}

%%
[a-z]       { yylval.no = createTreeNode(-1, Id, yytext, NULL , NULL ); return ID; }
[0-9]+      { number = atoi(yytext); yylval.no = createTreeNode(number, Num, "", NULL , NULL ); return NUM;}
[+-/*=();]  { return *yytext;}
(begin)     { return START;}
(end)       { return END;}
(read)      { return READ;}
(write)     { return WRITE;}
[ \t\n]     { }
.           {  yyerror("unknown character\n");exit(1);}

%%

int yywrap(void) {
    return 1;
}