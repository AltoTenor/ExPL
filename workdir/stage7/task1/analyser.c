#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "analyser.h"

struct Gsymbol * symbolTable;
struct Typetable * typeTable;
struct Classtable* classTable;
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
                                assignVarTypes(children[3]->children[1], Lentry, funcEntry->type, NULL);
                            else
                                assignVarTypes(children[3]->children[1], Lentry, TLookup("int"), NULL);
                            break;
                        }
        
        case classDefNode:{ // Make the Class Table Entry
                            struct Classtable * cur = CLookup(children[0]->name);
                            cur->memberField = fetchFieldList(children[1]);
                            cur->vFuncptr = constructMethodList(children[2]);
                            // Redeclare Checks
                            int fc = 0, mc = 0;
                            struct Fieldlist * i = cur->memberField;
                            while (i){
                                struct Fieldlist * j = cur->memberField;
                                while (j){
                                    if ( i->fieldIndex != j->fieldIndex 
                                        && strcmp(i->name, j->name) == 0){
                                        printf("Variable Redeclared: %s\n", i->name); 
                                        exit(1);
                                    }
                                    j = j->next;
                                }
                                i = i->next; fc++;
                            }
                            struct MemberFunclist * x = cur->vFuncptr;
                            while (x){
                                struct MemberFunclist * y = cur->vFuncptr;
                                while (y){
                                    if ( x->funcposition != y->funcposition 
                                        && strcmp(x->methodName, y->methodName) == 0){
                                        printf("Function Redeclared: %s\n", x->methodName); 
                                        exit(1);
                                    }
                                    y = y->next;
                                }
                                x = x->next; mc++;
                            }
                            cur ->fieldCount = fc;
                            cur ->methodCount = mc;
                            // Parse Body of Definitions
                            checkMDef(children[3], cur);

                            break;
                        }
        
        case CNameNode: {   // Setting up parent class if present
                            if ( children[1] ){
                                struct Classtable * p = CLookup(children[1]->name);
                                if ( p == NULL ){
                                    printf("Invalid Parent Inheritance\n");
                                    exit(1);
                                }
                                CInstall(children[0]->name, p);
                            }
                            else CInstall(children[0]->name, NULL);
                            temp->name = (char*)malloc(20*sizeof(char));
                            strcpy(temp->name, children[0]->name);
                            break;
                        }
    }
    temp->children = children;
    temp->Lentry = Lentry;
    temp->Ctype = NULL;
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

struct Typetable* TInstall( char *name, int size, 
                            struct Fieldlist *fields, 
                            GeneralType gt, 
                            struct Classtable * Ctype){
    struct Typetable* temp = (struct Typetable*)malloc(sizeof(struct Typetable));
    
    temp->name = (char *)malloc(sizeof(char)*20);
    strcpy( temp->name, name );
    temp->size = size;
    temp->fields = fields;
    temp->generalType = gt;
    temp->Ctype = Ctype;
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
    TInstall("int",1 , NULL, PRIMITIVE, NULL);
    TInstall("str",1 , NULL, PRIMITIVE, NULL);
    TInstall("boolean",1 , NULL, PRIMITIVE, NULL);
    TInstall("null",0 , NULL, PRIMITIVE, NULL);
    TInstall("void",0 , NULL, PRIMITIVE, NULL);
}

struct Typetable* make_pointer(struct Typetable* type){
    char newPtrType[30] = "pointer_";
    strcat(newPtrType, type->name);
    struct Typetable* tt = TLookup(newPtrType);
    if ( tt == NULL ){
        struct Fieldlist * f = (struct Fieldlist*)malloc(sizeof(struct Fieldlist));
        f->type = type;
        f->fieldIndex = 0;
        TInstall(newPtrType, 1, f, POINTER, NULL);
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

        struct Fieldlist * field = f1;
        int i = 0;
        while ( field->next ) {
            field->fieldIndex = i; 
            if (field->type->generalType == USER_DEF ) i++;
            else i+=field->type->size; 
            field = field->next; 
        }
        field->next = f2;
        while( field ) {
            field->fieldIndex = i; 
            if (field->type->generalType == USER_DEF ) i++;
            else i+=field->type->size; 
            field = field->next; 
        }
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
    if (type == NULL ) type = TInstall( children[0]->name, size, f, TYPE_TUPLE, NULL);

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

void assignVarTypes(struct tnode* t, 
                    struct Lsymbol * Lentry, 
                    struct Typetable* retType,
                    struct Classtable * classEntry ){
    // Anything to be done while going down
    // Recursive Calls
    if ( t->nodetype == memberNode ) assignVarTypes(t->children[0], Lentry, retType, classEntry);
    else if (t->nodetype != typeNode) for (int i = 0; i < t->childcount; i++ ){
            if (t->children[i]) assignVarTypes(t->children[i], Lentry, retType, classEntry);
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
                            // Check parent base pointer from GST/CT is a parent of childclass
                            // when doing parent = new(ChildClass)
                            int op5 = t->children[0]->Gentry
                                    && t->children[0]->Gentry->type->Ctype 
                                    && t->children[1]->Ctype 
                                    && checkDescendant( t->children[0]->Gentry->type->Ctype,
                                                        t->children[1]->Ctype );
                            int op6 = t->children[0]->Ctype
                                    && t->children[1]->Ctype 
                                    && checkDescendant( t->children[0]->Ctype,
                                                        t->children[1]->Ctype );
                            if ( op1 == 1 && op2 == 0 && op3 == 0  && op5 == 0 && op6 == 0){
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
        
        case exprNode:      t->type = t->children[0]->type;
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

        case geNode:
        case leNode:
        case ltNode:
        case gtNode:
        case eqNode:
        case neNode:{       int op1 = t->children[0]->type == t->children[1]->type;
                            int op2  = t->children[0]->type->generalType == USER_DEF 
                                    && t->children[1]->type == TLookup("null"); 
                            if ( op1 == 0 &&  op2 == 0 ){
                                printf("Type mismatch in relation op\n");
                                printf("LHS: type %s\n", t->children[0]->type->name);
                                printf("RHS: type %s\n", t->children[1]->type->name);
                                exit(1);
                            }    
                            break;}
        
        case andNode:
        case orNode:        if ( t->children[0]->type != t->children[1]->type 
                                ||  t->children[0]->type != TLookup("boolean") ){
                                printf("Only Booleans allowed for AND/OR ops\n");
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
        
        case funcCallNode:{ // Assign Type from GST or from child node ( if class )
                            if ( t->children[0]->Ctype ) t->type = t->children[0]->type;
                            else if (t->children[0]->Gentry) t->type = t->children[0]->Gentry->type;
                            else { printf("Incorrect Function Call Node\n"); exit(1); }

                            /* If left child (memNode) has a class in its node */
                            struct Paramstruct* argList = fetchArgList(t->children[1]); 
                            if ( t->children[0]->Ctype ){
                                struct MemberFunclist * method 
                                    = MethodLookup( t->children[0]->children[1]->name, 
                                                    t->children[0]->Ctype);
                                if ( method->paramlist ){
                                    if (verifyArgTypes( argList, method->paramlist ) == 0 ) {
                                        printf("Wrong Args passed to %s\n", 
                                                t->children[0]->children[1]->name);
                                        exit(1);
                                    }
                                }
                            }
                            /* If function exists in Global Symbol Table */
                            else if ( t->children[0]->Gentry ){
                                if ( t->children[0]->Gentry->paramlist ){
                                    if ( verifyArgTypes(argList, t->children[0]->Gentry->paramlist) 
                                        == 0 ){
                                            printf("Wrong Args passed to %s\n", 
                                                    t->children[0]->Gentry->name);
                                            exit(1);
                                        }
                                }
                            }
                            break; 
                        }
        
        case memberNode:{   /* To handle - self.x or self.func()
                                If leftside is selfNode then assign type directly if node is member
                                Else if node refers to function then assign return type and Class
                                to the node */
                            if (t->children[0]->nodetype == selfNode) {
                                if ( classEntry == NULL ){
                                    printf("Keyword \"self\" used outside class definition\n");
                                    exit(1);   
                                }
                                struct Fieldlist * mem 
                                    = MemberLookup(t->children[1]->name, classEntry);
                                struct MemberFunclist * met 
                                    = MethodLookup(t->children[1]->name, classEntry);
                                if ( mem == NULL && met == NULL){
                                    printf("self.%s does not exist\n", t->children[1]->name);
                                    exit(1);   
                                }
                                if (mem != NULL) t->type = mem->type;
                                if (met != NULL) t->type = met->type;
                                t->Ctype = classEntry;
                                break;
                            }
                            /* To handle - self.obj.func() and obj.func()
                                Checks if self.obj is of type CLASS or not. If yes change Ctype of
                                node and also check if function exists */
                            if  ( t->children[0]->type->generalType == CLASS_TYPE ){
                                struct MemberFunclist * met 
                                    = MethodLookup( t->children[1]->name, 
                                                    t->children[0]->type->Ctype);
                                if ( met == NULL){
                                    printf("%s.%s does not exist\n",
                                                t->children[0]->type->Ctype->className, 
                                                t->children[1]->name);
                                    exit(1);   
                                }
                                t->type = met->type;
                                t->Ctype = t->children[0]->type->Ctype;
                                break;
                            }
                            // Tuples and user defined types 
                            if  (   t->children[0]->type->generalType == USER_DEF 
                                ||  t->children[0]->type->generalType == TYPE_TUPLE ){
                                struct Fieldlist * f 
                                    = FLookup(t->children[0]->type, t->children[1]->name);
                                if ( f == NULL ){ printf("Undefined Member access\n"); exit(1); }
                                t->type = f->type;
                                break;
                            }
                            else {
                                printf("Invalid Member Access: %s\n", t->children[1]->name); 
                                exit(1); 
                            }
                        }
        
        case freeNode: {    if ( t->children[0]->type->generalType != USER_DEF ){
                                printf("Freeing a non user defined type: %s\n",t->children[0]->name);
                                exit(1);
                            }
                            break;
        }

        case newNode: {     t->Ctype = t->children[0]->type->Ctype;
                            t->type = t->children[0]->type;
                            break;
                        }
        
        case deleteNode:{   if ( t->children[0]->type->Ctype == NULL ){
                                printf("Cannot delete variable: %s\n", t->children[0]->name);
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
    if ( t == NULL ) return NULL;
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

// ----------------------------CLASS TABLE-------------------------------------------------------------
struct Classtable* CInstall(char *name, struct Classtable * parent){
    struct Classtable* temp = (struct Classtable*)malloc(sizeof(struct Classtable));

    temp->className = (char*)malloc(sizeof(char)*20);
    strcpy(temp->className, name);
    temp->memberField = NULL;      
    temp->vFuncptr = NULL;    
    temp->parentPtr = parent;
    temp->classIndex = -1;
    temp->fieldCount = -1;                     
    temp->methodCount = -1;                    
    temp->next = NULL;
    
    // Adding to class table
    if ( classTable == NULL ) classTable = temp;
    else{
        struct Classtable* t = classTable;
        while ( t->next ) t = t->next;
        t->next = temp;
    }

    // Adding class to type table
    TInstall(name, 2, NULL, CLASS_TYPE, temp);
    return temp;
}

struct Classtable* CLookup(char *name){
    if ( name == NULL ) return NULL;
    struct Classtable * t = classTable;
    while(t){
        if ( strcmp(t->className , name ) == 0 ){
            return t;
        }
        t = t->next;
    }
    return NULL;
}

struct MemberFunclist * constructMethodList(struct tnode * t){
    if ( t->nodetype == connectorNode ){
        struct MemberFunclist * head = NULL;
        struct MemberFunclist * tail = NULL;
        for (int i=0;i<t->childcount;i++){ 
            if ( head == NULL ) head = constructMethodList(t->children[i]);
            else {
                tail = head;
                while ( tail->next ) tail = tail->next;
                tail -> next = constructMethodList(t->children[i]);
                // Reassigning Function Position Numbers
                tail = head; int pos = 0;
                while (tail) { tail->funcposition = pos++; tail = tail->next; }
            }
        }
        return head;
    }
    else if (t->nodetype == MDeclNode ){
        struct Paramstruct* paramlist = constructMethodParamList(t->children[2]);
        struct Paramstruct* i = paramlist;

        // Checking if the paramlist has any repeating fields
        int i1 = 0, j1 = 0;
        while (i){
            struct Paramstruct* j = paramlist; j1 = 0;
            while (j){
                if ( i1 != j1 && strcmp(i->name, j->name) == 0){
                    printf("Variable Redeclared: %s\n", i->name); 
                    exit(1);
                }
                j = j->next; j1++;
            }
            i = i->next; i1++;
        }
        return MInstall( t->children[1]->name, t->children[0]->type, paramlist);
        
    }
    else { printf("Invalid Node\n"); exit(1); }
}

struct Paramstruct* constructMethodParamList(struct tnode* t){
    if ( t == NULL ) return NULL;
    if ( t->nodetype == connectorNode ){
        struct Paramstruct * head = NULL;
        struct Paramstruct * tail = NULL;
        for (int i=0;i<t->childcount;i++){ 
            if ( head == NULL ) head = constructMethodParamList(t->children[i]);
            else {
                tail = head;
                while ( tail->next ) tail = tail->next;
                tail -> next = constructMethodParamList(t->children[i]);
            }
        }
        return head;
    }
    else if (t->nodetype == paramNode ){
        struct Paramstruct* temp = (struct Paramstruct*)malloc(sizeof(struct Paramstruct));
        temp->name = (char*)malloc(sizeof(char)*20);
        strcpy(temp->name, t->children[1]->name);
        temp->type = t->children[0]->type;
        temp->checked = 0;
        temp->next = NULL;
        return temp;
    }
    else { printf("Invalid Node\n"); exit(1); }
}

struct MemberFunclist * MethodLookup(char * name, struct Classtable* CT){
    struct MemberFunclist * temp = CT->vFuncptr;
    while (temp){
        if ( strcmp(temp->methodName, name) == 0 ) return temp;
        temp = temp->next;
    }
    return NULL;
}

struct Fieldlist * MemberLookup(char * name, struct Classtable* CT){
    struct Fieldlist * temp = CT->memberField;
    while (temp){
        if ( strcmp(temp->name, name) == 0 ) return temp;
        temp = temp->next;
    }
    return NULL;
}

struct MemberFunclist * MInstall(char * methodName, 
                                 struct Typetable * type, 
                                 struct Paramstruct * paramlist){

    struct MemberFunclist * temp = (struct MemberFunclist *)malloc(sizeof(struct MemberFunclist));
    
    temp->methodName = (char*)malloc(sizeof(char)*20);
    strcpy(temp->methodName, methodName);
    temp->type = type;
    temp->paramlist = paramlist;
    temp->flabel = globalFlabel++;
    temp->funcposition = 0;
    temp->next = NULL;
    return temp;
}

void checkMDef(struct tnode* t, struct Classtable* CTEntry){
    // printf("%d\n", t->nodetype);
    if ( t->nodetype == connectorNode ){
        for (int i=0;i<t->childcount;i++) checkMDef(t->children[i], CTEntry);
    }
    else if ( t->nodetype == MDefNode ){
        struct MemberFunclist * method = MethodLookup(t->children[1]->name, CTEntry);
        // Checking if return type matches
        if ( method->type != t->children[0]->type ){
            printf("Mismatch in Return Type: %s\n", t->children[1]->name);
            printf("Should be: %s\n", method->type->name);
            printf("Found to be: %s\n", t->children[0]->type->name);
            exit(1);
        }
        // Checking if all arguments match or not
        if (t->children[2]) compareParamList(t->children[2], method->paramlist);
        struct Paramstruct * p = method->paramlist;
        while ( p ){
            if ( p->checked == 0 ){
                printf("Variable in Declaration not found in Definition: %s\n", p->name);
                exit(1);
            }
            p = p->next;
        }
        // printLSymbolTable(t->children[3]->Lentry);
        // Checking everything else works or not
        if (t->children[2] != NULL){
            t->Lentry = addParamListToLsymbolTable(method->paramlist, t->children[3]->Lentry);
        }
        else t->Lentry = t->children[3]->Lentry;
        LInstall("self", TLookup(CTEntry->className), t);

        addBindingAddr(t->Lentry);

        // Make LST accessible to all Body and skip the Local Declarations
        assignVarTypes(t->children[3]->children[1], t->Lentry, t->children[0]->type, CTEntry);
        // Adding class entry to MDefNode
        t->Ctype = CTEntry;

        printf("LST for Class: %s \nFunction: %s\n",CTEntry->className, t->children[1]->name);
        printLSymbolTable(t->Lentry);
    }
}

int checkDescendant(struct Classtable * parent, struct Classtable * child ){
    while ( child ){
        if ( strcmp(child->className, parent->className) == 0 ) return 1;
        child = child->parentPtr;
    }
    return 0;
}

// void addMemberstoLST(struct tnode* t, struct Classtable* CTEntry){
//     struct Fieldlist * f = CTEntry->memberField;
//     while(f){
//         LInstall(f->name, f->type, t);
//         f = f->next;
//     }
//     return;
// }

void printClassTable(){
    struct Classtable* temp = classTable;
    printf("Class Table \n");
    printf("| ClassName | Index | FieldCount | MethodCount| Parent\n"); 
    while (temp){
        printX(temp->className, 12, -1, 1);
        printX("", 6, temp->classIndex, 2);
        printX("", 12, temp->fieldCount, 2);
        printX("", 12, temp->methodCount, 2);
        if ( temp->parentPtr ) printX(temp->parentPtr->className, 11, -1, 1);
        else printX("(nil)", 11, -1, 1);
        struct Fieldlist * f = temp->memberField;
        printf("\n-> Class Members: \n");
        printf("-- | Name | Type | FieldIndex |\n");
        while(f){
            printf("-- ");
            printX(f->name, 7, -1, 1);
            printX(f->type->name, 7, -1, 1);
            printX("", 6, f->fieldIndex, 2);
            printf("\n");
            f = f->next; 
        }
        printf("-> Class Functions/Methods: \n");
        printf("-- | Name        | Type | Flabel | Funcpos |\n");
        struct MemberFunclist * g = temp->vFuncptr;
        while(g){
            printf("-- ");
            printX(g->methodName, 14, -1, 1);
            printX(g->type->name, 7, -1, 1);
            printX("", 8, g->flabel, 2);
            printX("", 10, g->funcposition, 2);
            printf("\n");
            g = g->next; 
        }
        printf("\n");
        temp = temp->next; 
    }
    printf("\n");
    printf("\n");
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
        printf("%s (%s, %s, %s, %s) \n", 
            printNode(t), 
            printType(t->type), 
            printType(a), 
            printType(b),
            printClass(t->Ctype)
        );
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
        case newNode: return "new";
        case deleteNode: return "delete";
        case subNode: return "Sub";
        case divNode: return "Div";
        case mulNode: return "Mul";
        case modNode: return "Mod";
        case CNameNode: return "CName";
        case selfNode: return "self";
        case geNode: return "ge";
        case leNode: return "le";
        case gtNode: return "gt";
        case ltNode: return "lt";
        case freeNode: return "free";
        case eqNode: return "eq";
        case neNode: return "ne";
        case ifNode: return "if";
        case classDefNode: return "classDefNode";
        case MDeclNode: return "MDeclNode";
        case MDefNode: return "MDefNode";
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
char * printClass( struct Classtable* t ){
    if ( t ) return (t->className);
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

// -------------------------------CODEGENS----------------------------------------------------------

void compile(struct tnode * t){

}