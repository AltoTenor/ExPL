#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "exprtree.h"

void check(struct tnode* t, int type){
    if ( t->type != type ){
        printf("Type Mismatch Error: %d != %d \n", t->type, type);
        exit(1);
    }
}

/*Creates and initializes a new node in the expression tree*/
struct tnode* createTree(   int val, 
                            int vartype, 
                            char* c, 
                            int nodetype, 
                            struct tnode *l, 
                            struct tnode *r ){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    switch (nodetype){
        case idNode:    temp->varname= (char*)malloc(sizeof(char));
                        *(temp->varname) = *(c);
                        temp->type = 0;
                        break;

        case numNode:   temp->val = val;
                        break;
    
        case addNode:   
        case mulNode:   
        case divNode:   
        case subNode:   check(l, intType);
                        check(r, intType);
                        break;
        case assignNode:check(r, intType);
                        break;
        case ifNode:    check(l, boolType);
                        break;

    }
    temp->type = vartype;
    temp->left = l;
    temp->right = r;
    temp->nodetype = nodetype;
    return temp;
}

/*Prints the expression tree in a pre-order traversal */
void printTree(struct tnode* t){
    if ( t!= NULL ){
        printf("%s\n", printNode(t) );
        printTree(t->left);
        printTree(t->right);
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
        case geNode: return "ge";
        case leNode: return "le";
        case gtNode: return "gt";
        case ltNode: return "lt";
        case eqNode: return "eq";
        case neNode: return "ne";
        case ifNode: return "ifNode";
        case ifstmtNode: return "ifstmtNode";
        case whileNode: return "whileNode";
        case numNode: printf("%d ->", t->val); return "Num";
        case idNode: printf("%s ->", t->varname); return "Id";
    }
}

// Function to evaluate an expression tree and prefix print
int arrpos( struct tnode * t ){
    return *(t->varname)-'a';
} 

int evaluate(struct tnode *t, int * ans){
    if ( t!= NULL){
        switch ( t->nodetype ){
            case readNode:  {
                scanf("%d", ans + arrpos(t->left) );
                return 0;
            }
            case writeNode:{
                int ev = evaluate(t->left, ans);
                printf("%d\n", ev );
                return 0;
            }
            case connectorNode: {
                evaluate(t->left, ans);
                evaluate(t->right, ans);
                return 0;
            }
            case assignNode:{
                ans[arrpos(t->left)] = evaluate(t->right, ans); 
                return 0;
            }
            case ifNode:{
                int cond = evaluate(t->left,ans);
                if ( cond ){
                    evaluate(t->right->left,ans);
                }
                else if ( t->right->right != NULL ) {
                    evaluate(t->right->right,ans);
                }
                return 0;
            }
            case whileNode:{
                while ( evaluate(t->left, ans) ) {
                    evaluate(t->right, ans);
                }
                return 0;
            }
            case addNode:{
                return evaluate(t->left, ans) + evaluate(t->right, ans);
            }
            case mulNode:{
                return evaluate(t->left, ans) * evaluate(t->right, ans);
            }
            case subNode:{
                return evaluate(t->left, ans) - evaluate(t->right, ans);
            }
            case divNode:{
                return evaluate(t->left, ans) / evaluate(t->right, ans);
            }
            case eqNode:{
                return evaluate(t->left, ans) == evaluate(t->right, ans);
            }
            case neNode:{
                return evaluate(t->left, ans) != evaluate(t->right, ans);
            }
            case leNode:{
                return evaluate(t->left, ans) <= evaluate(t->right, ans);
            }
            case ltNode:{
                return evaluate(t->left, ans) < evaluate(t->right, ans);
            }
            case geNode:{
                return evaluate(t->left, ans) >= evaluate(t->right, ans);
            }
            case gtNode:{
                return evaluate(t->left, ans) > evaluate(t->right, ans);
            }
            case numNode: return t->val;
            case idNode: return ans[arrpos(t)];
        }
    }
    return -10000;
}

