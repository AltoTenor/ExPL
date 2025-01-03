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
[a-z]       {   yylval.no = createTree(-1, intType, yytext, idNode,  NULL , 0 ); 
                return ID; }

[0-9]+      {   number = atoi(yytext); 
                yylval.no = createTree(number, intType, "", numNode, NULL , 0 );
                return NUM; }

[+-/*=();]  {   return *yytext; }
(begin)     {   return START; }
(end)       {   return END; }
(read)      {   return READ; }
(write)     {   return WRITE; }

(if)        {   return IF; }
(endif)     {   return ENDIF; }
(then)      {   return THEN; }
(else)      {   return ELSE; }

(while)     {   return WHILE; }
(do)        {   return DO; } 
(endwhile)  {   return ENDWHILE; }
(repeat)    {   return REPEAT; }
(until)     {   return UNTIL; }

(break)     {   yylval.no = createTree(-1, invalidType, "", breakNode,  NULL , 0 ); return BREAK; }
(continue)  {   yylval.no = createTree(-1, invalidType, "", contNode,  NULL , 0 ); return CONTINUE; }
                
(!=)        {   return NE; }
(==)        {   return EQ; }
(<=)        {   return LE; }
(<)         {   return LT; }
(>=)        {   return GE; }
(>)         {   return GT; }
[ \t\n]     {   }
.           {   yyerror("unknown character\n");exit(1); }
%%

int yywrap(void) {
    return 1;
}

int label_translate(){
    yyin = fopen("temp.xsm","r");
    yylex();
    return 0;
}