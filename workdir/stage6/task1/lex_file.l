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
(breakpoint)        {  yylval.no = createTree(NULL, NULL, NULL, brkpNode,  NULL , 0 , NULL, NULL);return BRKP; }

(begin)             {   return START; }
(end)               {   return END; }

(decl)              {   return DECL; }
(enddecl)           {   return ENDDECL; }

(type)              {   return TYPE; }
(endtype)           {   return ENDTYPE; }

(int)               {   return INT; }
(str)               {   return STR; }
(tuple)             {   return TUPLE; }
(null)              {   return NULL_KEYWORD; }

(main)              {   yylval.no = createTree("main", NULL, NULL, idNode,  NULL , 0 , NULL, NULL); return MAIN; }
(return)            {   return RETURN; }

(read)              {   return READ; }
(write)             {   return WRITE; }
(initialize)        {   return INITIALIZE; }
(alloc)             {   return ALLOC; }

(if)                {   return IF; }
(endif)             {   return ENDIF; }
(then)              {   return THEN; }
(else)              {   return ELSE; }

(while)             {   return WHILE; }
(do)                {   return DO; } 
(endwhile)          {   return ENDWHILE; }
(repeat)            {   return REPEAT; }
(until)             {   return UNTIL; }

(break)             {   yylval.no = createTree(NULL, NULL, NULL, breakNode,  NULL , 0 , NULL, NULL); return BREAK; }
(continue)          {   yylval.no = createTree(NULL, NULL, NULL, contNode,  NULL , 0 , NULL, NULL); return CONTINUE; }
                
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
                yylval.no = createTree(yytext, NULL, "void", idNode,  NULL , 0 , NULL, NULL);
                return ID; }

(\"(.)*\")  {   union Constant* c = (union Constant*)malloc(sizeof(union Constant));
                c->strval = (char*)malloc(sizeof(char)*20);
                strcpy( c->strval, yytext );
                yylval.no = createTree(NULL, c, "str", strConstNode,  NULL , 0 , NULL, NULL);
                return STRING; }

({digit})+  {   union Constant* c = (union Constant*)malloc(sizeof(union Constant));
                c->intval = atoi(yytext);
                yylval.no = createTree(NULL, c, "int", numNode, NULL , 0 , NULL, NULL);
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