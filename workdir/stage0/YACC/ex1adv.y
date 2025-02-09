%{
// Single Digit Infix to Postfix Converter
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
int yylex();
int yyerror(char const *s);

%}

%token NUM IF TRUE FALSE ELSE RELOP
%left '+'

%%

start   : stmt '\n'                         {printf("Max Depth of IF nesting: %d\n", $1);exit(1);}
    ;
stmt    : NUM                               {$$ = 0;}
        | IF '(' B ')' stmt                 {$$ = 1 + MAX($3, $5); }
        | IF '(' B ')' stmt ELSE stmt       {$$ = 1 + MAX($3, MAX($5, $7));}
        | stmt '+' stmt                     {$$ = MAX($1, $3); }
    ;
B       : TRUE                              {$$ = 0;}
        | FALSE                             {$$ = 0;}
        | stmt RELOP stmt                   {$$ = MAX($1, $3); }
    ;
%%

int yyerror(char const *s)
{
    printf("yyerror %s",s);
}

int yylex(){
    char a, b, c, d, e;
    a = getchar();
    // printf("Read: %c\n", a);
    if ( a == ' ' || a == '\t') return yylex();

    // NUM = Any number
    if ( isdigit(a) ) return NUM;

    // IF = if
    if ( a == 'i' ){
        b = getchar();
        if ( b == 'f' ){
            return IF;
        }
        else ungetc(b, stdin);
    }
    //ELSE = else
    if ( a == 'e' ){
        b = getchar();
        c = getchar();
        d = getchar();
        if ( b == 'l' && c=='s' && d == 'e'){
            return ELSE;
        }
        else{
            ungetc(d, stdin);
            ungetc(c, stdin);
            ungetc(b, stdin);
        }
    }
    //TRUE = true
    if ( a == 't' ){
        b = getchar();
        c = getchar();
        d = getchar();
        if ( b == 'r' && c=='u' && d == 'e'){
            return TRUE;
        }
        else{
            ungetc(d, stdin);
            ungetc(c, stdin);
            ungetc(b, stdin);
        }
    }
    //FALSE = false
    if ( a == 'f' ){
        b = getchar();
        c = getchar();
        d = getchar();
        e = getchar();
        if ( b == 'a' && c=='l' && d == 's' && e == 'e'){
            return FALSE;
        }
        else{
            ungetc(e, stdin);
            ungetc(d, stdin);
            ungetc(c, stdin);
            ungetc(b, stdin);
        }
    }
    //RELOPs
    if ( a == '>' ||  a == '<'){
        b = getchar();
        if ( b == '=' ) return RELOP;
        else {
            ungetc(b, stdin);
            return RELOP;
        }
    }
    if ( a == '=' ){
        b = getchar();
        if ( b == '=' ) return RELOP;
        else ungetc(b, stdin);
    }
    return a;
}

int main()
{
    yyparse();
    return 1;
}