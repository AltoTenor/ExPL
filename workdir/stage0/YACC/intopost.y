%{
/*** Auxiliary declarations section ***/
// Single Digit Infix to Postfix Converter
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
int yylex();
int yyerror(char const *s);
/* Custom function to print an operator*/
void print_operator(char op);

/* Variable to keep track of the position of the number in the input */
int pos=0;

%}

 /*** YACC Declarations section ***/
%token DIGIT
%left '+'
%left '*'
%%

/*** Rules Section ***/
start : expr '\n'  {exit(1);}
      ;

expr: expr '+' expr     {printf("PLUS ");}
    | expr '*' expr     {printf("MUL ");}
    | '(' expr ')'
    | DIGIT             {printf("NUM %d ",pos);}
    ;

%%

/*** Auxiliary functions section ***/

int yyerror(char const *s)
{
    printf("yyerror %s",s);
}

int yylex(){
    char c;
    c = getchar();
    if(isdigit(c)){
        pos++;
        return DIGIT;
    }
    else if(c == ' '){
        yylex();         /*This is to ignore whitespaces in the input*/
    }
    else {
        return c;
    }
}

int main()
{
    yyparse();
    return 1;
}