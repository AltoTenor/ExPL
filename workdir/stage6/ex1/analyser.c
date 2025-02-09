#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "analyser.h"

struct Gsymbol * symbolTable;
struct Typetable * typeTable;
int SP;
int *arr;
int globalFlabel;

/*Creates and initializes a new node in the expression tree*/
struct tnode* createTree(   char* name,  
                            union Constant * c,  
                            char* type, 
                            int nodetype, 
                            struct tnode **children, 
                            int childcount,
                            struct Gsymbol * Gentry,
                            struct Lsymbol * Lentry  ){
    
    struct tnode *temp;
    
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->Gentry = Gentry;

    temp->type = TLookup(type);
    if ( nodetype==typeNode && temp->type == NULL ){
        printf("Invalid Type Used: %s\n", type);
        exit(1);
    }
    temp->childcount = childcount;
    temp->nodetype = nodetype;
    switch (nodetype){
        case numNode:       temp->value.intval = c->intval;
                            break;

        case strConstNode:  temp->value.strval = (char*)malloc(sizeof(char)*30); 
                            strcpy(temp->value.strval, c->strval);
                            break;

        case idNode:        temp->name = (char*)malloc(20*sizeof(char));
                            strcpy(temp->name, name);
                            break;

                            break;
        case FDefNode:{      //Checks Function Decl match with Function Def
                            struct Gsymbol * funcEntry = GLookup(children[1]->name);
                            if ( strcmp(children[1]->name, "main")!=0 ){
                                if ( funcEntry == NULL  ) {
                                    printf("Declare Function in Header: %s\n", children[1]->name);
                                    exit(1);
                                }
                                checkFDef(children, funcEntry);
                            }
                            temp->Gentry = funcEntry;
                            children[1]->Gentry = funcEntry;
                            // Add parameters to local symbol table
                        
                            if (children[2] != NULL){
                                Lentry = addParamListToLsymbolTable(funcEntry->paramlist, Lentry);
                            }

                            addBindingAddr(Lentry);

                            // Make LST accessible to all Body and skip the Local Declarations
                            if ( strcmp(children[1]->name, "main")!=0 )
                                assignVarTypes(children[3]->children[1], Lentry, funcEntry->type);
                            else
                                assignVarTypes(children[3]->children[1], Lentry, TLookup("int"));
                            break;
                        }
    }
    temp->children = children;
    temp->Lentry = Lentry;
    return temp;
}

// ------------------GLOBAL SYMBOL TABLE------------------------------------------------------------

void GInstall(char *name, struct Typetable * type, int size, int fl){

    // Error when duplicate variable names
    if ( GLookup(name) != NULL ){ 
        printf("Variable redeclared!\n"); 
        exit(1); 
    }

    struct Gsymbol * temp = (struct Gsymbol *)malloc(sizeof(struct Gsymbol));
    temp->name = (char*)malloc(32*sizeof(char));
    strcpy(temp->name, name);
    temp->size = size;
    temp->type = type;
    temp->flabel = fl;
    temp->next = NULL;
    temp->paramlist = NULL;
    temp->binding = SP;
    SP += size;

    // Inserting in a linked list
    if ( symbolTable == NULL ){
        symbolTable = temp;
    }
    else {
        struct Gsymbol * tail = symbolTable;
        while ( tail->next != NULL ){ 
            tail = tail->next;
        }
        tail->next = temp;
    }
}

struct Gsymbol *GLookup(char * name){
    struct Gsymbol * ptr = symbolTable;
    while ( ptr ){
        if ( strcmp(name, ptr->name) == 0 ) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

void setGTypes(struct tnode* t, struct Typetable * type, struct Gsymbol* paramHead){
    if ( t->nodetype == connectorNode ){
        setGTypes( t -> children[0], type, paramHead);
        setGTypes( t -> children[1], type, paramHead);
    }
    else if( t->nodetype == idNode ){
        // For Dynamic Allocation we only storing the pointer
        if ( type->generalType == USER_DEF ) GInstall( t->name, type, 1, -1);
        // For Static ALlocation we need space = size
        else GInstall( t->name, type, type->size, -1);
    }
    else if( t->nodetype == ptrNode ){
        GInstall( t->children[0]->name, make_pointer(type), 1, -1);
    }
    else if( t->nodetype == arrTypeNode ){
        // No matter static/dynamic we store the size
        GInstall( t->children[0]->name, type, t->children[1]->value.intval, -1);
    }
    else if( t->nodetype == funcTypeGDeclNode ){
        GInstall( t->children[0]->name, type, 0, globalFlabel++);
        if ( t->children[1] ) setGTypes( t -> children[1], type, GLookup(t->children[0]->name));
    }
    else if( t->nodetype == funcPtrTypeGDeclNode ){
        GInstall( t->children[0]->name, make_pointer(type), 0, globalFlabel++);
        if ( t->children[1] ) setGTypes( t -> children[1], type, GLookup(t->children[0]->name));
    }
    else if( t->nodetype == paramNode ){
        struct Paramstruct * param = (struct Paramstruct*)malloc(sizeof(struct Paramstruct));
        param->name = (char*)malloc(sizeof(char)*20);
        strcpy(param->name, t->children[1]->name);
        param->type = t->children[0]->type;
        param->checked = 0;
        param->next = NULL;
        insertIntoParamList(paramHead, param);
    }
    else{
        printf("Invalid variable in declarations!\n");
        exit(1);
    }
}

void printGSymbolTable(){
    struct Gsymbol * g = symbolTable;
    printf("\nGlobal Symbol Table: \n");
    printf("| name     | type   | size | binding | flabel | paramlist\n");
    printf("-------------------------------------------------------\n");
    while(g){
        printX(g->name, 11, -1,  1);
        printX(g->type->name, 9, -1, 1);
        printX("", 6, g->size, 2);
        printX("", 6, g->binding, 2);
        printX("", 8, g->flabel, 2);
        struct Paramstruct* temp = g->paramlist;
        while(temp){
            printf("| (%s,%s)", temp->name, temp->type->name);
            temp = temp->next;
        }
        printf("\n");
        g = g->next;
    }
}


// ------------------------LOCAL SYMBOL TABLE-------------------------------------------------------

struct Lsymbol * LInstall(char *name, struct Typetable* type, struct tnode* head){

    struct Lsymbol * cur = (struct Lsymbol *)malloc(sizeof(struct Lsymbol));
    cur->name = (char*)malloc(sizeof(char)*20);
    strcpy(cur->name, name);
    cur->type = type;
    cur->binding = -1;
    cur->next = NULL;

    if ( head->Lentry == NULL ){
        head -> Lentry = cur;
    }
    else{
        struct Lsymbol* temp = head->Lentry;
        while ( temp->next ){
            temp = temp->next;
        }
        temp -> next = cur;
    }

    return cur;
}

struct Lsymbol * LLookup(char * name, struct Lsymbol * head){
    struct Lsymbol * temp = head;
    while (temp){
        if ( strcmp(temp->name, name) == 0 ) return temp;
        temp = temp->next;
    }
    return NULL;
}

void setLTypes(struct Typetable* type, struct tnode * t, struct tnode* head){
    if ( t->nodetype == connectorNode ){
        setLTypes( type, t -> children[0], head);
        setLTypes( type, t -> children[1], head);
    }
    else if ( t->nodetype == idNode ){
        struct Lsymbol * l = LInstall( t->name, type, head );
    }
    else if ( t->nodetype == ptrNode ){
        LInstall( t->children[0]->name, make_pointer(type), head);
        t->children[0]->Lentry = LLookup(t->children[0]->name, head->Lentry);
    }
}

void printLSymbolTable(struct Lsymbol * l ){
    struct Lsymbol * t = l;
    printf("| name   | type  | binding \n");
    printf("---------------------------\n");
    while(t){
        printX(t->name, 9, -1,  1);
        printX(t->type->name, 8, -1,  1);
        printX("", 10, t->binding,  2);
        printf("\n");
        t = t->next;
    }
    printf("\n");
}

struct Lsymbol * joinLsymbols(struct Lsymbol * Lentry1, struct Lsymbol * Lentry2){
    struct Lsymbol * t = Lentry1;
    while (t->next){ t = t->next; }
    t->next = Lentry2;
    return Lentry1;
}

void addBindingAddr(struct Lsymbol* Lentry){
    int addr = 1;
    struct Lsymbol* t = Lentry;
    while (t){
        t->binding = addr;
        // For Dynamic Types we only need 1 word to store a pointer
        if ( t->type->generalType == USER_DEF ) addr++;
        // For Static Types we need space to store the entire thing
        else addr += t->type->size;
        t = t->next;
    }
}

// ---------------------------TYPE TABLE------------------------------------------------------------

struct Typetable* TInstall(char *name, int size, struct Fieldlist *fields, GeneralType gt){
    struct Typetable* temp = (struct Typetable*)malloc(sizeof(struct Typetable));
    
    temp->name = (char *)malloc(sizeof(char)*20);
    strcpy( temp->name, name );
    temp->size = size;
    temp->fields = fields;
    temp->generalType = gt;
    temp->next = NULL;


    if ( typeTable == NULL ) typeTable = temp;
    else{
        struct Typetable* tail = typeTable;
        while (tail->next) tail = tail->next;
        tail->next = temp;
    }

    return temp;
}

struct Typetable* TLookup(char *name){
    if ( name == NULL ) return NULL;
    struct Typetable* t = typeTable;
    while(t){
        if ( strcmp(t->name , name ) == 0 ){
            return t;
        }
        t = t->next;
    }
    return NULL;
}

void TypeTableCreate(){
    typeTable = NULL;
    TInstall("int",1 , NULL, PRIMITIVE);
    TInstall("str",1 , NULL, PRIMITIVE);
    TInstall("boolean",1 , NULL, PRIMITIVE);
    TInstall("null",0 , NULL, PRIMITIVE);
    TInstall("void",0 , NULL, PRIMITIVE);
}

struct Typetable* make_pointer(struct Typetable* type){
    char newPtrType[30] = "pointer_";
    strcat(newPtrType, type->name);
    struct Typetable* tt = TLookup(newPtrType);
    if ( tt == NULL ){
        struct Fieldlist * f = (struct Fieldlist*)malloc(sizeof(struct Fieldlist));
        f->type = type;
        f->fieldIndex = 0;
        TInstall(newPtrType, 1, f, POINTER);
        tt = TLookup(newPtrType);
    }
    return tt;
}

struct Typetable* setUserDefType(struct tnode* t){
    struct Typetable* newType = TLookup(t->children[0]->name);
    if (newType == NULL ){
        printf("Improper definition of type\n Please redeclare.\n");
        exit(1);
    }
    struct Fieldlist *temp, * f = fetchFieldList(t->children[1]);
    temp = f; int size = 0;
    newType->fields = f;
    while(temp){ size++;  temp = temp->next; }
    newType->size = size;
    if ( size > 8 ){
        printf("Too many member fields (>8) for: %s\n",t->children[0]->name ); exit(1);
    }
    return newType;
}

void printTypeTable(){
    struct Typetable* temp = typeTable;
    printf("TypeTable \n");
    printf("| TypeName | Size | FieldList \n");
    while (temp){
        printX(temp->name, 11, -1, 1);
        printX("", 6, temp->size, 2);
        printf("|");
        struct Fieldlist * f = temp->fields;
        while(f){
            printX(f->name, 1, -1, 1);
            printX(f->type->name, 1, -1, 1);
            printX("", 1, f->fieldIndex, 2);
            printf(" |");
            f = f->next; 
        }
        printf("\n");
        temp = temp->next; 
    }
    printf("\n");
    printf("\n");
}

struct Fieldlist * fetchFieldList(struct tnode * t){
    if ( t->nodetype == paramNode ){
        struct Fieldlist * f = (struct Fieldlist * )malloc(sizeof(struct Fieldlist));
        f->name = (char*)malloc(sizeof(char)*20);
        strcpy(f->name, t->children[1]->name);
        f->type = t->children[0]->type;
        f->fieldIndex = 0;
        return f;
    }
    else {
        struct Fieldlist * f1 = fetchFieldList(t->children[0]);
        struct Fieldlist * f2 = fetchFieldList(t->children[1]);

        struct Fieldlist * t = f1;
        int i = 0;
        while ( t->next ) {t->fieldIndex = i++; t = t->next; }
        t->next = f2;
        while( t ) {t->fieldIndex = i++; t = t->next; }
        return f1;
    }
}

struct Fieldlist * FLookup(struct Typetable* type, char * name){
    if (   name == NULL 
        || type == NULL 
        || type->fields == NULL 
        || type->fields->name == NULL ) return NULL;

    struct Fieldlist * f = type->fields;
    while( f ){
        if ( strcmp(f->name, name) == 0 ) return f;
        f=f->next;
    }
    return NULL;
}

// ------------------TUPLES-------------------------------------------------------------------------

void initTuple(struct tnode **children, struct tnode * LSThead){
    struct Fieldlist *t, * f = fetchFieldList(children[1]);
    t = f;
    int size = 0;
    while(t){
        // printf("%s ", t->name);
        // printf("%d ", t->fieldIndex);
        size++; 
        t = t->next; 
    }
    // Try to add checks for field list is same or not
    struct Typetable* type = TLookup(children[0]->name);
    if (type == NULL ) type = TInstall( children[0]->name, size, f, TYPE_TUPLE);

    if ( LSThead == NULL ) setTupleIDinGST(children[2], type);
    else setTupleIDinLST(children[2], type, LSThead);
}

void setTupleIDinGST(struct tnode * t, struct Typetable* type){
    if ( t->nodetype == idNode ){
        GInstall(t->name, type, type->size, -1);
    }
    else if ( t->nodetype == ptrNode ){
        GInstall(t->children[0]->name, make_pointer(type), 1, -1);
    }
    else if ( t->nodetype == connectorNode ){
        setTupleIDinGST(t->children[0], type);
        setTupleIDinGST(t->children[1], type);
    }
    else{
        printf("invalid node\n");
        exit(1);
    }
}

void setTupleIDinLST(struct tnode * t, struct Typetable* type, struct tnode * LSThead){
    if ( t->nodetype == idNode ){
        LInstall(t->name, type, LSThead);
    }
    else if ( t->nodetype == ptrNode ){
        LInstall(t->children[0]->name, make_pointer(type), LSThead);
    }
    else if ( t->nodetype == connectorNode ){
        setTupleIDinLST(t->children[0], type, LSThead);
        setTupleIDinLST(t->children[1], type, LSThead);
    }
    else{
        printf("invalid node\n");
        exit(1);
    }
}


// ---------------------------FUNC DECL-------------------------------------------------------------

void checkFDef(struct tnode ** c, struct Gsymbol *  funcEntry){
    if ( funcEntry->type != c[0]->type ){
        printf("Return type mismatch\n");
        printf("Expected %s, returned %s\n", c[0]->type->name, funcEntry->type->name);
        exit(1);
    }
    if ( c[2] != NULL ){
        compareParamList(c[2], funcEntry->paramlist );
    }

    struct Paramstruct * p = funcEntry->paramlist;
    while ( p ){
        if ( p->checked == 0 ){
            printf("Variable in Declaration not found in Definition: %s\n", p->name);
            exit(1);
        }
        p = p->next;
    }
}

void assignVarTypes(struct tnode* t, struct Lsymbol * Lentry, struct Typetable* retType){
    // Anything to be done while going down
    // Recursive Calls
    if ( t->nodetype == memberNode ){
        assignVarTypes(t->children[0], Lentry, retType);
    }
    else{
        for (int i = 0; i < t->childcount; i++ ){
            if (t->children[i]) assignVarTypes(t->children[i], Lentry, retType);
        }
    }
    // Doing checks while coming up 
    switch (t->nodetype){
        case idNode:{       struct Lsymbol * t1 = LLookup( t->name , Lentry );
                            struct Gsymbol * t2 = GLookup( t->name );
                            // printLSymbolTable(Lentry);
                            // printf("%s\n",t->name);
                            if ( t1 != NULL ) {
                                t->Lentry = t1;
                                t->type = t1->type;
                            }
                            else if ( t2 != NULL ){ 
                                t->Gentry = t2;
                                t->type = t2->type;
                            }
                            else{
                                printf("Undeclared Variable: %s\n", t->name);
                                exit(1);
                            }
                            break; 
                            }
        case arrTypeNode:   // Checking Index Type
                            if ( t->children[1]->type !=  TLookup("int") ){
                                printf("Indexing non-int address");
                                exit(1);
                            }
                            t->type = t->children[0]->type;
                            // Assigning Type information
                            // t->Gentry = t->children[0]->Gentry;
                            break;
                            
        case assignNode:{   int op1 = t->children[0]->type != t->children[1]->type;
                            // Allow User Defined types to store NULL
                            int op2 = t->children[1]->type == TLookup("null") 
                                    && t->children[0]->type->generalType == USER_DEF;
                            // Allow User Defined types to store integer addresses
                            int op3 = t->children[1]->type == TLookup("int") 
                                    && t->children[0]->type->generalType == USER_DEF;
                            if ( op1 == 1 && op2 == 0 && op3 == 0 ){
                                printf("Assignment Type Wrong\n");
                                printf("Trying to assign %s to %s\n",
                                        t->children[1]->type->name, 
                                        t->children[0]->type->name);
                                exit(1);
                            }
                            break;}

        case derefOpNode:   if ( t->children[0]->type->generalType != POINTER ){
                                printf("Dereferencing non pointer: %d\n",t->children[0]->type->generalType);
                                exit(1);
                            }
                            t->type = t->children[0]->type->fields->type;
                            break;
        
        case exprNode:     t->type = t->children[0]->type;
                            break;

        case addrNode:{     t->type = make_pointer(t->children[0]->type);
                            break;}
        case addNode:   
        case subNode:{      int op1 = t->children[0]->type->generalType == POINTER 
                                && t->children[1]->type == TLookup("int");
                            int op2 = t->children[1]->type->generalType == POINTER 
                                && t->children[0]->type == TLookup("int");
                            int op3 = t->children[0]->type->generalType == POINTER
                                && t->children[1]->type->generalType == POINTER;
                            int op4 = t->children[0]->type == TLookup("int")
                                && t->children[1]->type == TLookup("int");
                            if ( op1==0 && op2==0 && op3==0 && op4 == 0){
                                printf("Operands mismatch ! \n");
                                exit(1);
                                }
                            }
                            if ( t->children[0]->type->generalType == POINTER)
                                t->type = t->children[0]->type;
                            else if ( t->children[1]->type->generalType == POINTER )
                                t->type = t->children[1]->type;
                            else {
                                t->type = t->children[0]->type;
                            }
                            break;
        case mulNode:   
        case divNode:       if (t->children[0]->type != TLookup("int")
                                || t->children[1]->type != TLookup("int")){
                                printf("Operands mismatch! \n");
                                printf("Only INTs allowed for MUL and DIV! \n");
                                exit(1);
                                }
                            break;       
        case ifNode:    
        case whileNode:     if ( t->children[0]->type !=  TLookup("boolean") ){
                                printf("If / Loop Condition Not Boolean");
                                exit(1);
                            }
                            break;
        case dowhileNode:
        case repeatNode:    if ( t->children[1]->type !=  TLookup("boolean") ){
                                printf("Loop Condition Not Boolean");
                                exit(1);
                            }
                            break;
        case returnNode:    if (t->children[0]->type != retType){
                                printf("Return Type Does not match\n");
                                printf("Expected %s, returned %s\n", 
                                        retType->name, 
                                        t->children[0]->type->name);
                                exit(1);
                            }
                            break;
        case funcCallNode:{ t->type = findType(t->children[0]);
                            if (t->children[1]){
                                struct Paramstruct* argList = fetchArgList(t->children[1]); 
                                if ( verifyArgTypes(argList, t->children[0]->Gentry->paramlist)==0){
                                    printf("Wrong Args passed to %s\n", 
                                            t->children[0]->Gentry->name);
                                    exit(1);
                                } 
                            }
                            break; }
        case memberNode:{   struct Fieldlist * f 
                                = FLookup(t->children[0]->type, t->children[1]->name);
                            if ( f == NULL ){
                                printf("Trying to access undefined member of tuple\n");
                                exit(1);
                            }
                            t->type = f->type;
                            break; }
        case freeNode: {    if ( t->children[0]->type->generalType == USER_DEF ){
                                printf("Freeing a non user defined type");
                                exit(1);
                            }
                            break;
        }
    } 
}

struct Typetable* findType(struct tnode * t){
    if ( t->Lentry != NULL ){
        return t->Lentry->type;
    }
    if ( t->Gentry != NULL ){
        return t->Gentry->type;
    }
    return TLookup("void");
}

// ---------------------------ARGUMENT LIST---------------------------------------------------------

struct Paramstruct* fetchArgList(struct tnode * t){

    if ( t->childcount == 2 ){
        struct Paramstruct* head = fetchArgList(t->children[0]);
        struct Paramstruct* cur = head;
        while ( cur->next ) cur = cur->next;

        struct Paramstruct* temp = (struct Paramstruct*)malloc(sizeof(struct Paramstruct));
        temp->type = t->children[1]->type;
        cur->next = temp;
        return head;
    }
    else{
        struct Paramstruct* temp = (struct Paramstruct*)malloc(sizeof(struct Paramstruct));
        temp->type = t->children[0]->type;

        return temp;
    }
}

int verifyArgTypes(struct Paramstruct* argList, struct Paramstruct* paramList ){
    struct Paramstruct* p1 = argList;
    struct Paramstruct* p2 = paramList;

    while (p1 && p2){
        if ( p2->type != p1->type ) return 0;
        p1 = p1->next;
        p2 = p2->next;
    }
    if ( p1 || p2 ) return 0;
    return 1;
}

// ---------------------------PARAM LIST------------------------------------------------------------

void insertIntoParamList(struct Gsymbol* paramHead, struct Paramstruct* cur){
    
    if ( paramHead->paramlist == NULL ){
        paramHead->paramlist = cur;
    }
    else {
        struct Paramstruct* temp = paramHead->paramlist;
        while ( temp->next != NULL ){ 
            if ( strcmp(temp->name, cur->name) == 0 ){
                printf("Variable Duplicate : %s\n", cur->name);
                exit(1);
            }
            temp = temp->next;
        }
        if ( strcmp(temp->name, cur->name) == 0 ){
            printf("Variable Duplicate : %s\n", cur->name);
            exit(1);
        }
        temp->next = cur;
    }
}

struct Paramstruct* searchParamList(struct Paramstruct* head, char* name){
    struct Paramstruct* temp = head;
    while ( temp != NULL ){ 
        if ( strcmp(temp->name, name) == 0 ){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void compareParamList(struct tnode * t, struct Paramstruct *paramlist){
    if ( t -> nodetype == connectorNode ) {
        for ( int i=0; i< t->childcount; i++ )
            compareParamList( t->children[i], paramlist );
    }
    if ( t -> nodetype == paramNode ) {
        struct Paramstruct* entry = searchParamList( paramlist, t->children[1]->name );
        if ( entry == NULL ){
            printf("Function Parameter not found: %s\n", t->children[1]->name);
            exit(1);
        }
        if ( entry -> checked ){
            printf("Variable declared twice/ Function declared twice: %s\n", t->children[1]->name);
            exit(1);
        }
        if ( entry -> type != t->children[0]->type ){
            printf("Type Mismatch in Function Declaration: %s\n", t->children[1]->name);
            exit(1);
        }
        entry->checked = 1;
    }
}

struct Lsymbol* addParamListToLsymbolTable(struct Paramstruct * pl, struct Lsymbol* table){
    struct Paramstruct * t1 = pl;
    struct Lsymbol * head = table;
    struct Lsymbol * t2 = table;

    while ( t2 && t2->next ){
        t2 = t2->next;
    }

    while( t1 ){
        struct Lsymbol * l = (struct Lsymbol *)malloc(sizeof(struct Lsymbol)); 
        l->name = (char*)malloc(sizeof(char)*20);
        strcpy( l->name, t1->name);
        l->type = t1->type;
        l->binding = -1;

        if ( t2 ){
            t2 -> next = l;
            t2 = t2->next;
        }
        else{
            head = l;
            t2 = l;
        }
        t1 = t1->next;
    }
    return head;
}

// ----------------------------PRINTING-------------------------------------------------------------
void printTree(struct tnode* t, struct tnode* p, int depth){
    if ( t!= NULL){
        for (int i=0;i<depth;i++) printf("- ");
        // if (p) printf("%s -> %s\n", printNode(p), printNode(t) );
        // else printf("%s\n", printNode(t) );
        struct Typetable* a = NULL,*b = NULL;
        if (t->Gentry) a = t->Gentry->type;
        if (t->Lentry) b = t->Lentry->type;
        printf("%s (%s, %s, %s) \n", printNode(t), printType(t->type), printType(a), printType(b) );
        for (int i = 0; i < (t->childcount) ;  i++){
            printTree(t->children[i], t, depth+1);
        }
    }
}

char * printNode( struct tnode* t ){
    switch ( t->nodetype ){
        case readNode: return "Read";
        case writeNode: return "Write";
        case connectorNode: return "Connector";
        case assignNode: return "Assign";
        case addNode: return "Add";
        case subNode: return "Sub";
        case divNode: return "Div";
        case mulNode: return "Mul";
        case modNode: return "Mod";
        case geNode: return "ge";
        case leNode: return "le";
        case gtNode: return "gt";
        case ltNode: return "lt";
        case freeNode: return "free";
        case eqNode: return "eq";
        case neNode: return "ne";
        case ifNode: return "if";
        case initNode: return "initNode";
        case allocNode: return "allocNode";
        case nullNode: return "nullNode";
        case ptrNode: return "ptrNode";
        case whileNode: return "while";
        case dowhileNode: return "dowhile";
        case repeatNode: return "repeat";
        case breakNode: return "break";
        case contNode: return "continue";
        case typeDefNode: return "typeDef";
        case fieldDeclNode: return "fieldDecl";
        case typeNode: return "typeNode";
        case arrTypeNode: return "arrType";
        case exprNode: return "exprNode";
        case brkpNode: return "BRKP";
        case funcTypeGDeclNode: return "funcTypeGDeclNode";
        case paramNode: return "paramNode";
        case GDeclNode: return "GDeclNode";
        case rootNode: return "rootNode";
        case FDefNode: return "FDefNode";
        case memberNode: return "memberNode";
        case LDeclNode: return "LDeclNode";
        case returnNode: return "returnNode";
        case FBodyNode: return "FBodyNode";
        case funcCallNode: return "funcCallNode";
        case argNode: return "argNode";
        case derefOpNode: return "derefOpNode";
        case orNode: return "orNode";
        case andNode: return "andNode";
        case addrNode: return "addrNode";
        case tupleNode: return "tupleNode";
        case strConstNode: printf("%s : ", t->value.strval); return "strConst";
        case numNode: printf("%d : ", t->value.intval); return "Num";
        case idNode: printf("%s : ", t->name); return "Id";
        return "Uninitialized X";
    }
}

char * printType( struct Typetable* t ){
    if ( t ) return (t->name);
    else return "";
}

void printX(char * s, int X, int val, int type){
    if ( s == NULL ) return;
    int n = strlen(s);
    if ( type == 1 )
        printf("| %s", s);
    if ( type == 2 )
        printf("| %d", val);
    for (int i=0;i<X-n-2;i++) printf(" ");
}

// -------------------------------------------------------------------------------------------------

