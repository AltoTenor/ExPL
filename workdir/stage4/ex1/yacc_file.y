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
    extern int SP;
    extern struct Gsymbol * symbolTable;
%}

%union{
    struct tnode * no;
    struct tnode ** arr_no;
}

%type <no> expr program slist stmt identifier
%type <no> declarations declList decl type varList
%type <no> AsgStmt InputStmt OutputStmt IfStmt WhileStmt DoWhileStmt RepeatStmt
%token <no> START END DECL ENDDECL STR INT
%token <no> ID STRING NUM READ WRITE 
%token <no> IF ENDIF ELSE THEN  
%token <no> REPEAT UNTIL WHILE DO ENDWHILE BREAK CONTINUE
%token <no> GE GT LE LT EQ NE
%nonassoc GE GT LE LT EQ NE
%left '+' '-'
%left '*' '/' '%'

%%

program : declarations START slist END ';' {
                        $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, $3), 2, NULL);
                        printTree($$);
                        
                        int temp_arr[2];
                        temp_arr[0] = -1;
                        temp_arr[1] = -1;
                        initialize();
                        codeGen($3, temp_arr);
                    }
        | declarations START END {   
                        $$ = $1;
                        // printTree($$);
                    }
        | declarations slist {
                        $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, $2), 2, NULL);
                        printTree($$);

                        int temp_arr[2];
                        temp_arr[0] = -1;
                        temp_arr[1] = -1;
                        initialize();
                        codeGen($2, temp_arr);
                    }
;

declarations : DECL declList ENDDECL    { $$ = $2; }
        | DECL ENDDECL                  { $$ = NULL; }
;

declList: declList decl { $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, $2), 2, NULL); }
    | decl              { $$ = $1; }
;

decl : type varList ';'{ 
        setTypes( $2, $1->type );
        $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, $2), 2, NULL);
        
    }
;

type : INT  { $$ = $1; }
    | STR   { $$ = $1; }
;

varList : varList ',' ID    { 
        $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, $3), 2, NULL);
    }
    | varList ',' ID '[' NUM ']'    { 
        struct tnode * t = createTree(-1, invalidType, "",arrTypeNode,  alloc_2($3, $5), 2, NULL);
        $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, t), 2, NULL);
    }
    | ID '[' NUM ']'    { 
        $$ = createTree(-1, invalidType, "", arrTypeNode,  alloc_2($1, $3), 2, NULL); 
    }
    | varList ',' ID '[' NUM ']''[' NUM ']'    { 
        struct tnode * t = createTree(-1, invalidType, "", arr2dTypeNode,  alloc_3($3, $5, $8), 3, NULL); 
        $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, t), 2, NULL);
    }
    | ID '[' NUM ']' '[' NUM ']'    { 
        $$ = createTree(-1, invalidType, "", arr2dTypeNode,  alloc_3($1, $3, $6), 3, NULL); 
    }
    | ID    { $$ = $1; }
;

slist : slist stmt  { $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, $2), 2, NULL); } 
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

AsgStmt : identifier '=' expr ';' {
            $$ = createTree(-1, invalidType, "", assignNode , alloc_2($1, $3), 2, NULL);
        }
;

AsgStmt : identifier '=' STRING ';' {
            $$ = createTree(-1, invalidType, "", assignNode , alloc_2($1, $3), 2, NULL);
        }
;

identifier : ID         { $$ = $1; }
    | ID '[' expr ']' {   
        struct Gsymbol* g = Lookup($1->varname);
        if ( g == NULL ){
            printf("Undeclared Variable\n");
            exit(1);
        }
        $$ = createTree(-1, g->type , "", arrTypeNode,  alloc_2($1, $3), 2 , g);
    }
    | ID '[' expr ']' '[' expr ']' {
        struct Gsymbol* g = Lookup($1->varname);
        if ( g == NULL ){
            printf("Undeclared Variable\n");
            exit(1);
        }
        $$ = createTree(-1, g->type , "", arr2dTypeNode,  alloc_3($1, $3, $6), 3 , g);
    }
;


InputStmt : READ '(' identifier ')' ';' {
            $$ = createTree(-1, invalidType, "", readNode, alloc_1($3), 1, NULL);
        }
;

OutputStmt : WRITE '(' expr ')' ';'{
            $$ = createTree(-1, invalidType, "", writeNode, alloc_1($3), 1, NULL);
        }
;

OutputStmt : WRITE '(' STRING ')' ';'{
            $$ = createTree(-1, invalidType, "", writeNode, alloc_1($3), 1, NULL);
        }
;

IfStmt : IF '(' expr ')' THEN slist ELSE slist ENDIF ';' {
            $$ = createTree(-1, invalidType, "", ifNode, alloc_3($3, $6, $8), 3, NULL);
        }
    | IF '(' expr ')' THEN slist ENDIF ';' {
            $$ = createTree(-1, invalidType, "", ifNode, alloc_3($3, $6, NULL), 3, NULL);
        }
;

WhileStmt : WHILE '(' expr ')' DO slist ENDWHILE ';' {
            $$ = createTree(-1, invalidType, "", whileNode, alloc_2($3,$6), 2, NULL);
        }
;

DoWhileStmt : DO slist WHILE '(' expr ')' ';' {
            $$ = createTree(-1, invalidType, "", dowhileNode, alloc_2($2,$5), 2, NULL);
        }
;

RepeatStmt : REPEAT slist UNTIL '(' expr ')' ';' {
            $$ = createTree(-1, invalidType, "", repeatNode, alloc_2($2,$5), 2, NULL);
        }
;

expr : expr '+' expr    {   $$ = createTree(-1, intType , "", addNode, alloc_2($1, $3), 2, NULL); }
    | expr '*' expr     {   $$ = createTree(-1, intType , "", mulNode, alloc_2($1, $3), 2, NULL); }
    | expr '-' expr     {   $$ = createTree(-1, intType , "", subNode, alloc_2($1, $3), 2, NULL); }
    | expr '/' expr     {   $$ = createTree(-1, intType , "", divNode, alloc_2($1, $3), 2, NULL); }
    | expr '%' expr     {   $$ = createTree(-1, intType , "", modNode, alloc_2($1, $3), 2, NULL); }
    | expr GE expr      {   $$ = createTree(-1, boolType , "", geNode, alloc_2($1, $3), 2, NULL); }
    | expr LE expr      {   $$ = createTree(-1, boolType , "", leNode, alloc_2($1, $3), 2, NULL); }
    | expr LT expr      {   $$ = createTree(-1, boolType , "", ltNode, alloc_2($1, $3), 2, NULL); }
    | expr GT expr      {   $$ = createTree(-1, boolType , "", gtNode, alloc_2($1, $3), 2, NULL); }
    | expr NE expr      {   $$ = createTree(-1, boolType , "", neNode, alloc_2($1, $3), 2, NULL); }
    | expr EQ expr      {   $$ = createTree(-1, boolType , "", eqNode, alloc_2($1, $3), 2, NULL); }
    | NUM               {   $$ = $1; }
    | identifier        {   $$ = createTree(-1, $1->type , "", derefNode, alloc_1($1), 1, NULL); }
;

%%

int yyerror(char const *s)
{
    printf("yyerror %s\n",s);
    exit(1);
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
    symbolTable = NULL;
    SP = 4096;

    yyparse();
    fprintf(fp, "JMP EXIT\n");

    printSymbolTable();
    printf("YACC: Compiled succesfully (with labels)! \n");

    return 0;
}