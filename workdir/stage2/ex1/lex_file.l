%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "y.tab.h"
    #include "exprtree.h"
    #include "codegen.h"
    int number;
    void yyerror(char const *);

%}

%%
[a-z]       {   
				// printf("Case 0\n");
				yylval.no = createTree(-1, Id, yytext, NULL , NULL ); return ID; }
[0-9]+      {  
                // printf("Case 1\n");
                number = atoi(yytext); yylval.no = createTree(number, Num, "", NULL , NULL ); return NUM;}
[+-/*=();]    {  
                // printf("Case 2\n");
                return *yytext;}
(begin)     {  
                // printf("Case 3\n");
                return START;}
(end)       {  
                // printf("Case 4\n");
                return END;}
(read)       {  
                // printf("Case 6\n");
                return READ;}
(write)       {  
                // printf("Case 7\n");
                return WRITE;}
[ \t\n]       {  
                // printf("Case 8\n");
                }

.           {  
                // printf("Case 9\n");
                yyerror("unknown character\n");exit(1);}

%%

int yywrap(void) {
    return 1;
}