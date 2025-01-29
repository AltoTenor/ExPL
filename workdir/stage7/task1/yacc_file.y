%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stddef.h>
    #include "analyser.h"
    #include "codegen.h"
    int yylex(void);
    int yyerror(char const *s);
    struct tnode** alloc_1(struct tnode * t1);
    struct tnode** alloc_2(struct tnode * t1, struct tnode * t2);
    struct tnode** alloc_3(struct tnode * t1, struct tnode * t2,  struct tnode * t3);
    struct tnode** alloc_4( struct tnode * t1, 
                            struct tnode * t2,  
                            struct tnode * t3,  
                            struct tnode * t4);
    struct tnode** alloc_5( struct tnode * t1, 
                            struct tnode * t2,  
                            struct tnode * t3,  
                            struct tnode * t4,
                            struct tnode * t5);
    extern FILE *yyin;
    extern FILE *fp;
    extern int SP;
    extern int globalFlabel;
    extern struct Gsymbol * symbolTable;
%}

%union{
    struct tnode * no;
}

%type <no> Program GDeclBlock FDefBlock MainBlock TypeDefBlock ClassDefBlock
%type <no> ClassDefList ClassDef Cname MethodDecl MethodDefns MDecl
%type <no> GDeclList GDecl GidList Gid
%type <no> Fdef LdeclBlock Body LDecList LDecl FuncBody
%type <no> slist stmt AsgStmt InputStmt OutputStmt IfStmt WhileStmt DoWhileStmt RepeatStmt ReturnStmt
%type <no> identifier expr ArgList IdList ParamList Param Type
%type <no> TypeDefList TypeDef FieldDeclList FieldDecl NewTypeID

%token <no> START END DECL ENDDECL BRKP MAIN RETURN TUPLE TYPE ENDTYPE
%token <no> ID STR INT NUM STRING READ WRITE INITIALIZE ALLOC NULL_KEYWORD FREE
%token <no> IF ENDIF ELSE THEN  
%token <no> CLASS ENDCLASS EXTENDS
%token <no> REPEAT UNTIL WHILE DO ENDWHILE BREAK CONTINUE
%token <no> GE GT LE LT EQ NE AND OR
%left AND OR 
%nonassoc GE GT LE LT EQ NE 
%left '+' '-'
%left '*' '/' '%'

%%

/* Grammar rules section */


/* Program Root section */
Program : TypeDefBlock ClassDefBlock GDeclBlock FDefBlock MainBlock {          
        $$ = createTree(NULL, NULL, "void", rootNode,  alloc_5($1, $2, $3, $4, $5) , 5 , NULL, NULL); 
        printTypeTable();
        printTree($$, NULL, 0);

        initialize();
        struct Context * c = (struct Context *)malloc(sizeof(struct Context));
        c->jumpLabels = (int *)malloc(sizeof(int)*2);
        c->mainFunc = 0;
        codeGen($4, c);
        c->mainFunc = 1;
        codeGen($5, c);
    }
    | TypeDefBlock ClassDefBlock GDeclBlock MainBlock {
        $$ = createTree(NULL, NULL, "void", rootNode,  alloc_4($1, $2, $3, $4) , 4 , NULL, NULL); 
        printTypeTable();
        printTree($$, NULL, 0);
        
        initialize();
        struct Context * c = (struct Context *)malloc(sizeof(struct Context));
        c->jumpLabels = (int *)malloc(sizeof(int)*2);
        c->mainFunc = 1;
        codeGen($4, c);
    }
;

/* Type Definitions */
TypeDefBlock : TYPE TypeDefList ENDTYPE { $$ = $2; }
    | { $$ = NULL; }
;
TypeDefList : TypeDefList TypeDef {
        $$ = createTree(NULL, NULL, "void", connectorNode,  alloc_2($1, $2) , 2 , NULL, NULL);
    }
    | TypeDef { $$ = $1; }
;
TypeDef : NewTypeID '{' FieldDeclList '}' {
        $$ = createTree(NULL, NULL, "void", typeDefNode,  alloc_2($1, $3) , 2 , NULL, NULL);
        setUserDefType($$);
    }
;
NewTypeID: ID { TInstall($1->name, 1, NULL, USER_DEF); $$ = $1; }
;
FieldDeclList : FieldDeclList FieldDecl {
        $$ = createTree(NULL, NULL, "void", connectorNode,  alloc_2($1, $2) , 2 , NULL, NULL);
    }   
    | FieldDecl { $$ = $1; }
;
FieldDecl : Type ID ';' {
        $$ = createTree(NULL, NULL, "void", paramNode,  alloc_2($1, $2) , 2 , NULL, NULL);
    }
;

/* Class Declarations */
ClassDefBlock : CLASS ClassDefList ENDCLASS 
    | { $$ = NULL; }
;
ClassDefList : ClassDefList ClassDef
    | ClassDef
;
ClassDef : Cname '{' DECL FieldDeclList MethodDecl ENDDECL MethodDefns '}'
;
Cname : ID { }
    | ID EXTENDS ID { }
;
MethodDecl : MethodDecl MDecl {}
    | MDecl {}
;
MDecl : Type ID '(' ParamList ')' ';' { }
    | Type ID '(' ')' ';' {}
;
MethodDefns : MethodDefns Fdef { }
    | Fdef { }
;

/* Global Declarations */
GDeclBlock : DECL GDeclList ENDDECL { $$ = $2; }
    | { $$ = NULL; }
;
GDeclList : GDeclList GDecl {
        $$ = createTree(NULL, NULL, "void", connectorNode,  alloc_2($1, $2) , 2 , NULL, NULL); 
    }
    | GDecl { $$ = $1; }
;
GDecl : Type GidList ';' {
        $$ = createTree(NULL, NULL, "void", GDeclNode,  alloc_2($1, $2) , 2 , NULL, NULL); 
        setGTypes( $2, $1->type, NULL );
    }
    | TUPLE ID '(' ParamList ')' IdList ';'{
        $$ = createTree(NULL, NULL, "void", tupleNode, alloc_3($2, $4, $6), 3, NULL, NULL);
        initTuple($$->children, NULL);
    }
;
GidList : GidList ',' Gid {
        $$ = createTree(NULL, NULL, "void", connectorNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | Gid { $$ = $1; }
;
Gid : ID { $$ = $1; }
    | '*' ID { 
        $$ = createTree(NULL, NULL, "void", ptrNode,  alloc_1($2) , 1 , NULL, NULL); 
    }
    | ID '[' NUM ']'  { 
        $$ = createTree(NULL, NULL, "void", arrTypeNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | ID '(' ParamList ')' {
        $$ = createTree(NULL, NULL, "void", funcTypeGDeclNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | ID '(' ')' {
        $$ = createTree(NULL, NULL, "void", funcTypeGDeclNode,  alloc_2($1, NULL) , 2 , NULL, NULL); 
    }
    | '*' ID '(' ParamList ')' {
        $$ = createTree(NULL, NULL, "void", funcPtrTypeGDeclNode,  alloc_2($2, $4) , 2 , NULL, NULL); 
    }
    | '*' ID '(' ')' {
        $$ = createTree(NULL, NULL, "void", funcPtrTypeGDeclNode,  alloc_2($2, NULL) , 2 , NULL, NULL); 
    }
;

/* Function Definition */
FDefBlock : FDefBlock Fdef {
        $$ = createTree(NULL, NULL, "void", connectorNode, alloc_2($1, $2), 2, NULL, NULL );
    }
    | Fdef { $$ = $1; }
;
Fdef : Type ID '(' ParamList ')' '{' FuncBody '}' {
        $$ = createTree(NULL, NULL, "void", FDefNode, alloc_4($1, $2, $4, $7), 4, NULL, $7-> Lentry );
        printf("LST %s function\n", $2->name);
        printLSymbolTable($$->Lentry);
    }
    | Type ID '(' ')' '{' FuncBody '}' {
        $$ = createTree(NULL, NULL, "void", FDefNode, alloc_4($1, $2, NULL, $6), 4, NULL, $6 -> Lentry);
        printf("LST %s function\n", $2->name);
        printLSymbolTable($$->Lentry);
    }
    | Type '*' ID '(' ParamList ')' '{' FuncBody '}' {
        $1->type = make_pointer($1->type);
        $$ = createTree(NULL, NULL, "void", FDefNode, alloc_4($1, $3, $5, $8), 4, NULL, $8-> Lentry );
        printf("LST %s function\n", $3->name);
        printLSymbolTable($$->Lentry);
    }
    | Type '*' ID '(' ')' '{' FuncBody '}' {
        $1->type = make_pointer($1->type);
        $$ = createTree(NULL, NULL, "void", FDefNode, alloc_4($1, $3, NULL, $7), 4, NULL, $7 -> Lentry);
        printf("LST %s function\n", $3->name);
        printLSymbolTable($$->Lentry);
    }
;
/* Main Function */
MainBlock: Type MAIN '(' ')' '{' FuncBody '}' {
        if ( $1->type != TLookup("int") ){ printf("Main not INT\n"); exit(1);}
        $$ = createTree(NULL, NULL, "void", FDefNode, alloc_4($1, $2, NULL, $6), 4, NULL, $6 -> Lentry);
        printf("LST %s function\n", $2->name);
        printLSymbolTable($$->Lentry);
    }
;
FuncBody : LdeclBlock Body {
        struct Lsymbol * ltemp = NULL;
        if ($1 != NULL ) ltemp = $1->Lentry;
        $$ = createTree(NULL, NULL, "void", FBodyNode, alloc_2($1, $2), 2, NULL, ltemp );
    }
    | Body { $$ = createTree(NULL, NULL, "void", FBodyNode, alloc_2(NULL, $1), 2, NULL, NULL ); }
;

/* Local Function Declarations */
LdeclBlock : DECL LDecList ENDDECL { $$ = $2; }
    | DECL ENDDECL { $$ = NULL; }
;
LDecList : LDecList LDecl {
        struct Lsymbol * Lentry = joinLsymbols($1->Lentry, $2->Lentry);
        $$ = createTree(NULL, NULL, "void", connectorNode, alloc_2($1, $2), 2, NULL, Lentry );
    }
    | LDecl { $$ = $1; }
;
LDecl : Type IdList ';'{
        $$ = createTree(NULL, NULL, "void", LDeclNode, alloc_2($1, $2), 2, NULL, NULL);
        setLTypes($1->type, $2, $$);
    }
    | TUPLE ID '(' ParamList ')' IdList ';'{
        $$ = createTree(NULL, NULL, "void", tupleNode, alloc_3($2, $4, $6), 3, NULL, NULL);
        initTuple($$->children, $$);
    }
;
IdList : IdList ',' ID {
        $$ = createTree(NULL, NULL, "void", connectorNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | IdList ',' '*' ID {
        struct tnode * t = createTree(NULL, NULL, "void", ptrNode, alloc_1($4) , 1, NULL, NULL); 
        $$ = createTree(NULL, NULL, "void", connectorNode,  alloc_2($1, t) , 2 , NULL, NULL); 
    }
    | '*' ID { 
        $$ = createTree(NULL, NULL, "void", ptrNode,  alloc_1($2) , 1 , NULL, NULL); 
    }
    | ID { $$ = $1; }
;

/* Parameters in Function Declaration */
ParamList : ParamList ',' Param {
        $$ = createTree(NULL, NULL, "void", connectorNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
    | Param { $$ = $1; }
;
Param : Type ID {
        $$ = createTree(NULL, NULL, "void", paramNode,  alloc_2($1, $2) , 2 , NULL, NULL); 
    }
    | Type '*' ID {
        $1->type = make_pointer($1->type);
        $$ = createTree(NULL, NULL, "void", paramNode,  alloc_2($1, $3) , 2 , NULL, NULL); 
    }
;
Type : INT { $$ = createTree(NULL, NULL, "int", typeNode, NULL , 0, NULL, NULL); }
    | STR { $$ = createTree(NULL, NULL, "str", typeNode, NULL , 0, NULL, NULL); }
    | ID { $$ = createTree(NULL, NULL, $1->name, typeNode, alloc_1($1) , 1, NULL, NULL); }
;


/* Main / Function Code */
Body: START slist END { $$ = $2; }
;


slist : slist stmt  { $$ = createTree(NULL, NULL, "void",connectorNode,  alloc_2($1, $2), 2, NULL, NULL); } 
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
        $$ = createTree(NULL, NULL, "void", returnNode , alloc_1($2), 1, NULL, NULL);
    }
    | RETURN STRING ';' {
        $$ = createTree(NULL, NULL, "void", returnNode , alloc_1($2), 1, NULL, NULL);
    }
;

AsgStmt : identifier '=' expr ';' {
        $$ = createTree(NULL, NULL, "void", assignNode , alloc_2($1, $3), 2, NULL, NULL);
    }
    | '*' identifier '=' expr ';' {
        printf("%s\n", $2->type->name);
        struct tnode * t = createTree(NULL, NULL, "void", derefOpNode, alloc_1($2), 1, NULL, NULL);
        $$ = createTree(NULL, NULL, "void", assignNode, alloc_2(t, $4), 2, NULL, NULL);
    }
    | identifier '=' STRING ';' {
        $$ = createTree(NULL, NULL, "void", assignNode , alloc_2($1, $3), 2, NULL, NULL);
    }
    | '*' identifier '=' STRING ';' {
        printf("%s\n", $2->type->name);
        struct tnode * t = createTree(NULL, NULL, "void", derefOpNode, alloc_1($2), 1, NULL, NULL);
        $$ = createTree(NULL, NULL, "void", assignNode, alloc_2(t, $4), 2, NULL, NULL);
    }
;

identifier : ID  { $$ = $1; }
    | ID '[' expr ']' {
        $$ = createTree(NULL, NULL, "int", arrTypeNode,  alloc_2($1, $3), 2 , NULL, NULL);
    }
    | identifier '.' ID {
        $$ = createTree(NULL, NULL, "void", memberNode,  alloc_2($1, $3), 2 , NULL, NULL);
    }
;


InputStmt : READ '(' identifier ')' ';' {
        $$ = createTree(NULL, NULL, "void", readNode, alloc_1($3), 1, NULL, NULL);
    }
;

OutputStmt : WRITE '(' expr ')' ';'{
        $$ = createTree(NULL, NULL, "void", writeNode, alloc_1($3), 1, NULL, NULL);
    }
    | WRITE '(' STRING ')' ';'{
        $$ = createTree(NULL, NULL, "void", writeNode, alloc_1($3), 1, NULL, NULL);
    }
;

IfStmt : IF '(' expr ')' THEN slist ELSE slist ENDIF ';' {
        $$ = createTree(NULL, NULL, "void", ifNode, alloc_3($3, $6, $8), 3, NULL, NULL);
    }
    | IF '(' expr ')' THEN slist ENDIF ';' {
        $$ = createTree(NULL, NULL, "void", ifNode, alloc_3($3, $6, NULL), 3, NULL, NULL);
    }
;

WhileStmt : WHILE '(' expr ')' DO slist ENDWHILE ';' {
        $$ = createTree(NULL, NULL, "void", whileNode, alloc_2($3,$6), 2, NULL, NULL);
    }
;

DoWhileStmt : DO slist WHILE '(' expr ')' ';' {
        $$ = createTree(NULL, NULL, "void", dowhileNode, alloc_2($2,$5), 2, NULL, NULL);
    }
;

RepeatStmt : REPEAT slist UNTIL '(' expr ')' ';' {
        $$ = createTree(NULL, NULL, "void", repeatNode, alloc_2($2,$5), 2, NULL, NULL);
    }
;

expr : '(' expr ')'     {   $$ = $2; }
    | expr '+' expr     {   $$ = createTree(NULL, NULL, "void", addNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr '*' expr     {   $$ = createTree(NULL, NULL, "int", mulNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr '-' expr     {   $$ = createTree(NULL, NULL, "void", subNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr '/' expr     {   $$ = createTree(NULL, NULL, "int", divNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr '%' expr     {   $$ = createTree(NULL, NULL, "int", modNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr GE expr      {   $$ = createTree(NULL, NULL, "boolean", geNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr LE expr      {   $$ = createTree(NULL, NULL, "boolean", leNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr LT expr      {   $$ = createTree(NULL, NULL, "boolean", ltNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr GT expr      {   $$ = createTree(NULL, NULL, "boolean", gtNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr NE expr      {   $$ = createTree(NULL, NULL, "boolean", neNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr EQ expr      {   $$ = createTree(NULL, NULL, "boolean", eqNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr AND expr     {   $$ = createTree(NULL, NULL, "boolean", andNode, alloc_2($1, $3), 2, NULL, NULL); }
    | expr OR expr      {   $$ = createTree(NULL, NULL, "boolean", orNode, alloc_2($1, $3), 2, NULL, NULL); }
    | '*' expr          {   $$ = createTree(NULL, NULL, "void", derefOpNode, alloc_1($2), 1, NULL, NULL); }
    | NUM               {   $$ = $1; }
    | identifier        {   $$ = createTree(NULL, NULL, "void", exprNode, alloc_1($1), 1, NULL, NULL); }
    | '&' identifier    {   $$ = createTree(NULL, NULL, "void", addrNode, alloc_1($2), 1, NULL, NULL); }
    | identifier '(' ')' {
        $$ = createTree(NULL, NULL, "void", funcCallNode, alloc_2($1, NULL), 2, NULL, NULL);
    }
    | identifier '(' ArgList ')' {
        $$ = createTree(NULL, NULL, "void", funcCallNode, alloc_2($1, $3), 2, NULL, NULL);
    }
    | INITIALIZE '(' ')' { $$ = createTree(NULL, NULL, "int", initNode, NULL, 0, NULL, NULL); }
    | ALLOC '(' ')'      { $$ = createTree(NULL, NULL, "int", allocNode, NULL, 0, NULL, NULL); }
    | NULL_KEYWORD       { $$ = createTree(NULL, NULL, "null", nullNode, NULL, 0, NULL, NULL); }
    | FREE '(' ID ')'    { $$ = createTree(NULL, NULL, "int", freeNode, alloc_1($3), 1, NULL, NULL); }
;

ArgList : ArgList ',' expr {
        $$ = createTree( NULL, NULL, "void", argNode, alloc_2($1, $3), 2, NULL, NULL );
    }
    | expr {
        $$ = createTree( NULL, NULL, "void", argNode, alloc_1($1), 1, NULL, NULL );
    }
;

%%

int yyerror(char const *s)
{
    printf("YYerror: %s\n",s);
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

struct tnode** alloc_5( struct tnode * t1, 
                        struct tnode * t2,  
                        struct tnode * t3, 
                        struct tnode * t4,
                        struct tnode * t5){
    struct tnode ** arr = (struct tnode **)malloc(5*sizeof(struct tnode*));
    arr[0] = t1;
    arr[1] = t2;
    arr[2] = t3;
    arr[3] = t4;
    arr[4] = t5;
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
    globalFlabel = 0;
    TypeTableCreate();
    yyparse();

    printGSymbolTable();
    printf("YACC: Compiled succesfully (with labels)! \n");

    return 0;
}