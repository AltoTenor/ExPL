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
(breakpoint)        {   yylval.no = createTree(NULL, NULL, NULL, NODE_BRKP,  NULL , 0 , NULL, NULL);
                        return BRKP; }

(begin)             {   return START; }
(end)               {   return END; }

(decl)              {   return DECL; }
(enddecl)           {   return ENDDECL; }

(type)              {   return TYPE; }
(endtype)           {   return ENDTYPE; }

(class)             {   return CLASS; }
(endclass)          {   return ENDCLASS; }
(extends)           {   return EXTENDS; }

(int)               {   return INT; }
(str)               {   return STR; }
(tuple)             {   return TUPLE; }
(null)              {   return NULL_KEYWORD; }
(free)              {   return FREE; }
(self)              {   return SELF; }
(new)               {   return NEW; }
(delete)            {   return DELETE; }

(main)              {   yylval.no = createTree("main", NULL, NULL, NODE_ID,  NULL , 0 , NULL, NULL); 
                        return MAIN; }
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

(break)             {   yylval.no 
                            = createTree(NULL, NULL, NULL, NODE_BREAK,  NULL , 0 , NULL, NULL); 
                        return BREAK; }
(continue)          {   yylval.no 
                            = createTree(NULL, NULL, NULL, NODE_CONTINUE,  NULL , 0 , NULL, NULL); 
                        return CONTINUE; }
                
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
                yylval.no = createTree(yytext, NULL, "void", NODE_ID,  NULL , 0 , NULL, NULL);
                return ID; }

(\"[^\"]*\")  {   union Constant* c = (union Constant*)malloc(sizeof(union Constant));
                c->strval = (char*)malloc(sizeof(char)*20);
                strcpy( c->strval, yytext );
                yylval.no = createTree(NULL, c, "str", NODE_CONST_STR,  NULL , 0 , NULL, NULL);
                return STRING; }

({digit})+  {   union Constant* c = (union Constant*)malloc(sizeof(union Constant));
                c->intval = atoi(yytext);
                yylval.no = createTree(NULL, c, "int", NODE_CONST_NUM, NULL , 0 , NULL, NULL);
                return NUM; }

(.)         {   printf("unknown character: %s\n", yytext); yyerror("\n");exit(1); }

%%

int yywrap(void) {
    return 1;
}
