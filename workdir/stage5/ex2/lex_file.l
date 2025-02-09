%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "y.tab.h"
    #include "analyser.h"
    #include "codegen.h"
    int number;
    void yyerror(char const *);

%}

letter [a-zA-Z]
digit [0-9]


%%
(breakpoint)        {  yylval.no = createTree(-1, invalidType, "", brkpNode,  NULL , 0 , NULL, NULL);return BRKP; }

(begin)             {   return START; }
(end)               {   return END; }

(decl)              {   return DECL; }
(enddecl)           {   return ENDDECL; }
(int)               {   return INT; }
(str)               {   return STR; }

(main)              {   yylval.no = createTree(-1, invalidType, "main", idNode,  NULL , 0 , NULL, NULL); return MAIN; }
(return)            {   return RETURN; }

(read)              {   return READ; }
(write)             {   return WRITE; }

(if)                {   return IF; }
(endif)             {   return ENDIF; }
(then)              {   return THEN; }
(else)              {   return ELSE; }

(while)             {   return WHILE; }
(do)                {   return DO; } 
(endwhile)          {   return ENDWHILE; }
(repeat)            {   return REPEAT; }
(until)             {   return UNTIL; }

(tuple)             {   return TUPLE; }

(break)             {   yylval.no = createTree(-1, invalidType, "", breakNode,  NULL , 0 , NULL, NULL); return BREAK; }
(continue)          {   yylval.no = createTree(-1, invalidType, "", contNode,  NULL , 0 , NULL, NULL); return CONTINUE; }
                
(!=)                {   return NE; }
(==)                {   return EQ; }
(<=)                {   return LE; }
(<)                 {   return LT; }
(>=)                {   return GE; }
(>)                 {   return GT; }

(and)|(AND)|(&&)    {   return AND; }
(or)|(OR)|(\|\|)    {   return OR; }



[ \t\n]             {   }
[+-/*=();%]         {   return *yytext; }
[\[\]&{}]           {   return *yytext; }

({letter})({letter}|{digit})* { 
                yylval.no = createTree(-1, intType, yytext, idNode,  NULL , 0 , NULL, NULL);
                return ID; }

(\"(.)*\")  {   yylval.no = createTree(-1, strType, yytext, strConstNode,  NULL , 0 , NULL, NULL);
                return STRING; }

({digit})+  {   number = atoi(yytext); 
                yylval.no = createTree(number, intType, "", numNode, NULL , 0 , NULL, NULL);
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