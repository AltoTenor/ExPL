#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "exprtree.h"

FILE *fp;

int pos = 0;

int getReg(){ 
    if ( pos == 20 ) {printf("Out of Registers");exit(1);}
    return pos++;
}
int freeReg(){ pos>0 && pos--; }

void initialize(){
    fp = fopen("temp.xsm", "w");
    fprintf(fp, "0\n2140\n0\n0\n0\n0\n0\n0\n");

    // Exit System Call
    fprintf(fp,"MOV     R1,\"Exit\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"CALL    0\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");

    // Read System Call 
    fprintf(fp,"MOV     R1 , \"Read\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1 , -2\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1 , 4096\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"CALL    0\n");
    fprintf(fp,"POP     R0\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"RET\n");

    // Write System Call 
    fprintf(fp,"MOV     R1, \"Write\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1, -2\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1, [4096]\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"CALL    0\n");
    fprintf(fp,"POP     R0\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"RET\n");

    // Main function
    fprintf(fp, "BRKP\n");
    fprintf(fp, "MOV SP, 4096\n");
}

int codeGen( struct tnode *t ) {
    int i,j;
    if ( t == NULL ) return -1;
    else if(t->op == NULL){
        i = getReg();
        fprintf(fp, "MOV R%d, %d\n", i, t->val);
    }
    else{
        i = codeGen(t->left);
        j = codeGen(t->right);
        switch(*(t->op)){
            case '+':   fprintf(fp, "ADD R%d, R%d\n", i, j);
                        break;
            case '*':   fprintf(fp, "MUL R%d, R%d\n", i, j);
                        break;
            case '-':   fprintf(fp, "SUB R%d, R%d\n", i, j);
                        break;
            case '/':   fprintf(fp, "DIV R%d, R%d\n", i, j);
                        break;
        }
        freeReg();
    }
    return i;
}
