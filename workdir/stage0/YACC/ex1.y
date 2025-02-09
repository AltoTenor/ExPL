%{
/*** Auxiliary declarations section ***/
// Single Digit Infix to Postfix Converter
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
int yyerror(char const *s);
int yylex();

/* Custom function to print an operator*/
void print_operator(char op);

/* Variable to keep track of the position of the number in the input */
int pos=0;

%}

 /*** YACC Declarations section ***/
%token ETC
%token IF
%%

/*** Rules Section ***/
S   :   E '\n'  { printf("Case 1\n");exit(1);}
    ;
E   :   ETC     { printf("Case 2\n");}
    |   E E     { printf("Case 3\n");}
    |   IF      { printf("Case 4 IF level: %d\n", ++pos);}
    ;
%%

/*** Auxiliary functions section ***/

int yyerror(char const *s)
{
    printf("yyerror %s",s);
}

int yylex(){
    char a,b;
    a = getchar();
    if ( a == '\n' ) return '\n';
    b = getchar();
    // printf("Read: %c%c\n",a,b);
    if ( a == 'i' && b=='f' ){
        return IF;
    }
    ungetc(b, stdin);
    return ETC;
}

int main()
{
    yyparse();
    return 1;
}