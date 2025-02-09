#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "exprtree.h"
#include "codegen.h"

FILE *fp;

int register_index = 0;
int label_index = 0;

/* Getting a free register if available */
int getReg(){ 
    if ( register_index == 19 ) {printf("Out of Registers");exit(1);}
    return register_index++;
}

/* Releasing the highest indexed register used */
int freeReg(){ register_index>0 && register_index--; }

/* Getting a new label for Jump instructions */
int getLabel(){ return label_index++; }

/* Some generic start up code containing read write and exit function calls and setting up stack */
void initialize(){  
    fp = fopen("temp.xsm", "w");
    fprintf(fp, "0\n2056\n0\n0\n0\n0\n0\n0\n");

    // Jmp to Main Label
    fprintf(fp,"JMP     MAIN\n");


    // Exit System Call
    fprintf(fp,"EXIT:\n");
    fprintf(fp,"MOV     R1,\"Exit\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"CALL    0\n");

    // Read System Call 
    fprintf(fp,"READ:\n");
    fprintf(fp,"PUSH    R0\n");
    fprintf(fp,"PUSH    R1\n");

    fprintf(fp,"MOV     R1 , \"Read\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1 , -2\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1 , R19\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"CALL    0\n");
    fprintf(fp,"POP     R0\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");

    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R0\n");
    fprintf(fp,"RET\n");

    // Write System Call 
    fprintf(fp,"WRITE:\n");
    fprintf(fp,"PUSH    R0\n");
    fprintf(fp,"PUSH    R1\n");

    fprintf(fp,"MOV     R1, \"Write\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1, -2\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1, R19\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"CALL    0\n");
    fprintf(fp,"POP     R0\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R1\n");

    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R0\n");
    fprintf(fp,"RET\n");

    // Main function
    fprintf(fp, "MAIN:\n");
    fprintf(fp, "BRKP\n");
    fprintf(fp, "MOV SP, 4121\n");
}

int addr( struct tnode * t ){
    return 4096+*(t->varname)-'a';
} 

/* Code Generation to XSM */
int codeGen( struct tnode *t , int* jmpLabels) {

    int i=-2,j=-2;
    if ( t != NULL ){
        // printf("%d %d \n", t->nodetype, t->nodetype & 1);
        
        // Just a connector node ( just propagates call )
        if ( t->nodetype == connectorNode ){
            codeGen(t->children[0], jmpLabels);
            codeGen(t->children[1], jmpLabels);
        }
        // Assignment Operator ( evaluates RHS expression and frees that register )
        else if ( t->nodetype == assignNode ){
            j = codeGen(t->children[1], jmpLabels);
            fprintf(fp, "MOV [%d], R%d\n", addr(t->children[0]), j);
            freeReg();
        }
        // If then else construct
        else if ( t-> nodetype == ifNode ){
            int l1 = getLabel();
            int l2;
            int elseExists = (t->children[2] != NULL);
            if (elseExists) l2 = getLabel();

            // Code for condition check
            i = codeGen(t->children[0], jmpLabels);
            // If condition is not true then jump to exit / else
            fprintf(fp, "JZ  R%d, L%d\n", i, l1);
            // If it is true then execute this part
            codeGen(t->children[1], jmpLabels);
            // After true part is done exit
            if (elseExists) fprintf(fp, "JMP L%d\n", l2);
            // Else/Exit label
            fprintf(fp, "L%d:\n", l1 );
            // Else exists
            codeGen(t->children[2], jmpLabels);
            // Exit label
            if (elseExists) fprintf(fp, "L%d:\n", l2 );
        }
        // While construct
        else if ( t-> nodetype == whileNode ){
            int l1 = getLabel();
            int l2 = getLabel();

            // Label to restart loop
            fprintf(fp, "L%d:\n", l1 );
            // Check condition
            i = codeGen(t->children[0], jmpLabels);
            // If condition fails jump to end
            fprintf(fp, "JZ  R%d, L%d\n", i, l2);
            // If condiiton suceeds execute code and pass labels
            int * temp = (int *)malloc(2*sizeof(int));
            temp[0] = l1;
            temp[1] = l2;
            codeGen(t->children[1], temp);
            // Jump to start
            fprintf(fp, "JMP L%d\n", l1);
            // Exit label
            fprintf(fp, "L%d:\n", l2 );
        }
        else if ( t-> nodetype == breakNode ){
            fprintf(fp, "JMP L%d\n", jmpLabels[1]);
        }
        else if ( t-> nodetype == contNode ){
            fprintf(fp, "JMP L%d\n", jmpLabels[0]);
        }
        // Unary Operators grouped together
        else if( (t->nodetype & 1) == 0 ){
            switch( t->nodetype ){
                // Simple immediate MOV instruction
                case numNode:   i = getReg();   
                                fprintf(fp, "MOV R%d, %d\n", i, t->val);
                                break;
                // Value read from memory location (pointed by this node ) to new register
                case idNode :   i = getReg();   
                                fprintf(fp, "MOV R%d, [%d]\n", i, addr(t) );
                                break;
                // Read() from console to memory location (pointed by children[0] and only child)
                case readNode:  fprintf(fp, "MOV R19, %d\n", addr(t->children[0]) );
                                fprintf(fp, "CALL READ\n");
                                break;
                // Write() to console from RHS expression (pointed by children[0] and only child)
                case writeNode: i = codeGen(t->children[0], jmpLabels);
                                fprintf(fp, "MOV R19, R%d\n", i);
                                fprintf(fp, "CALL WRITE\n");
                                break;
            }
        }
        // Arithmetic Operators grouped together
        else if ( (t->nodetype & 1) == 1){
            i = codeGen(t->children[0], jmpLabels);
            j = codeGen(t->children[1], jmpLabels);
            switch( t->nodetype ){
                case addNode:   fprintf(fp, "ADD R%d, R%d\n", i, j);
                                break;
                case mulNode:   fprintf(fp, "MUL R%d, R%d\n", i, j);
                                break;
                case subNode:   fprintf(fp, "SUB R%d, R%d\n", i, j);
                                break;
                case divNode:   fprintf(fp, "DIV R%d, R%d\n", i, j);
                                break;
                case geNode:    fprintf(fp, "GE  R%d, R%d\n", i, j);
                                break;
                case leNode:    fprintf(fp, "LE  R%d, R%d\n", i, j);
                                break;
                case gtNode:    fprintf(fp, "GT  R%d, R%d\n", i, j);
                                break;
                case ltNode:    fprintf(fp, "LT  R%d, R%d\n", i, j);
                                break;
                case eqNode:    fprintf(fp, "EQ  R%d, R%d\n", i, j);
                                break;
                case neNode:    fprintf(fp, "NE  R%d, R%d\n", i, j);
                                break;
            }
            freeReg();
        }
    }
    return i;
}
