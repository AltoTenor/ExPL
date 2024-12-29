%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <stddef.h>
    #include "exprtree.h"
    #include "codegen.h"
    int yylex(void);
    int yyerror(char const *s);
    extern FILE *yyin;
    extern FILE *fp;
    extern int temp;
%}

%union{
    struct tnode *no;
}
%type <no> expr program slist stmt 
%type <no> AsgStmt InputStmt OutputStmt
%token <no> NUM START END ID READ WRITE
%left '+' '-'
%left '*' '/'

%%

program : START slist END ';' {
                        $$ = $2;
                        // printTree($$);
                        codeGen($$);
                        fprintf(fp, "JMP    2056\n");
                        printf("Compiled successfully! \n");
                        int * ans = (int *)malloc(sizeof(int)*26);
                        evaluate($$, ans);
                        exit(0);
                    }
        | START END {   
                        codeGen($$);
                        fprintf(fp, "JMP    2056\n");
                        printf("Compiled successfully! \n");
                        exit(0);
                    }
;

slist : slist stmt  {$$ = createTree(-1, Connector, "", $1, $2); } 
    | stmt          {$$=$1;}
;

stmt : AsgStmt      {$$ = $1;} 
    | InputStmt     {$$ = $1;} 
    | OutputStmt    {$$ = $1;}
;

AsgStmt : ID '=' expr ';' {
            $$ = createTree(-1, Assign, "", $1, $3);
        }
;

InputStmt : READ '(' ID ')' ';' {
            $$ = createTree(-1, Read, "", $3, NULL);
        }
;

OutputStmt : WRITE '(' expr ')' ';'{
            $$ = createTree(-1, Write, "", $3, NULL);
        }
;

expr : expr '+' expr    {$$ = createTree(-1, Add, "", $1, $3);}
    | expr '*' expr     {$$ = createTree(-1, Mul, "", $1, $3);}
    | NUM               {$$ = $1; }
    | ID                {$$ = $1; }
;

%%

int yyerror(char const *s)
{
    printf("yyerror %s",s);
    return 0;
}


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Open the file specified in the first argument
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }

    initialize();
    yyparse();
    return 0;
}