%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "y.tab.h"
    #include "exprtree.h"
    #include "codegen.h"
    int number;
    void yyerror(char const *);

%}

letter [a-zA-Z]
digit [0-9]


%%
(begin)     {   return START; }
(end)       {   return END; }

(decl)      {   return DECL; }
(enddecl)   {   return ENDDECL; }
(int)       {   yylval.no = createTree(-1, intType, "", intTypeNode,  NULL , 0, NULL); return INT; }
(str)       {   yylval.no = createTree(-1, strType, "", strTypeNode,  NULL , 0, NULL); return STR; }

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

(break)     {   yylval.no = createTree(-1, invalidType, "", breakNode,  NULL , 0 , NULL); return BREAK; }
(continue)  {   yylval.no = createTree(-1, invalidType, "", contNode,  NULL , 0 , NULL); return CONTINUE; }
                
(!=)        {   return NE; }
(==)        {   return EQ; }
(<=)        {   return LE; }
(<)         {   return LT; }
(>=)        {   return GE; }
(>)         {   return GT; }
[ \t\n]     {   }
[+-/*=();%] {   return *yytext; }
[\[\]]      {   return *yytext; }

({letter})({letter}|{digit})* { 
                struct Gsymbol * entry = Lookup(yytext);
                if ( entry == NULL )
                    yylval.no = createTree(-1, invalidType, yytext, idNode,  NULL , 0 , NULL);
                else
                    yylval.no = createTree(-1, entry->type, yytext, idNode,  NULL , 0 , entry);
                return ID; }

(\"(.)*\")  {   yylval.no = createTree(-1, strType, yytext, strConstNode,  NULL , 0 , NULL);
                return STRING; }

({digit})+  {   number = atoi(yytext); 
                yylval.no = createTree(number, intType, "", numNode, NULL , 0 , NULL);
                return NUM; }

(.)         {   printf("unknown character: %s\n", yytext); yyerror("\n");exit(1); }

%%

int yywrap(void) {
    return 1;
}

int label_translate(){
    yyin = fopen("temp.xsm","r");
    yylex();
    return 0;
}