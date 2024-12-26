%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <stddef.h>
    #include "exprtree.h"
    int yylex(void);
    int yyerror(char const *s);
%}

%union{
    struct tnode *no;
}
%type <no> expr program 
%token <no> NUM END
%left '+' '-'
%left '*' '/'

%%

program : expr END {
        $$ = $2;
        printf("Answer : %d\n",evaluate($1));
        printf("Prefix: ");prefix($1);printf("\n");
        printf("Postfix: ");postfix($1);printf("\n");
        exit(1);
    }
;

expr : expr '+' expr    {$$ = makeOperatorNode('+',$1,$3);}
    | expr '-' expr     {$$ = makeOperatorNode('-',$1,$3);}
    | expr '*' expr     {$$ = makeOperatorNode('*',$1,$3);}
    | expr '/' expr     {$$ = makeOperatorNode('/',$1,$3);}
    | '(' expr ')'      {$$ = $2;}
    | NUM               {$$ = $1;}
;

%%

int yyerror(char const *s)
{
    printf("yyerror %s",s);
    return 0;
}


int main(void) {
    yyparse();
    return 0;
}