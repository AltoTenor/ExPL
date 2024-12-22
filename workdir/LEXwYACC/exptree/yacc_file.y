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
%token PLUS MINUS MUL DIV
%token <no> NUM END
%left PLUS MINUS
%left MUL DIV

%%

program : expr END {
        $$ = $2;
        printf("\nAnswer : %d\n",evaluate($1));

        exit(1);
    }
;

expr : expr PLUS expr  {$$ = makeOperatorNode('+',$1,$3);}
    | expr MINUS expr   {$$ = makeOperatorNode('-',$1,$3);}
    | expr MUL expr {$$ = makeOperatorNode('*',$1,$3);}
    | expr DIV expr {$$ = makeOperatorNode('/',$1,$3);}
    | '(' expr ')'  {$$ = $2;}
    | NUM   {$$ = $1;}
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