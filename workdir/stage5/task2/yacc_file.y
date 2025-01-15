%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <stddef.h>
    #include "analyser.h"
    #include "codegen.h"
    int yylex(void);
    int yyerror(char const *s);
    struct tnode** alloc_1(struct tnode * t1);
    struct tnode** alloc_2(struct tnode * t1, struct tnode * t2);
    struct tnode** alloc_3(struct tnode * t1, struct tnode * t2,  struct tnode * t3);
    struct tnode** alloc_4(struct tnode * t1, struct tnode * t2,  struct tnode * t3,  struct tnode * t4);
    extern FILE *yyin;
    extern FILE *fp;
    extern int SP;
    extern struct Gsymbol * symbolTable;
%}

%union{
    struct tnode * no;
}

%type <no> Program GDeclBlock FDefBlock MainBlock Type
%type <no> GDeclList GDecl GidList Gid
%type <no> Fdef LdeclBlock ParamList Param Body LDecList LDecl  FuncBody
%type <no> IdList
%type <no> slist stmt AsgStmt InputStmt OutputStmt IfStmt WhileStmt DoWhileStmt RepeatStmt ReturnStmt
%type <no> identifier expr

%token <no> START END DECL ENDDECL BRKP MAIN RETURN
%token <no> ID STR INT NUM STRING READ WRITE
%token <no> IF ENDIF ELSE THEN  
%token <no> REPEAT UNTIL WHILE DO ENDWHILE BREAK CONTINUE
%token <no> GE GT LE LT EQ NE
%nonassoc GE GT LE LT EQ NE
%left '+' '-'
%left '*' '/' '%'

%%

/* Grammar rules section */


/* Program Root section */
Program : GDeclBlock FDefBlock MainBlock {
        $$ = createTree(-1, invalidType, "", rootNode,  alloc_3($1, $2, $3) , 3 , NULL, NULL); 
        printTree($$, NULL, 0);
    }
    | GDeclBlock MainBlock {
        $$ = createTree(-1, invalidType, "", rootNode,  alloc_2($1, $2) , 2 , NULL, NULL); 
        printTree($$, NULL, 0);
    }
    | MainBlock {
        $$ = $1;
    }
;

/* Global Declarations */
GDeclBlock : DECL GDeclList ENDDECL { $$ = $2; }
    | DECL ENDDECL { $$ = NULL; }
;
GDeclList : GDeclList GDecl {
        $$ = createTree(-1, invalidType, "", connectorNode,  alloc_2($1, $2) , 2 , NULL, NULL); 
    }
    | GDecl { $$ = $1; }
;
GDecl : Type GidList ';' {
        $$ = createTree(-1, invalidType, "", GDeclNode,  alloc_2($1, $2) , 2 , NULL, NULL); 
        setGTypes( $2, $1->type, NULL );
    }
;
GidList : GidList ',' Gid {
        $$ = createTree(-1, invalidType, "", connectorNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | Gid { $$ = $1; }
;
Gid : ID { 
        $$ = $1; 
    }
    | ID '[' NUM ']'  { 
        $$ = createTree(-1, invalidType, "", arrTypeNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | ID '(' ParamList ')' {
        $$ = createTree(-1, invalidType, "", funcTypeGDeclNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | ID '(' ')' {
        $$ = createTree(-1, invalidType, "", funcTypeGDeclNode,  alloc_2($1, NULL) , 2 , NULL, NULL); 
    }
;

/* Function Definition */
FDefBlock : FDefBlock Fdef {
        $$ = createTree(-1, invalidType, "", connectorNode, alloc_2($1, $2), 2, NULL, NULL );
    }
    | Fdef { $$ = $1; }
;
Fdef : Type ID '(' ParamList ')' '{' FuncBody '}' {
        $$ = createTree(-1, invalidType, "", FDefNode, alloc_4($1, $2, $4, $7), 4, NULL, $7-> Lentry );
        printLSymbolTable($$->Lentry);
    }
    | Type ID '(' ')' '{' FuncBody '}' {
        $$ = createTree(-1, invalidType, "", FDefNode, alloc_4($1, $2, NULL, $6), 4, NULL, $6 -> Lentry);
        printLSymbolTable($$->Lentry);
    }
;
/* Main Function */
MainBlock: INT MAIN '(' ')' '{' FuncBody '}' {
        $$ = createTree(-1, invalidType, "", FDefNode, alloc_4($1, $2, NULL, $6), 4, NULL, $6 -> Lentry);
        printLSymbolTable($$->Lentry);
    }
;
FuncBody : LdeclBlock Body {
        struct Lsymbol * ltemp = NULL;
        if ($1 != NULL ) ltemp = $1->Lentry;
        $$ = createTree(-1, invalidType, "", FBodyNode, alloc_2($1, $2), 2, NULL, ltemp );
    }
    | Body { $$ = createTree(-1, invalidType, "", FBodyNode, alloc_2(NULL, $1), 2, NULL, NULL ); }
;

/* Local Function Declarations */
LdeclBlock : DECL LDecList ENDDECL { $$ = $2; }
    | DECL ENDDECL { $$ = NULL; }
;
LDecList : LDecList LDecl {
        struct Lsymbol * Lentry = joinLsymbols($1->Lentry, $2->Lentry);
        $$ = createTree(-1, invalidType, "", connectorNode, alloc_2($1, $2), 2, NULL, Lentry );
    }
    | LDecl { $$ = $1; }
;
LDecl : Type IdList ';'{
        $$ = createTree(-1, invalidType, "", LDeclNode, alloc_2($1, $2), 2, NULL, NULL);
        setLTypes($1->type, $2, $$);
    }
;
IdList : IdList ',' ID {
        $$ = createTree(-1, invalidType, "", connectorNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | ID { $$ = $1; }
;

/* Parameters in Function Declaration */
ParamList : ParamList ',' Param {
        $$ = createTree(-1, invalidType, "", connectorNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | Param { $$ = $1; }
;
Param : Type ID {
        $$ = createTree(-1, invalidType, "", paramNode,  alloc_2($1, $2) , 2 , NULL, NULL); 
    }
;
Type : INT { $$ = createTree(-1, intType, "", intTypeNode,  NULL , 0, NULL, NULL); }
    | STR { $$ = createTree(-1, strType, "", strTypeNode,  NULL , 0, NULL, NULL); }
;


/* Main / Function Code */
Body: START slist END { $$ = $2; }
;


slist : slist stmt  { $$ = createTree(-1, invalidType, "",connectorNode,  alloc_2($1, $2), 2, NULL, NULL); } 
    | stmt          { $$ = $1; }
;

stmt : AsgStmt      { $$ = $1; } 
    | InputStmt     { $$ = $1; } 
    | OutputStmt    { $$ = $1; }
    | IfStmt        { $$ = $1; }
    | WhileStmt     { $$ = $1; }
    | DoWhileStmt   { $$ = $1; }
    | RepeatStmt    { $$ = $1; }
    | ReturnStmt    { $$ = $1; }
    | BREAK ';'     { $$ = $1; }
    | CONTINUE ';'  { $$ = $1; }
    | BRKP ';'      { $$ = $1; }
;

ReturnStmt: RETURN expr ';' {
        $$ = createTree(-1, invalidType, "", returnNode , alloc_1($2), 1, NULL, NULL);
    }
;

AsgStmt : identifier '=' expr ';' {
        $$ = createTree(-1, invalidType, "", assignNode , alloc_2($1, $3), 2, NULL, NULL);
    }
;

AsgStmt : identifier '=' STRING ';' {
        $$ = createTree(-1, invalidType, "", assignNode , alloc_2($1, $3), 2, NULL, NULL);
    }
;

identifier : ID  { $$ = $1; }
    | ID '[' expr ']' {
        $$ = createTree(-1, intType , "", arrTypeNode,  alloc_2($1, $3), 2 , NULL, NULL);
    }
;


InputStmt : READ '(' identifier ')' ';' {
        $$ = createTree(-1, invalidType, "", readNode, alloc_1($3), 1, NULL, NULL);
    }
;

OutputStmt : WRITE '(' expr ')' ';'{
        $$ = createTree(-1, invalidType, "", writeNode, alloc_1($3), 1, NULL, NULL);
    }
;

OutputStmt : WRITE '(' STRING ')' ';'{
        $$ = createTree(-1, invalidType, "", writeNode, alloc_1($3), 1, NULL, NULL);
    }
;

IfStmt : IF '(' expr ')' THEN slist ELSE slist ENDIF ';' {
        $$ = createTree(-1, invalidType, "", ifNode, alloc_3($3, $6, $8), 3, NULL, NULL);
    }
    | IF '(' expr ')' THEN slist ENDIF ';' {
        $$ = createTree(-1, invalidType, "", ifNode, alloc_3($3, $6, NULL), 3, NULL, NULL);
    }
;

WhileStmt : WHILE '(' expr ')' DO slist ENDWHILE ';' {
        $$ = createTree(-1, invalidType, "", whileNode, alloc_2($3,$6), 2, NULL, NULL);
    }
;

DoWhileStmt : DO slist WHILE '(' expr ')' ';' {
        $$ = createTree(-1, invalidType, "", dowhileNode, alloc_2($2,$5), 2, NULL, NULL);
    }
;

RepeatStmt : REPEAT slist UNTIL '(' expr ')' ';' {
        $$ = createTree(-1, invalidType, "", repeatNode, alloc_2($2,$5), 2, NULL, NULL);
    }
;

expr : '(' expr ')'     {   $$ = $2; }
    | expr '+' expr     {   $$ = createTree(-1, intType , "", addNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr '*' expr     {   $$ = createTree(-1, intType , "", mulNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr '-' expr     {   $$ = createTree(-1, intType , "", subNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr '/' expr     {   $$ = createTree(-1, intType , "", divNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr '%' expr     {   $$ = createTree(-1, intType , "", modNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr GE expr      {   $$ = createTree(-1, boolType , "", geNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr LE expr      {   $$ = createTree(-1, boolType , "", leNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr LT expr      {   $$ = createTree(-1, boolType , "", ltNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr GT expr      {   $$ = createTree(-1, boolType , "", gtNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr NE expr      {   $$ = createTree(-1, boolType , "", neNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr EQ expr      {   $$ = createTree(-1, boolType , "", eqNode, alloc_2($1, $3), 2, NULL, NULL); }
    | '*' expr          {   $$ = createTree(-1, invalidType , "", derefNode, alloc_1($2), 1, NULL, NULL); }
    | NUM               {   $$ = $1; }
    | identifier        {   $$ = createTree(-1, invalidType , "", derefNode, alloc_1($1), 1, NULL, NULL); }
    | '&' identifier    {   $$ = $2; }
    | identifier '(' ')' 
    | identifier '(' ArgList ')'
;

ArgList : ArgList ',' expr 
    | expr
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

struct tnode** alloc_4(struct tnode * t1, struct tnode * t2,  struct tnode * t3, struct tnode * t4){
    struct tnode ** arr = (struct tnode **)malloc(4*sizeof(struct tnode*));
    arr[0] = t1;
    arr[1] = t2;
    arr[2] = t3;
    arr[3] = t4;
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
    // fprintf(fp, "JMP EXIT\n");

    printGSymbolTable();
    printf("YACC: Compiled succesfully (with labels)! \n");

    return 0;
}