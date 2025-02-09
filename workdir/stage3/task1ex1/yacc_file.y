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
%type <no> AsgStmt InputStmt OutputStmt IfStmt WhileStmt
%token <no> NUM START END ID READ WRITE IF ENDIF ELSE THEN WHILE DO ENDWHILE
%token <no> GE GT LE LT EQ NE
%nonassoc GE GT LE LT EQ NE
%left '+' '-'
%left '*' '/'

%%

program : START slist END ';' {
                        $$ = $2;
                        printTree($$);
                        // codeGen($$);
                        // fprintf(fp, "JMP    2056\n");
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

slist : slist stmt  {$$ = createTree(-1, invalidType, "",connectorNode,  $1, $2); } 
    | stmt          {$$ = $1;}
;

stmt : AsgStmt      {$$ = $1;} 
    | InputStmt     {$$ = $1;} 
    | OutputStmt    {$$ = $1;}
    | IfStmt        {$$ = $1;}
    | WhileStmt     {$$ = $1;}
;

AsgStmt : ID '=' expr ';' {
            $$ = createTree(-1, invalidType, "", assignNode , $1, $3);
        }
;

InputStmt : READ '(' ID ')' ';' {
            $$ = createTree(-1, invalidType, "", readNode, $3, NULL);
        }
;

OutputStmt : WRITE '(' expr ')' ';'{
            $$ = createTree(-1, invalidType, "", writeNode, $3, NULL);
        }
;

IfStmt : IF '(' expr ')' THEN slist ELSE slist ENDIF ';' {
                struct tnode* temp = createTree(-1, invalidType, "", ifstmtNode, $6, $8);
                $$ = createTree(-1, invalidType, "", ifNode, $3, temp);
            }
    | IF '(' expr ')' THEN slist ENDIF ';' {
                struct tnode* temp = createTree(-1, invalidType, "", ifstmtNode, $6, NULL);
                $$ = createTree(-1, invalidType, "", ifNode, $3, temp);
            }
;

WhileStmt : WHILE '(' expr ')' DO slist ENDWHILE ';' {
                $$ = createTree(-1, invalidType, "", whileNode, $3, $6);
            }
;


expr : expr '+' expr    { $$ = createTree(-1, intType , "", addNode, $1, $3); }
    | expr '*' expr     { $$ = createTree(-1, intType , "", mulNode, $1, $3); }
    | expr '-' expr     { $$ = createTree(-1, intType , "", subNode, $1, $3); }
    | expr '/' expr     { $$ = createTree(-1, intType , "", divNode, $1, $3); }
    | expr GE expr      { $$ = createTree(-1, boolType , "", geNode, $1, $3); }
    | expr LE expr      { $$ = createTree(-1, boolType , "", leNode, $1, $3); }
    | expr LT expr      { $$ = createTree(-1, boolType , "", ltNode, $1, $3); }
    | expr GT expr      { $$ = createTree(-1, boolType , "", gtNode, $1, $3); }
    | expr NE expr      { $$ = createTree(-1, boolType , "", neNode, $1, $3); }
    | expr EQ expr      { $$ = createTree(-1, boolType , "", eqNode, $1, $3); }
    | NUM               { $$ = $1; }
    | ID                { $$ = $1; }
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