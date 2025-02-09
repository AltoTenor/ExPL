#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "analyser.h"
#include "codegen.h"

struct Gsymbol * symbolTable;
struct Typetable * typeTable;
struct Classtable* classTable;
int SP;
int *arr;
int globalFlabel;
int classIndex;

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
    if ( nodetype==NODE_TYPE && temp->type == NULL ){
        printf("Invalid Type Used: %s\n", type);
        exit(1);
    }
    temp->childcount = childcount;
    temp->Ctype = NULL;
    temp->nodetype = nodetype;
    switch (nodetype){
        case NODE_CONST_NUM:       temp->value.intval = c->intval;
                            break;

        case NODE_CONST_STR:  temp->value.strval = (char*)malloc(sizeof(char)*30); 
                            strcpy(temp->value.strval, c->strval);
                            break;

        case NODE_ID:        temp->name = (char*)malloc(20*sizeof(char));
                            strcpy(temp->name, name);
                            break;

                            break;
        case NODE_FDEF:{      //Checks Function Decl match with Function Def
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
                                assignVarTypes( children[3]->children[1], 
                                                Lentry, funcEntry->type, NULL);
                            else
                                assignVarTypes(children[3]->children[1], 
                                                Lentry, TLookup("int"), NULL);
                            break;
                        }
        
        case NODE_CLASS_DEF:{ // Make the Class Table Entry
                            struct Classtable * cur = CLookup(children[0]->name);
                            
                            // Setting up members
                            cur->memberField = fetchFieldList(children[1]);
                            cur->memberField = addParentFieldList(cur);
                            
                            // Setting up methods
                            cur->vFuncptr = constructMethodList(children[2]);
                            cur->vFuncptr = addParentMethods(cur);
                            
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
                            while (x){ x = x->next; mc++; }
                            
                            cur ->fieldCount = fc;
                            cur ->methodCount = mc;
                            // Max 8 methods and members allowed
                            if ( fc > 8 || mc > 8 ) 
                                {printf("More than 8 members/methods\n"); exit(1);}
                            // Parse Body of Definitions
                            checkMDef(children[3], cur);
                            temp->Ctype = cur;
                            break;
                        }
        
        case NODE_CNAME: {   // Setting up parent class if present
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
    if ( t->nodetype == NODE_CONNECTOR ){
        setGTypes( t -> children[0], type, paramHead);
        setGTypes( t -> children[1], type, paramHead);
    }
    else if( t->nodetype == NODE_ID ){
        // For Dynamic Allocation we only storing the pointer
        if ( type->generalType == USER_DEF ) GInstall( t->name, type, 1, -1);
        // For Static ALlocation we need space = size
        else GInstall( t->name, type, type->size, -1);
    }
    else if( t->nodetype == NODE_PTR ){
        GInstall( t->children[0]->name, make_pointer(type), 1, -1);
    }
    else if( t->nodetype == NODE_ARR_TYPE ){
        // No matter static/dynamic we store the size
        GInstall( t->children[0]->name, type, t->children[1]->value.intval, -1);
    }
    else if( t->nodetype == NODE_GDECL_FUNC ){
        GInstall( t->children[0]->name, type, 0, globalFlabel++);
        if ( t->children[1] ) setGTypes( t -> children[1], type, GLookup(t->children[0]->name));
    }
    else if( t->nodetype == NODE_GDECL_PTRFUNC ){
        GInstall( t->children[0]->name, make_pointer(type), 0, globalFlabel++);
        if ( t->children[1] ) setGTypes( t -> children[1], type, GLookup(t->children[0]->name));
    }
    else if( t->nodetype == NODE_PARAM ){
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
    printf("Global Symbol Table: \n");
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
    printf("\n");
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
    if ( t->nodetype == NODE_CONNECTOR ){
        setLTypes( type, t -> children[0], head);
        setLTypes( type, t -> children[1], head);
    }
    else if ( t->nodetype == NODE_ID ){
        struct Lsymbol * l = LInstall( t->name, type, head );
    }
    else if ( t->nodetype == NODE_PTR ){
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
    printf("Global TypeTable \n");
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
}

struct Fieldlist * fetchFieldList(struct tnode * t){
    if ( t == NULL ) return NULL;
    if ( t->nodetype == NODE_PARAM ){
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
    if ( t->nodetype == NODE_ID ){
        GInstall(t->name, type, type->size, -1);
    }
    else if ( t->nodetype == NODE_PTR ){
        GInstall(t->children[0]->name, make_pointer(type), 1, -1);
    }
    else if ( t->nodetype == NODE_CONNECTOR ){
        setTupleIDinGST(t->children[0], type);
        setTupleIDinGST(t->children[1], type);
    }
    else{
        printf("invalid node\n");
        exit(1);
    }
}

void setTupleIDinLST(struct tnode * t, struct Typetable* type, struct tnode * LSThead){
    if ( t->nodetype == NODE_ID ){
        LInstall(t->name, type, LSThead);
    }
    else if ( t->nodetype == NODE_PTR ){
        LInstall(t->children[0]->name, make_pointer(type), LSThead);
    }
    else if ( t->nodetype == NODE_CONNECTOR ){
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
    if ( t->nodetype == NODE_MEMBER ) assignVarTypes(t->children[0], Lentry, retType, classEntry);
    else if (t->nodetype != NODE_TYPE) for (int i = 0; i < t->childcount; i++ ){
            if (t->children[i]) assignVarTypes(t->children[i], Lentry, retType, classEntry);
    }
    // Doing checks while coming up 
    switch (t->nodetype){
        case NODE_ID:{       struct Lsymbol * t1 = LLookup( t->name , Lentry );
                            struct Gsymbol * t2 = GLookup( t->name );
                            // printLSymbolTable(Lentry);
                            // printf("%s\n",t->name);
                            if ( t1 != NULL ) {
                                t->Lentry = t1;
                                t->type = t1->type;
                                t->Ctype = t1->type->Ctype;
                            }
                            else if ( t2 != NULL ){ 
                                t->Gentry = t2;
                                t->type = t2->type;
                                t->Ctype = t2->type->Ctype;
                            }
                            else{
                                printf("Undeclared Variable: %s\n", t->name);
                                exit(1);
                            }
                            break; 
                            }
        
        case NODE_ARR_TYPE:   // Checking Index Type
                            if ( t->children[1]->type !=  TLookup("int") ){
                                printf("Indexing non-int address");
                                exit(1);
                            }
                            t->type = t->children[0]->type;
                            // Assigning Type information
                            // t->Gentry = t->children[0]->Gentry;
                            break;
                            
        case NODE_ASSIGN:{   int op1 = t->children[0]->type != t->children[1]->type;
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

        case NODE_DEREF:   if ( t->children[0]->type->generalType != POINTER ){
                                printf("Dereferencing non pointer: %d\n",
                                        t->children[0]->type->generalType);
                                exit(1);
                            }
                            t->type = t->children[0]->type->fields->type;
                            break;
        
        case NODE_EXPR:      t->type = t->children[0]->type;
                            t->Ctype = t->children[0]->Ctype;
                            break;

        
        case NODE_ADDR:{     t->type = make_pointer(t->children[0]->type);
                            break;}
        
        case NODE_ADD:   
        case NODE_SUB:{      int op1 = t->children[0]->type->generalType == POINTER 
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
        
        case NODE_MUL:   
        case NODE_DIV:       if (t->children[0]->type != TLookup("int")
                                || t->children[1]->type != TLookup("int")){
                                printf("Operands mismatch! \n");
                                printf("Only INTs allowed for MUL and DIV! \n");
                                exit(1);
                                }
                            break;   

        case NODE_GE:
        case NODE_LE:
        case NODE_LT:
        case NODE_GT:
        case NODE_EQ:
        case NODE_NE:{       int op1 = t->children[0]->type == t->children[1]->type;
                            int op2a  = t->children[0]->type->generalType == USER_DEF 
                                    && t->children[1]->type == TLookup("null"); 
                            int op2b  = t->children[1]->type->generalType == USER_DEF 
                                    && t->children[0]->type == TLookup("null"); 
                            int op3a  = t->children[0]->type->generalType == CLASS_TYPE 
                                    && t->children[1]->type == TLookup("null"); 
                            int op3b  = t->children[1]->type->generalType == CLASS_TYPE 
                                    && t->children[0]->type == TLookup("null"); 
                            if ( op1 == 0 && op2a == 0 && op2b == 0 && op3a == 0 && op3b == 0 ){
                                printf("Type mismatch in relation op\n");
                                printf("LHS: type %s\n", t->children[0]->type->name);
                                printf("RHS: type %s\n", t->children[1]->type->name);
                                exit(1);
                            }    
                            break;}
        
        case NODE_AND:
        case NODE_OR:        if ( t->children[0]->type != t->children[1]->type 
                                ||  t->children[0]->type != TLookup("boolean") ){
                                printf("Only Booleans allowed for AND/OR ops\n");
                                exit(1);
                            }    
                            break;
        
        case NODE_IF:    
        case NODE_WHILE:     if ( t->children[0]->type !=  TLookup("boolean") ){
                                printf("If / Loop Condition Not Boolean");
                                exit(1);
                            }
                            break;
        
        case NODE_DOWHILE:
        case NODE_REPEAT:    if ( t->children[1]->type !=  TLookup("boolean") ){
                                printf("Loop Condition Not Boolean");
                                exit(1);
                            }
                            break;
        
        case NODE_RETURN:    if (t->children[0]->type != retType){
                                printf("Return Type Does not match\n");
                                printf("Expected %s, returned %s\n", 
                                        retType->name, 
                                        t->children[0]->type->name);
                                exit(1);
                            }
                            break;
        
        case NODE_FUNC_CALL:{ // Assign Type from GST or from child node ( if class )
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
        
        case NODE_MEMBER:{   /* To handle - self.x or self.func()
                                If leftside is NODE_SELF then assign type directly if node is member
                                Else if node refers to function then assign return type and Class
                                to the node */
                            if (t->children[0]->nodetype == NODE_SELF) {
                                if ( classEntry == NULL ){
                                    printf("Keyword \"self\" used outside class definition\n");
                                    exit(1);   
                                }
                                struct MemberFunclist * x = classEntry->vFuncptr;
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
        
        case NODE_FREE: {    if ( t->children[0]->type->generalType != USER_DEF ){
                                printf("Freeing a non user defined type: %s\n",
                                        t->children[0]->name);
                                exit(1);
                            }
                            break;
        }

        case NODE_NEW:   {   t->Ctype = t->children[0]->type->Ctype;
                            t->type = t->children[0]->type;
                            break;
                        }
        case NODE_SELF:  {   struct Lsymbol * t1 = LLookup( "self" , Lentry );
                            t->Lentry = t1;
                            t->type = t1->type;
                            t->Ctype = t1->type->Ctype;
                            break;
                        }
        
        case NODE_DELETE:{   if ( t->children[0]->type->Ctype == NULL ){
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
    if ( t -> nodetype == NODE_CONNECTOR ) {
        for ( int i=0; i< t->childcount; i++ )
            compareParamList( t->children[i], paramlist );
    }
    if ( t -> nodetype == NODE_PARAM ) {
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

// ----------------------------CLASS TABLE----------------------------------------------------------
struct Classtable* CInstall(char *name, struct Classtable * parent){
    struct Classtable* temp = (struct Classtable*)malloc(sizeof(struct Classtable));

    if ( CLookup(name) != NULL ) {printf("Duplicate class: %s\n", name);exit(1);}

    temp->className = (char*)malloc(sizeof(char)*20);
    strcpy(temp->className, name);
    temp->memberField = NULL;      
    temp->vFuncptr = NULL;    
    temp->parentPtr = parent;
    temp->classIndex = classIndex++;
    temp->fieldCount = -1;                     
    temp->methodCount = -1;                    
    temp->next = NULL;
    SP += 8;
    
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
    if ( t->nodetype == NODE_CONNECTOR ){
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
    else if (t->nodetype == NODE_MDECL ){
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
    if ( t->nodetype == NODE_CONNECTOR ){
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
    else if (t->nodetype == NODE_PARAM ){
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
    if ( t->nodetype == NODE_CONNECTOR ){
        for (int i=0;i<t->childcount;i++) checkMDef(t->children[i], CTEntry);
    }
    else if ( t->nodetype == NODE_MDEF ){
        struct MemberFunclist * method = MethodLookup(t->children[1]->name, CTEntry);
        // Checking if function is declared
        if ( method == NULL ) {
            printf("Method Not Declared: %s in class %s\n",
                    t->children[1]->name, CTEntry->className); 
            exit(1);
        }
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
        // Adding an entry for self
        LInstall("self", TLookup(CTEntry->className), t);
        // Adding binding address to all entries
        addBindingAddr(t->Lentry);

        // Make LST accessible to all Body and skip the Local Declarations
        assignVarTypes(t->children[3]->children[1], t->Lentry, t->children[0]->type, CTEntry);
        // Adding class entry to NODE_MDEF
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

struct Fieldlist * addParentFieldList(struct Classtable * CTEntry){
    // If no parent OR parent does not have any members
    if ( CTEntry->parentPtr == NULL 
        || CTEntry->parentPtr->memberField == NULL) return CTEntry->memberField;
    
    struct Fieldlist *head = NULL, *tail=NULL;
    struct Fieldlist *parentMem = CTEntry->parentPtr->memberField;
    
    // Copying parent member field into a new member field
    while ( parentMem ){
        struct Fieldlist * temp = (struct Fieldlist * )malloc(sizeof(struct Fieldlist));
        temp->name = (char*)malloc(sizeof(char)*20);

        strcpy(temp->name, parentMem->name);
        temp->fieldIndex = parentMem->fieldIndex;
        temp->type = parentMem->type;
        temp->next = NULL;

        if ( head == NULL ){
            head = temp;
            tail = temp;
        }
        else {
            tail->next = temp;
            tail = temp;
        }
        parentMem = parentMem -> next;
    }
    
    // Setting the field index correctly for new members
    int offset = tail->fieldIndex+1;
    tail->next = CTEntry->memberField;
    tail = tail->next;
    while ( tail ) { tail->fieldIndex += offset; tail = tail->next;}
    return head;

}

struct MemberFunclist * addParentMethods(struct Classtable * CTEntry){
    // Checking to see if any function is duplicate in the new declaration
    struct MemberFunclist * x = CTEntry->vFuncptr;
    while (x){
        struct MemberFunclist * y = CTEntry->vFuncptr;
        while (y){
            if ( x->funcposition != y->funcposition 
                && strcmp(x->methodName, y->methodName) == 0){
                printf("Function Redeclared: %s\n", x->methodName); 
                exit(1);
            }
            y = y->next;
        }
        x = x->next;
    }

    // If no parent return this itself
    if ( CTEntry->parentPtr == NULL ) return CTEntry->vFuncptr;

    // Making a copy of the parent method list
    struct MemberFunclist *head = NULL, *tail=NULL ,*tail2 = NULL;
    struct MemberFunclist *parentMet = CTEntry->parentPtr->vFuncptr;
    
    // Copying parent member field into a new member field
    while ( parentMet ){
        struct MemberFunclist * temp
            = (struct MemberFunclist * )malloc(sizeof(struct MemberFunclist));
        temp->methodName = (char*)malloc(sizeof(char)*20);

        strcpy(temp->methodName, parentMet->methodName);
        temp->type = parentMet->type;
        temp->flabel = parentMet->flabel;
        temp->funcposition = parentMet->funcposition;
        temp->paramlist = parentMet->paramlist;
        temp->next = NULL;

        if ( head == NULL ){
            head = temp;
            tail = temp;
        }
        else {
            tail->next = temp;
            tail = temp;
        }
        parentMet = parentMet -> next;
    }

    // Replacing or adding the new methods
    struct MemberFunclist * newMethod = CTEntry->vFuncptr;
    while ( newMethod ){
        tail = head;
        int redefinedFlag = 0;
        while ( tail ){
            if ( strcmp(tail->methodName, newMethod->methodName) == 0 ){
                redefinedFlag = 1;
                tail->flabel = newMethod->flabel;
                struct Paramstruct * p = tail->paramlist;
                while(p) { p->checked = 0; p = p->next; }
                // Check if signature of redeclaration is same or not
                if ( tail->type != newMethod->type ){
                    printf("Cannot redeclare function with different type: %s\n", 
                        newMethod->methodName); 
                    exit(1);
                }
            }
            tail2 = tail;
            tail = tail->next;
        }
        tail = newMethod -> next;
        // If function is a new function
        if ( redefinedFlag == 0 ){
            tail2 -> next = newMethod;
            newMethod->next = NULL;
            newMethod->funcposition = tail2->funcposition+1;
        }
        newMethod = tail;
    }
    return head;
}

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
        case NODE_READ: return "Read";
        case NODE_WRITE: return "Write";
        case NODE_CONNECTOR: return "Connector";
        case NODE_ASSIGN: return "Assign";
        case NODE_ADD: return "Add";
        case NODE_NEW: return "new";
        case NODE_DELETE: return "delete";
        case NODE_SUB: return "Sub";
        case NODE_DIV: return "Div";
        case NODE_MUL: return "Mul";
        case NODE_MOD: return "Mod";
        case NODE_CNAME: return "CName";
        case NODE_SELF: return "self";
        case NODE_GE: return "ge";
        case NODE_LE: return "le";
        case NODE_GT: return "gt";
        case NODE_LT: return "lt";
        case NODE_FREE: return "free";
        case NODE_EQ: return "eq";
        case NODE_NE: return "ne";
        case NODE_IF: return "if";
        case NODE_CLASS_DEF: return "classDef";
        case NODE_MDECL: return "MDecl";
        case NODE_MDEF: return "MDef";
        case NODE_INIT: return "init";
        case NODE_ALLOC: return "alloc";
        case NODE_NULL: return "nullNode";
        case NODE_PTR: return "ptrNode";
        case NODE_WHILE: return "while";
        case NODE_DOWHILE: return "dowhile";
        case NODE_REPEAT: return "repeat";
        case NODE_BREAK: return "break";
        case NODE_CONTINUE: return "continue";
        case NODE_TYPEDEF: return "typeDef";
        case NODE_FIELD_DECL: return "fieldDecl";
        case NODE_TYPE: return "typeNode";
        case NODE_ARR_TYPE: return "arrType";
        case NODE_EXPR: return "exprNode";
        case NODE_BRKP: return "BRKP";
        case NODE_GDECL_FUNC: return "GeclNode";
        case NODE_PARAM: return "param";
        case NODE_GDECL: return "GDecl";
        case NODE_ROOT: return "rootNode";
        case NODE_FDEF: return "FDef";
        case NODE_MEMBER: return "memberNode";
        case NODE_LDECL: return "LDecl";
        case NODE_RETURN: return "returnNode";
        case NODE_FBODY: return "FBody";
        case NODE_FUNC_CALL: return "funcCallNode";
        case NODE_ARG: return "argNode";
        case NODE_DEREF: return "derefNode";
        case NODE_OR: return "orNode";
        case NODE_AND: return "andNode";
        case NODE_ADDR: return "addrNode";
        case NODE_TUPLE: return "tuple";
        case NODE_CONST_STR: printf("%s : ", t->value.strval); return "strConst";
        case NODE_CONST_NUM: printf("%d : ", t->value.intval); return "Num";
        case NODE_ID: printf("%s : ", t->name); return "Id";
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
    initialize();
    struct Context * c = (struct Context *)malloc(sizeof(struct Context));
    c->jumpLabels = (int *)malloc(sizeof(int)*2);
    c->mainFunc = 0;
    codeGen(t->children[1], c);
    codeGen(t->children[3], c);
    c->mainFunc = 1;
    codeGen(t->children[4], c);
    return;
}