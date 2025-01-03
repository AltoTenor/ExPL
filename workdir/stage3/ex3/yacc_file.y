%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <stddef.h>
    #include "exprtree.h"
    #include "codegen.h"
    int yylex(void);
    int yyerror(char const *s);
    struct tnode** alloc_1(struct tnode * t1);
    struct tnode** alloc_2(struct tnode * t1, struct tnode * t2);
    struct tnode** alloc_3(struct tnode * t1, struct tnode * t2,  struct tnode * t3);
    extern FILE *yyin;
    extern FILE *fp;
    extern int temp;
%}

%union{
    struct tnode *no;
}

%type <no> expr program slist stmt 
%type <no> AsgStmt InputStmt OutputStmt IfStmt WhileStmt DoWhileStmt RepeatStmt
%token <no> START END 
%token <no> ID NUM READ WRITE 
%token <no> IF ENDIF ELSE THEN  
%token <no> REPEAT UNTIL WHILE DO ENDWHILE BREAK CONTINUE
%token <no> GE GT LE LT EQ NE
%nonassoc GE GT LE LT EQ NE
%left '+' '-'
%left '*' '/'

%%

program : START slist END ';' {
                        $$ = $2;
                        printTree($$);
                        int temp_arr[2];
                        temp_arr[0] = -1;
                        temp_arr[1] = -1;
                        codeGen($$, temp_arr);
                        int * ans = (int *)malloc(sizeof(int)*26);
                        evaluate($$, ans);
                    }
        | START END {   
                        // codeGen($$);
                    }
;

slist : slist stmt  { $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, $2), 2); } 
    | stmt          { $$ = $1; }
;

stmt : AsgStmt      { $$ = $1; } 
    | InputStmt     { $$ = $1; } 
    | OutputStmt    { $$ = $1; }
    | IfStmt        { $$ = $1; }
    | WhileStmt     { $$ = $1; }
    | DoWhileStmt   { $$ = $1; }
    | RepeatStmt    { $$ = $1; }
    | BREAK ';'     { $$ = $1; }
    | CONTINUE ';'  { $$ = $1; }
;

AsgStmt : ID '=' expr ';' {
            $$ = createTree(-1, invalidType, "", assignNode , alloc_2($1, $3), 2);
        }
;

InputStmt : READ '(' ID ')' ';' {
            $$ = createTree(-1, invalidType, "", readNode, alloc_1($3), 1);
        }
;

OutputStmt : WRITE '(' expr ')' ';'{
            $$ = createTree(-1, invalidType, "", writeNode, alloc_1($3), 1);
        }
;

IfStmt : IF '(' expr ')' THEN slist ELSE slist ENDIF ';' {
                $$ = createTree(-1, invalidType, "", ifNode, alloc_3($3, $6, $8), 3);
            }
    | IF '(' expr ')' THEN slist ENDIF ';' {
                $$ = createTree(-1, invalidType, "", ifNode, alloc_3($3, $6, NULL), 3);
            }
;

WhileStmt : WHILE '(' expr ')' DO slist ENDWHILE ';' {
                $$ = createTree(-1, invalidType, "", whileNode, alloc_2($3,$6), 2);
            }
;

DoWhileStmt : DO slist WHILE '(' expr ')' ';' {
                $$ = createTree(-1, invalidType, "", dowhileNode, alloc_2($2,$5), 2);
            }
;

RepeatStmt : REPEAT slist UNTIL '(' expr ')' ';' {
                $$ = createTree(-1, invalidType, "", repeatNode, alloc_2($2,$5), 2);
            }
;

expr : expr '+' expr    {   $$ = createTree(-1, intType , "", addNode, alloc_2($1, $3), 2); }
    | expr '*' expr     {   $$ = createTree(-1, intType , "", mulNode, alloc_2($1, $3), 2); }
    | expr '-' expr     {   $$ = createTree(-1, intType , "", subNode, alloc_2($1, $3), 2); }
    | expr '/' expr     {   $$ = createTree(-1, intType , "", divNode, alloc_2($1, $3), 2); }
    | expr GE expr      {   $$ = createTree(-1, boolType , "", geNode, alloc_2($1, $3), 2); }
    | expr LE expr      {   $$ = createTree(-1, boolType , "", leNode, alloc_2($1, $3), 2); }
    | expr LT expr      {   $$ = createTree(-1, boolType , "", ltNode, alloc_2($1, $3), 2); }
    | expr GT expr      {   $$ = createTree(-1, boolType , "", gtNode, alloc_2($1, $3), 2); }
    | expr NE expr      {   $$ = createTree(-1, boolType , "", neNode, alloc_2($1, $3), 2); }
    | expr EQ expr      {   $$ = createTree(-1, boolType , "", eqNode, alloc_2($1, $3), 2); }
    | NUM               {   $$ = $1; }
    | ID                {   $$ = $1; }
;

%%

int yyerror(char const *s)
{
    printf("yyerror %s",s);
    return 0;
}

struct tnode** alloc_1(struct tnode * t1){
    struct tnode ** arr = (struct tnode **)malloc(sizeof(struct tnode*));
    arr[0] = t1;
    return arr;
}

struct tnode** alloc_2(struct tnode * t1, struct tnode * t2){
    struct tnode ** arr = (struct tnode **)malloc(2*sizeof(struct tnode*));
    arr[0] = t1;
    arr[1] = t2;
    return arr;
}

struct tnode** alloc_3(struct tnode * t1, struct tnode * t2,  struct tnode * t3){
    struct tnode ** arr = (struct tnode **)malloc(3*sizeof(struct tnode*));
    arr[0] = t1;
    arr[1] = t2;
    arr[2] = t3;
    return arr;
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
    fprintf(fp, "JMP EXIT\n");
    printf("YACC: Compiled succesfully (with labels)! \n");

    return 0;
}