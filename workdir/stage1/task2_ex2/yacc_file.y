%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <stddef.h>
    #include "exprtree.h"
    int yylex(void);
    int yyerror(char const *s);
    extern FILE *fp;
    extern int temp;
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
        printf("Answer : %d\n",evaluate($1));
        printf("Prefix: ");prefix($1);printf("\n");
        printf("Postfix: ");postfix($1);printf("\n");
        int reg = codeGen($1);
        fprintf(fp, "MOV    [4096], R%d\n", reg);
        fprintf(fp, "CALL   2110\n");
        fprintf(fp, "JMP    2056\n");
        exit(0);
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
    initialize();
    yyparse();
    return 0;
}