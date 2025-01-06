#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "exprtree.h"

struct Gsymbol * symbolTable;
int SP;
int *arr;

// Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.
struct Gsymbol *Lookup(char * name){
    struct Gsymbol * ptr = symbolTable;
    while ( ptr ){
        if ( strcmp(name, ptr->name) == 0 ) return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

// Creates a symbol table entry.
void Install(char *name, int type, int first, int second){

    if ( Lookup(name) != NULL ){ 
        printf("Variable redeclared!\n"); 
        exit(1); 
    }

    struct Gsymbol * temp = (struct Gsymbol *)malloc(sizeof(struct Gsymbol *));
    temp->name = (char*)malloc(32*sizeof(char));
    strcpy(temp->name, name);

    temp->size = (int*)malloc(8*sizeof(int));

    temp->size[0] = first;
    temp->size[1] = second;

    int prod = temp->size[0];
    if (temp->size[1]!=0) prod *= temp->size[1];

    temp->type = type;
    temp->binding = SP;
    temp->next = NULL;
    SP += prod;

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


void setTypes(struct tnode* t, int type){
    if ( t->nodetype == connectorNode ){
        setTypes( t -> children[0], type);
        setTypes( t -> children[1], type);
    }
    else if( t->nodetype == idNode ){
        Install( t->varname, type, 1, 0);
        t->Gentry = Lookup(t->varname);
        t->type = type;
    }
    else if( t->nodetype == arrTypeNode ){
        Install( t->children[0]->varname, type, t->children[1]->val, 0);
        t->children[0]->Gentry = Lookup(t->children[0]->varname);
        t->children[0]->type = type;
    }
    else if( t->nodetype == arr2dTypeNode ){
        Install( t->children[0]->varname, type, t->children[1]->val, t->children[2]->val);
        t->children[0]->Gentry = Lookup(t->children[0]->varname);
        t->children[0]->type = type;
    }
    else{
        printf("Invalid variable in declarations!\n");
        exit(1);
    }
}

void printSymbolTable(){
    struct Gsymbol * g = symbolTable;
    printf("name | type | size | binding\n");
    while(g){
        printf("%s | %d | (",g->name, g->type);
        for (int i=0;i<3;i++){
            if ( g->size[i]!=0 ) printf(" %d ,", g->size[i]);
        }
        printf(") | %d\n", g->binding );
        g = g->next;
    }
}

// Checks if a node is of a certain type
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

/*Creates and initializes a new node in the expression tree*/
struct tnode* createTree(   int val, 
                            int vartype, 
                            char* c, 
                            int nodetype, 
                            struct tnode **children, 
                            int childcount,
                            struct Gsymbol * entry ){
    struct tnode *temp;
    char mssg[25];
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    switch (nodetype){

        case strConstNode:
        case idNode:    temp->varname= (char*)malloc(12*sizeof(char));
                        strcpy(temp->varname, c);
                        break;

        case numNode:   temp->val = val;
                        break;
    
        case addNode:   
        case mulNode:   
        case divNode:   
        case subNode:   check(children[0]->type, intType, "Mismatched operands");
                        check(children[1]->type, intType, "Mismatched operands");
                        break;

        case assignNode:sprintf(mssg, "Variable: %s", children[0]->varname);
                        check(children[0]->Gentry->type, children[1]->type, mssg);
                        break;
        
        case derefNode: check(children[0]->type, intType, "Derefencing non-int address");
                        break;

        case arrTypeNode:
                        check(children[1]->type, intType, "Indexing non-int address");
                        break;

        case ifNode:    
        case whileNode: check(children[0]->type, boolType, "Loop Condition Not Boolean");
                        break;

        case dowhileNode:
        case repeatNode: check(children[1]->type, boolType, "Loop Condition Not Boolean");
                        break;

    }
    temp->type = vartype;
    temp->children = children;
    temp->childcount = childcount;
    temp->nodetype = nodetype;
    temp->Gentry = entry;
    return temp;
}

/*Prints the expression tree in a pre-order traversal */
void printTree(struct tnode* t){
    if ( t!= NULL ){
        printf("%s\n", printNode(t) );
        for (int i = 0; i < (t->childcount) ;  i++){
            printTree(t->children[i]);
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
        case strConstNode: printf("%s -> ", t->varname); return "strConst";
        case numNode: printf("%d -> ", t->val); return "Num";
        case idNode: printf("%s -> ", t->varname); return "Id";
    }
}

// Function to evaluate an expression tree and prefix print
// int arrpos( struct tnode * t ){
//     return *(t->varname)-'a';
// } 

// int evaluate(struct tnode *t, int * ans){
//     if ( t!= NULL){
//         switch ( t->nodetype ){
//             case readNode:  {
//                 scanf("%d", ans + arrpos( t->children[0]) );
//                 return 0;
//             }
//             case writeNode:{
//                 int ev = evaluate(t->children[0], ans);
//                 printf("%d\n", ev );
//                 return 0;
//             }
//             case connectorNode: {
//                 evaluate(t->children[0], ans);
//                 evaluate(t->children[1], ans);
//                 return 0;
//             }
//             case assignNode:{
//                 ans[arrpos(t->children[0])] = evaluate(t->children[1], ans); 
//                 return 0;
//             }
//             case ifNode:{
//                 int cond = evaluate(t->children[0],ans);
//                 if ( cond ){
//                     evaluate(t->children[1],ans);
//                 }
//                 else if ( t->children[2] != NULL ) {
//                     evaluate(t->children[2],ans);
//                 }
//                 return 0;
//             }
//             case whileNode:{
//                 while ( evaluate(t->children[0], ans) ) {
//                     evaluate(t->children[1], ans);
//                 }
//                 return 0;
//             }
//             case addNode:{
//                 return evaluate(t->children[0], ans) + evaluate(t->children[1], ans);
//             }
//             case mulNode:{
//                 return evaluate(t->children[0], ans) * evaluate(t->children[1], ans);
//             }
//             case subNode:{
//                 return evaluate(t->children[0], ans) - evaluate(t->children[1], ans);
//             }
//             case divNode:{
//                 return evaluate(t->children[0], ans) / evaluate(t->children[1], ans);
//             }
//             case eqNode:{
//                 return evaluate(t->children[0], ans) == evaluate(t->children[1], ans);
//             }
//             case neNode:{
//                 return evaluate(t->children[0], ans) != evaluate(t->children[1], ans);
//             }
//             case leNode:{
//                 return evaluate(t->children[0], ans) <= evaluate(t->children[1], ans);
//             }
//             case ltNode:{
//                 return evaluate(t->children[0], ans) < evaluate(t->children[1], ans);
//             }
//             case geNode:{
//                 return evaluate(t->children[0], ans) >= evaluate(t->children[1], ans);
//             }
//             case gtNode:{
//                 return evaluate(t->children[0], ans) > evaluate(t->children[1], ans);
//             }
//             case numNode: return t->val;
//             case idNode: return ans[arrpos(t)];
//         }
//     }
//     return -10000;
// }


