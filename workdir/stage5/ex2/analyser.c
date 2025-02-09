#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "analyser.h"

struct Gsymbol * symbolTable;
int SP;
int *arr;
int globalFlabel;

/*Creates and initializes a new node in the expression tree*/
struct tnode* createTree(   int val, 
                            int vartype, 
                            char* c, 
                            int nodetype, 
                            struct tnode **children, 
                            int childcount,
                            struct Gsymbol * Gentry,
                            struct Lsymbol * Lentry ){
    struct tnode *temp;
    struct Gsymbol * funcEntry = NULL;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->Gentry = Gentry;
    temp->type = vartype;
    temp->childcount = childcount;
    temp->nodetype = nodetype;
    switch (nodetype){
        case strConstNode:
        case idNode:    temp->varname= (char*)malloc(12*sizeof(char));
                        strcpy(temp->varname, c);
                        break;
        case numNode:   temp->val = val;
                        break;
        case FDefNode:  //Checks Function Decl match with Function Def
                        funcEntry = GLookup(children[1]->varname);
                        if ( strcmp(children[1]->varname, "main")!=0 ){
                            if ( funcEntry == NULL  ) {
                                printf("Declare Function in Header: %s\n", children[1]->varname);
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
                        if ( strcmp(children[1]->varname, "main")!=0 )
                            assignVarTypes(children[3]->children[1], Lentry, funcEntry->type);
                        else   
                            assignVarTypes(children[3]->children[1], Lentry, intType);

                        break;
    }
    temp->children = children;
    temp->Lentry = Lentry;
    return temp;
}

// ------------------GLOBAL SYMBOL TABLE------------------------------------------------------------

void GInstall(char *name, char *tupName, int type, int size, int fl){

    // printf("%s\n", name);

    // Error when duplicate variable names
    if ( GLookup(name) != NULL ){ 
        printf("Variable redeclared!\n"); 
        exit(1); 
    }

    struct Gsymbol * temp = (struct Gsymbol *)malloc(sizeof(struct Gsymbol));
    temp->name = (char*)malloc(32*sizeof(char));
    strcpy(temp->name, name);

    temp->tupName = (char*)malloc(32*sizeof(char));
    strcpy(temp->tupName, tupName);

    temp->size = size;
    temp->type = type;
    temp->binding = SP;
    temp->flabel = fl;
    temp->next = NULL;
    temp->paramlist = NULL;
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

void setGTypes(struct tnode* t, int type, struct Gsymbol* paramHead){
    if ( t->nodetype == connectorNode ){
        setGTypes( t -> children[0], type, paramHead);
        setGTypes( t -> children[1], type, paramHead);
    }
    else if( t->nodetype == idNode ){
        GInstall( t->varname, "", type, 1, -1);
        t->Gentry = GLookup(t->varname);
        t->type = type;
    }
    else if( t->nodetype == arrTypeNode ){
        GInstall( t->children[0]->varname, "", type, t->children[1]->val, -1);
        t->children[0]->Gentry = GLookup(t->children[0]->varname);
        t->children[0]->type = type;
    }
    else if( t->nodetype == funcTypeGDeclNode ){
        GInstall( t->children[0]->varname, "", type, 0, globalFlabel++);
        t->children[0]->Gentry = GLookup(t->children[0]->varname);
        t->children[0]->type = type;
        if ( t->children[1] )
            setGTypes( t -> children[1], type, t->children[0]->Gentry);
    }
    else if( t->nodetype == paramNode ){
        struct Paramstruct * param = (struct Paramstruct*)malloc(sizeof(struct Paramstruct));
        param->name = (char*)malloc(sizeof(char)*20);
        strcpy(param->name, t->children[1]->varname);
        param->type = t->children[0]->type;
        param->checked = 0;
        param->next = NULL;
        insertIntoParamList(paramHead, param);
    }
    else{
        printf("Invalid variable in declarations!\n");
        exit(1);
    }
    // else if( t->nodetype == arr2dTypeNode ){
    //     Install( t->children[0]->varname, type, t->children[1]->val, t->children[2]->val);
    //     t->children[0]->Gentry = GLookup(t->children[0]->varname);
    //     t->children[0]->type = type;
    // }
}

void printGSymbolTable(){
    struct Gsymbol * g = symbolTable;
    printf("\nGLOBAL SYMBOL TABLE: \n");
    printf("| name | tupname | type | size | binding | flabel| paramlist\n");
    while(g){
        printX(g->name, 7, -1,  strType);
        printX(g->tupName, 10, -1,  strType);
        printX("",7 ,g->type, intType);
        printX("",7 ,g->size, intType);
        printX("",10 ,g->binding, intType);
        printX("",8 ,g->flabel, intType);
        struct Paramstruct* temp = g->paramlist;
        while(temp){
            printf("| (%s,%d)", temp->name, temp->type);
            temp = temp->next;
        }
        printf("\n");
        g = g->next;
    }
}

void check(int type1, int type2, char * mssg){
    if ( type2 == invalidType){
        printf("Undeclared %s\n", mssg);
        exit(1);
    }
    if ( type1 != type2 ){
        printf("%s\n", mssg);
        printf("Type Mismatch Error\n");
        exit(1);
    }
}

void addBindingAddr(struct Lsymbol* Lentry){
    int addr = 1;
    struct Lsymbol* t = Lentry;
    while (t){
        t->binding = addr;
        if (t->type != tupleType) addr++;
        struct Paramstruct * p = t->paramlist;
        while(p){ addr++; p=p->next;}
        t = t->next;
    }
}

// ------------------------TUPLE FUNCTIONS----------------------------------------------------------

void addTupleGST(struct tnode* t, struct tnode* paramNode, char* tupname){
    
    if ( t->nodetype == connectorNode ){
        addTupleGST(t->children[0], paramNode, tupname);
        addTupleGST(t->children[1], paramNode, tupname);
    } 
    else if( t->nodetype == idNode ){
        GInstall( t->varname, tupname, tupleType, 0, -1);
        struct Gsymbol * g = GLookup(t->varname);

        // Settting up params
        setGTypes(paramNode,  tupleType, g);

        // Calculating Size of each struct
        int size = 0;
        struct Paramstruct * p = g->paramlist;
        while(p){ size++; p = p->next; }
        g->size  = size;
        SP += size;

    }

}

void addTupleLST(struct tnode* t, struct tnode* paramNode, char* tupname, struct tnode* head){
    
    if ( t->nodetype == connectorNode ){
        addTupleLST(t->children[0], paramNode, tupname, head);
        addTupleLST(t->children[1], paramNode, tupname, head);
    } 
    else if( t->nodetype == idNode ){
        LInstall( t->varname, tupname, tupleType, head);
        struct Lsymbol * temp = LLookup(t->varname, head->Lentry);

        // Settting up params
        setTupleLSTMembers(paramNode,  tupleType, temp);

        // Calculating Size of each struct
        int size = 0;
        struct Paramstruct * p = temp->paramlist;
        while(p){ size++; p = p->next; }

    }
}

void setTupleLSTMembers(struct tnode* t, int type, struct Lsymbol* paramHead){
    if ( t->nodetype == connectorNode ){
        setTupleLSTMembers( t -> children[0], type, paramHead);
        setTupleLSTMembers( t -> children[1], type, paramHead);
    }
    else if( t->nodetype == paramNode ){
        struct Paramstruct * param = (struct Paramstruct*)malloc(sizeof(struct Paramstruct));
        param->name = (char*)malloc(sizeof(char)*20);
        strcpy(param->name, t->children[1]->varname);
        param->type = t->children[0]->type;
        param->checked = 0;
        param->next = NULL;

        insertIntoParamListLST(paramHead, param);
    }
}

void insertIntoParamListLST(struct Lsymbol* paramHead, struct Paramstruct* cur){
    
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

// ------------------------LOCAL SYMBOL TABLE-------------------------------------------------------

struct Lsymbol * LInstall(char *name, char* tupname, int type, struct tnode* head){

    struct Lsymbol * cur = (struct Lsymbol *)malloc(sizeof(struct Lsymbol));
    cur->name = (char*)malloc(sizeof(char)*20);
    strcpy(cur->name, name);

    cur->tupname = (char*)malloc(sizeof(char)*20);
    strcpy(cur->tupname, tupname);

    cur->type = type;
    cur->paramlist = NULL;
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

void setLTypes(int type, struct tnode * t, struct tnode* head){
    if ( t->nodetype == connectorNode ){
        setLTypes( type, t -> children[0], head);
        setLTypes( type, t -> children[1], head);
    }
    else if ( t->nodetype == idNode ){
        struct Lsymbol * l = LInstall( t->varname, "", type, head );
        t->Lentry = l;
        t->type = type;
    }
}

void printLSymbolTable(struct Lsymbol * l ){
    struct Lsymbol * t = l;
    printf("| name | tupname | type | binding | paramlist \n");
    while(t){
        printX(t->name, 7, -1,  strType);
        printX(t->tupname, 10, -1,  strType);
        printX("", 7, t->type,  intType);
        printX("", 7, t->binding,  intType);
        struct Paramstruct* temp = t->paramlist;
        while(temp){
            printf("| (%s,%d)", temp->name, temp->type);
            temp = temp->next;
        }
        printf("\n");
        t = t->next;
    }
    printf("\n");

}

struct Lsymbol * joinLsymbols(struct Lsymbol * Lentry1, struct Lsymbol * Lentry2){
    struct Lsymbol * t = Lentry1;
    while (t->next){
        t = t->next;
    }
    t->next = Lentry2;
    return Lentry1;
}

// ---------------------------FUNC DECL-------------------------------------------------------------

void checkFDef(struct tnode ** c, struct Gsymbol *  funcEntry){
    if ( funcEntry->type != c[0]->type ){
        printf("Return type mismatch\n");
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

void assignVarTypes(struct tnode* t, struct Lsymbol * Lentry, int retType){
    // Recursive Calls
    if ( t->nodetype == tupleAccessNode )
        assignVarTypes(t->children[0], Lentry, retType);
    else
        for (int i = 0; i < t->childcount; i++ ){
            if (t->children[i]) 
                assignVarTypes(t->children[i], Lentry, retType);
        }
    switch (t->nodetype){
        case idNode:{       struct Lsymbol * t1 = LLookup( t->varname , Lentry );
                            struct Gsymbol * t2 = GLookup( t->varname );
                            // printLSymbolTable(Lentry);
                            if ( t1 != NULL ) t->Lentry = t1;
                            else if ( t2 != NULL ) t->Gentry = t2;
                            else{
                                printf("Undeclared Variable: %s\n", t->varname);
                                exit(1);
                            }
                            break; }
        case arrTypeNode:   // Checking Index Type
                            check(t->children[1]->type, intType, "Indexing non-int address");
                            // Assigning Type information
                            t->Lentry = t->children[0]->Lentry;
                            t->Gentry = t->children[0]->Gentry;
                            break;
                            
        case assignNode:    if (t->children[0]->nodetype == idNode ) 
                                check(  findType(t->children[0]), 
                                        t->children[1]->type, 
                                        "Assignment Type Wrong" );
                            break;

        case derefNode:     check( t->children[0]->type, intType, "Dereferencing non int address" );
                            t->Lentry = t->children[0]->Lentry;
                            t->Gentry = t->children[0]->Gentry;
                            t->type = findType(t->children[0]);
                            break;
        case addNode:   
        case mulNode:   
        case divNode:   
        case subNode:       check(t->children[0]->type, intType, "Mismatched operands");
                            check(t->children[1]->type, intType, "Mismatched operands");
                            break;
        case ifNode:    
        case whileNode: 
                            check(t->children[0]->type, boolType, "If / Loop Condition Not Boolean");
                            break;
        case dowhileNode:
        case repeatNode:    check(t->children[1]->type, boolType, "Loop Condition Not Boolean");
                            break;
        case returnNode:    check(t->children[0]->type, retType, "Return Type Does not match");
                            break;
        case funcCallNode:{ t->type = findType(t->children[0]);
                            if (t->children[1]){
                                struct Paramstruct* argList = fetchArgList(t->children[1]); 
                                if ( verifyArgTypes(argList, t->children[0]->Gentry->paramlist)  == 0 ){
                                    printf("Wrong Args passed to %s\n", t->children[0]->Gentry->name);
                                    exit(1);
                                } 
                            }
                            break;
                            }
    } 
}

int findType(struct tnode * t){
    if ( t->Lentry != NULL ){
        return t->Lentry->type;
    }
    if ( t->Gentry != NULL ){
        return t->Gentry->type;
    }
    return invalidType;
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
        struct Paramstruct* entry = searchParamList( paramlist, t->children[1]->varname );
        if ( entry == NULL ){
            printf("Function Parameter not found: %s\n", t->children[1]->varname);
            exit(1);
        }
        if ( entry -> checked ){
            printf("Variable declared twice/ Function declared twice: %s\n", t->children[1]->varname);
            exit(1);
        }
        if ( entry -> type != t->children[0]->type ){
            printf("Type Mismatch in Function Declaration: %s\n", t->children[1]->varname);
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
    if ( t!= NULL && t->nodetype != GDeclNode){
        for (int i=0;i<depth;i++) printf("- ");
        // if (p) printf("%s -> %s\n", printNode(p), printNode(t) );
        // else printf("%s\n", printNode(t) );
        int a = -1,b = -1;
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
        case eqNode: return "eq";
        case neNode: return "ne";
        case ifNode: return "if";
        case whileNode: return "while";
        case dowhileNode: return "dowhile";
        case repeatNode: return "repeat";
        case breakNode: return "break";
        case contNode: return "continue";
        case intTypeNode: return "intType";
        case strTypeNode: return "strType";
        case arrTypeNode: return "arrType";
        case arr2dTypeNode: return "arr2DType";
        case derefNode: return "dereference";
        case brkpNode: return "BRKP";
        case funcTypeGDeclNode: return "funcTypeGDeclNode";
        case paramNode: return "paramNode";
        case GDeclNode: return "GDeclNode";
        case rootNode: return "rootNode";
        case FDefNode: return "FDefNode";
        case LDeclNode: return "LDeclNode";
        case returnNode: return "returnNode";
        case FBodyNode: return "FBodyNode";
        case funcCallNode: return "funcCallNode";
        case argNode: return "argNode";
        case orNode: return "orNode";
        case andNode: return "andNode";
        case tupleNode: return "tupleNode";
        case tupleAccessNode: return "tupleAccessNode";
        case strConstNode: printf("%s : ", t->varname); return "strConst";
        case numNode: printf("%d : ", t->val); return "Num";
        case idNode: printf("%s : ", t->varname); return "Id";
        return "Uninitialized X";
    }
}

char * printType( int t ){
    switch(t){
        case strType : return "strType ";
        case intType : return "intType ";
        case strArrType : return "strArrType ";
        case intArrType : return "intArrType ";
        case boolType : return "boolType ";
        case invalidType : return "invalidType ";    
        case tupleType : return "tupleType ";    
    }
    return "";
}

void printX(char * s, int X, int val, int type){
    int n;
    if ( type == strType ){
        n = strlen(s);
        printf("| %s", s);
        for (int i=0;i<X-n-2;i++) printf(" ");
    }
    if ( type == intType ){
        char str[30];
        sprintf(str, "%d", val);
        n = strlen(str);
        printf("| %s", str);
        for (int i=0;i<X-n-2;i++) printf(" ");
    }
}

// -------------------------------------------------------------------------------------------------
