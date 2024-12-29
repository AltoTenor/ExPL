/*
 * Filename: exprtree.c
 * Description: Implementation of an expression tree for arithmetic operations.
 *              This file contains functions to create and evaluate nodes of an 
 *              expression tree. The nodes can represent either operands (numbers)
 *              or operators (+, -, *, /).
 *
 * Functions:
 *   - makeLeafNode(int n): Creates a leaf node with a numeric value.
 *   - makeOperatorNode(char c, struct tnode *l, struct tnode *r): Creates an operator 
 *     node with a specified operator and links to left and right child nodes.
 *   - evaluate(struct tnode *t): Recursively evaluates the expression tree and 
 *     returns the computed result. Also prints the evaluation steps.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "exprtree.h"


/*Create a node tnode*/
struct tnode* createTree(int val, int type, char* c, struct tnode *l, struct tnode *r){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    if ( type == Id ){
        temp->varname= (char*)malloc(sizeof(char));
        *(temp->varname) = *(c);
        temp->type = 0;
    }
    else if ( type == Num ){
        temp->val = val;
    }
    temp->left = l;
    temp->right = r;
    temp->nodetype = type;
    return temp;
}

void printTree(struct tnode* t){
    if ( t!= NULL ){
        printf("%s\n", printNode(t) );
        printTree(t->left);
        printTree(t->right);
    }
}

char * printNode( struct tnode* t ){
    switch ( t->nodetype ){
        case Read: return "Read";
        case Write: return "Write";
        case Connector: return "Connector";
        case Assign: return "Assign";
        case Add: return "Add";
        case Mul: return "Mul";
        case Num: printf("%d ->", t->val); return "Num";
        case Id: printf("%s ->", t->varname); return "Id";
    }
}

int arrpos( struct tnode * t ){
    return *(t->varname)-'a';
} 

// Function to evaluate an expression tree and prefix print
int evaluate(struct tnode *t, int * ans){
    if ( t!= NULL){
        switch ( t->nodetype ){
            case Read:  {
                scanf("%d", ans + arrpos(t->left) );
                return 0;
            }
            case Write:{
                int ev = evaluate(t->left, ans);
                printf("%d\n", ev );
                return 0;
            }
            case Connector: {
                evaluate(t->left, ans);
                evaluate(t->right, ans);
                return 0;
            }
            case Assign:{
                ans[arrpos(t->left)] = evaluate(t->right, ans); 
            }
            case Add:{
                return evaluate(t->left, ans) + evaluate(t->right, ans);
            }
            case Mul:{
                return evaluate(t->left, ans) * evaluate(t->right, ans);
            }
            case Num: return t->val;
            case Id: return ans[arrpos(t)];
        }
    }
}

// void prefix(struct tnode *t){
//     if(t->op == NULL)
//     {
//         printf("%d ", t->val);
//     }
//     else{
//         printf("%s ", t->op);
//         prefix(t->left);
//         prefix(t->right);
//     }
// }

// void postfix(struct tnode *t){
//     if(t->op == NULL)
//     {
//         printf("%d ", t->val);
//     }
//     else{
//         postfix(t->left);
//         postfix(t->right);
//         printf("%s ", t->op);
//     }
// }

