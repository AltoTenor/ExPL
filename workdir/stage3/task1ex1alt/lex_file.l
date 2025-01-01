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
				yylval.no = createTree(-1, intType, yytext, idNode,  NULL , 0 ); return ID; }
[0-9]+      {  
                // printf("Case 1\n");
                number = atoi(yytext); yylval.no = createTree(number, intType, "", numNode, NULL , 0 ); return NUM;}
[+-/*=();]  {  
                // printf("Case 2\n");
                return *yytext;}
(begin)     {  
                // printf("Case 3\n");
                return START;}
(end)       {  
                // printf("Case 4\n");
                return END;}
(read)      {  
                // printf("Case 6\n");
                return READ;}
(write)     {  
                // printf("Case 7\n");
                return WRITE;}
(if)     {  
                // printf("Case 8\n");
                return IF;}
(endif)     {  
                // printf("Case 9\n");
                return ENDIF;}
(then)     {  
                // printf("Case 10\n");
                return THEN;}
(else)     {  
                // printf("Case 11\n");
                return ELSE;}
(while)     { return WHILE; }
(do)        { return DO; } 
(endwhile)  { return ENDWHILE; }
                
(!=)        {   return NE; }
(==)        {   return EQ; }
(<=)        {   return LE; }
(<)         {   return LT; }
(>=)        {   return GE; }
(>)         {   return GT; }
[ \t\n]     {  
                // printf("Case 8\n");
                }
.           {  
                // printf("Case 9\n");
                yyerror("unknown character\n");exit(1);}

%%

int yywrap(void) {
    return 1;
}