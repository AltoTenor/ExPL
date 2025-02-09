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
}

%type <no> Program GDeclBlock FDefBlock MainBlock Type
%type <no> GDeclList GDecl GidList Gid
%type <no> Fdef LdeclBlock ParamList Param Body LDecList LDecl  
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

Program : GDeclBlock FDefBlock MainBlock {
        $$ = createTree(-1, invalidType, "", rootNode,  alloc_3($1, $2, $3) , 3 , NULL); 
    }
    | GDeclBlock MainBlock {
        $$ = createTree(-1, invalidType, "", rootNode,  alloc_2($1, NULL) , 2 , NULL); 
        printTree($2, NULL);
    }
    | MainBlock {
        $$ = $1;
    }
;


GDeclBlock : DECL GDeclList ENDDECL { $$ = $2; }
    | DECL ENDDECL { $$ = NULL; }
;

GDeclList : GDeclList GDecl {
        $$ = createTree(-1, invalidType, "", connectorNode,  alloc_2($1, $2) , 2 , NULL); 
    }
    | GDecl { $$ = $1; }
;
GDecl : Type GidList ';' {
        setTypes( $2, $1->type, NULL );
        $$ = createTree(-1, invalidType, "", GDeclNode,  alloc_2($1, $2) , 2 , NULL); 
    }
;

GidList : GidList ',' Gid {
        $$ = createTree(-1, invalidType, "", connectorNode,  alloc_2($1, $3) , 2 , NULL); 
    }
    | Gid { $$ = $1; }
;

Gid : ID { 
        $$ = $1; 
    }
    | ID '[' NUM ']'  { 
        $$ = createTree(-1, invalidType, "", arrTypeNode,  alloc_2($1, $3) , 2 , NULL); 
    }
    | ID '(' ParamList ')' {
        $$ = createTree(-1, invalidType, "", funcTypeGDeclNode,  alloc_2($1, $3) , 2 , NULL); 
    }
;


FDefBlock : FDefBlock Fdef 
    | Fdef
;

Fdef : Type ID '(' ParamList ')' '{' FuncBody '}'
    | Type ID '(' ')' '{' FuncBody '}'
;

FuncBody : LdeclBlock Body 
    | Body
;


LdeclBlock : DECL LDecList ENDDECL 
    | DECL ENDDECL
;
LDecList : LDecList LDecl 
    | LDecl
;

LDecl : Type IdList ';'{
    
    }
;

IdList : IdList ',' ID {
        $$ = createTree(-1, invalidType, "", connectorNode,  alloc_2($1, $3) , 2 , NULL); 
    }
    | ID { $$ = $1; }
;


ParamList : ParamList ',' Param {
        $$ = createTree(-1, invalidType, "", connectorNode,  alloc_2($1, $3) , 2 , NULL); 
    }
    | Param { $$ = $1; }
;

Param : Type ID {
        $$ = createTree(-1, invalidType, "", paramNode,  alloc_2($1, $2) , 2 , NULL); 
    }
;

Type : INT { $$ = createTree(-1, intType, "", intTypeNode,  NULL , 0, NULL); }
    | STR { $$ = createTree(-1, strType, "", strTypeNode,  NULL , 0, NULL); }
;


MainBlock: INT MAIN '(' ')' '{' FuncBody '}' {

    }
;

Body: START slist END {

    }
;


slist : slist stmt  {  } 
    | stmt          {  }
;

stmt : AsgStmt      {  } 
    | InputStmt     {  } 
    | OutputStmt    {  }
    | IfStmt        {  }
    | WhileStmt     {  }
    | DoWhileStmt   {  }
    | RepeatStmt    {  }
    | ReturnStmt    {  }
    | BREAK ';'     {  }
    | CONTINUE ';'  {  }
    | BRKP ';'      {  }
;

ReturnStmt: RETURN expr ';' {

    }
;

AsgStmt : identifier '=' expr ';' {
            // $$ = createTree(-1, invalidType, "", assignNode , alloc_2($1, $3), 2, NULL);
        }
;

AsgStmt : identifier '=' STRING ';' {
            // $$ = createTree(-1, invalidType, "", assignNode , alloc_2($1, $3), 2, NULL);
        }
;

identifier : ID { 
    }
    | ID '[' expr ']' {   
    }
    | ID '[' expr ']' '[' expr ']' {
    }
;


InputStmt : READ '(' identifier ')' ';' {
            // $$ = createTree(-1, invalidType, "", readNode, alloc_1($3), 1, NULL);
        }
        
;

OutputStmt : WRITE '(' expr ')' ';'{
            // $$ = createTree(-1, invalidType, "", writeNode, alloc_1($3), 1, NULL);
        }
;

OutputStmt : WRITE '(' STRING ')' ';'{
            // $$ = createTree(-1, invalidType, "", writeNode, alloc_1($3), 1, NULL);
        }
;

IfStmt : IF '(' expr ')' THEN slist ELSE slist ENDIF ';' {
            // $$ = createTree(-1, invalidType, "", ifNode, alloc_3($3, $6, $8), 3, NULL);
        }
    | IF '(' expr ')' THEN slist ENDIF ';' {
            // $$ = createTree(-1, invalidType, "", ifNode, alloc_3($3, $6, NULL), 3, NULL);
        }
;

WhileStmt : WHILE '(' expr ')' DO slist ENDWHILE ';' {
            // $$ = createTree(-1, invalidType, "", whileNode, alloc_2($3,$6), 2, NULL);
        }
;

DoWhileStmt : DO slist WHILE '(' expr ')' ';' {
            // $$ = createTree(-1, invalidType, "", dowhileNode, alloc_2($2,$5), 2, NULL);
        }
;

RepeatStmt : REPEAT slist UNTIL '(' expr ')' ';' {
            // $$ = createTree(-1, invalidType, "", repeatNode, alloc_2($2,$5), 2, NULL);
        }
;

expr :  '(' expr ')' {

    }
    | expr '+' expr    {   
    // $$ = createTree(-1, intType , "", addNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr '*' expr     {   
        // $$ = createTree(-1, intType , "", mulNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr '-' expr     {   
        // $$ = createTree(-1, intType , "", subNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr '/' expr     {   
        // $$ = createTree(-1, intType , "", divNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr '%' expr     {   
        // $$ = createTree(-1, intType , "", modNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr GE expr      {   
        // $$ = createTree(-1, boolType , "", geNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr LE expr      {   
        // $$ = createTree(-1, boolType , "", leNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr LT expr      {   
        // $$ = createTree(-1, boolType , "", ltNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr GT expr      {   
        // $$ = createTree(-1, boolType , "", gtNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr NE expr      {   
        // $$ = createTree(-1, boolType , "", neNode, alloc_2($1, $3), 2, NULL); 
    }
    | expr EQ expr      {   
        // $$ = createTree(-1, boolType , "", eqNode, alloc_2($1, $3), 2, NULL); 
    }
    | '*' expr          {   
        // $$ = createTree(-1, $2->type , "", derefNode, alloc_1($2), 1, NULL); 
    }
    | NUM               {  $$ = $1;  }
    | identifier        {   
        // $$ = createTree(-1, $1->Gentry->type , "", derefNode, alloc_1($1), 1, NULL); 
    }
    | '&' identifier    { }
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

    printSymbolTable();
    printf("YACC: Compiled succesfully (with labels)! \n");

    return 0;
}