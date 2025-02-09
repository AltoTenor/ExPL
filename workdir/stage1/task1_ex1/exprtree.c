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
#include <stddef.h>
#include "exprtree.h"


// Function to create a leaf node in the expression tree
struct tnode* makeLeafNode(int n)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL;
    temp->val = n;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

// Function to create an operator node in the expression tree
struct tnode* makeOperatorNode(char c,struct tnode *l,struct tnode *r){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = malloc(sizeof(char));
    *(temp->op) = c;
    temp->left = l;
    temp->right = r;
    return temp;
}

// Function to evaluate an expression tree and prefix print
int evaluate(struct tnode *t){
    if(t->op == NULL)
    {
        return t->val;
    }
    else{
        int ans = 0;
        switch(*(t->op)){
            case '+' : ans = evaluate(t->left) + evaluate(t->right);
                       break;
            case '-' : ans = evaluate(t->left) - evaluate(t->right);
                       break;
            case '*' : ans = evaluate(t->left) * evaluate(t->right);
                       break;
            case '/' : ans = evaluate(t->left) / evaluate(t->right);
                       break;
        }
        return ans;
    }
}

void prefix(struct tnode *t){
    if(t->op == NULL)
    {
        printf("%d ", t->val);
    }
    else{
        printf("%s ", t->op);
        prefix(t->left);
        prefix(t->right);
    }
}

void postfix(struct tnode *t){
    if(t->op == NULL)
    {
        printf("%d ", t->val);
    }
    else{
        postfix(t->left);
        postfix(t->right);
        printf("%s ", t->op);
    }
}

