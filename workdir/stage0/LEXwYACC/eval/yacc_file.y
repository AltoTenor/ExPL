%{
    #include <stdio.h>
    #include <stdlib.h>
    int yyerror();
    int yylex();
%}

%token DIGIT 
%token NEWLINE

%left '+' '-'
%left '*' '/'

%%

start : expr NEWLINE  { printf("Expression value = %d\n",$1);exit(0);}
    ;

expr:   expr '+' expr  {$$ = $1 + $3;}
    |   expr '*' expr  {$$ = $1 * $3;}
    |   expr '-' expr  {$$ = $1 - $3;}
    |   expr '/' expr  {$$ = $1 / $3;}
    |   '(' expr ')'   {$$ = $2;}
    |   DIGIT   {$$ = $1;}
    ;

%%

int yyerror()
{
    printf("Error");
}

int main()
{
    yyparse();
    return 1;
}
