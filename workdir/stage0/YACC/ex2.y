%{
/*** Auxiliary declarations section ***/
// Recogize single variables

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

/* Variable to keep track of the position of the number in the input */
int pos=0;

%}

 /*** YACC Declarations section ***/

%token DIGIT
%token ALPHA
%%

/*** Rules Section ***/
S   :   ALPHA C '\n'    { printf("Case 1\nSuccess\n");exit(0);}
    |   ALPHA '\n'      { printf("Case 2\nSuccess\n");exit(0);}
    ;   
C   :   C ALPHA     { printf("Case 4\n");}
    |   DIGIT       { printf("Case 5\n");}
    |   C DIGIT     { printf("Case 6\n");}
    |   ALPHA       { printf("Case 7\n");}
    ;
%%

/*** Auxiliary functions section ***/

int yyerror(char const *s)
{
    printf("Invalid Variable\n");
}

int yylex(){
    char c;
    c = getchar();
    printf("read: %c",c);
    if ( isalpha(c) ){
        pos++;
        return ALPHA;
    }
    else if( isdigit(c) ){
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