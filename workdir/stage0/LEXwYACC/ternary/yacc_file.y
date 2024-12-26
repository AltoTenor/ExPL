%{
    #include <stdio.h>    
    #include <stdlib.h>    
    #include <string.h>   
    int yyerror(char const *s); 
    int yylex();
%}

%token NUM
%token EQ
%nonassoc '?' ':'
%nonassoc EQ
%left '+'

%%

S   :   A '\n'      {printf("Answer: %d\n", $$);exit(0);}
    ;
A   :   A '+' A     { $$ = $1+$3;}
    |   NUM         { $$ = $1;}
    |   B'?'A':'A   { $$ = $1?$3:$5;}
    ;

B   :   A EQ A      { $$ = ($1 == $3);}
;
%%

int yyerror(char const *s)
{
    printf("yyerror %s",s);
    return 0;
}

int main(){
    yyparse();
    return 0;
}