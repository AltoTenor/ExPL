/*
 * Filename: exprtree.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "exprtree.h"


/*Create a node tnode*/
struct tnode* createTreeNode(int val, int type, char* c, struct tnode *l, struct tnode *r){
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
