#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "analyser.h"
#include "codegen.h"

FILE *fp;

int register_index = 0;
int label_index = 0;
extern int SP;

/* Getting a free register if available */
int getReg(){ 
    if ( register_index == 19 ) {printf("Out of Registers");exit(1);}
    return register_index++;
}

/* Releasing the highest indexed register used */
int freeReg(){ if(register_index>0) register_index--; }

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
    fprintf(fp,"MOV     R1 , -1\n");
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

}

int findParam(struct Paramstruct * p, char * name){
    while (p){
        if ( strcmp(p->name, name) == 0 ) return 1;
        p = p->next;
    }
    return 0;
}

/* Code Generation to XSM */
int codeGen( struct tnode *t , struct Context * c) {

    int i=-2,j=-2, k=-2;
    if ( t != NULL ){
        // printf("%d %d \n", t->nodetype, t->nodetype & 1);
        if( (t->nodetype & 1) == 0 ){
            switch( t->nodetype ){

                // Checked -
                case connectorNode: {
                    codeGen(t->children[0], c);
                    codeGen(t->children[1], c);
                    break;
                }
                // Checked -
                case FDefNode: {
                    if ( strcmp(t->children[1]->varname, "main") == 0 ){
                        fprintf(fp, "MAIN:\n");
                        fprintf(fp, "MOV SP, %d\n", SP);
                        fprintf(fp, "MOV BP, SP\n");

                        struct Lsymbol * l = t->Lentry;
                        int inc = 0;
                        while ( l ){
                            inc++;
                            l = l->next;
                        }
                        fprintf(fp, "ADD SP, %d\n", inc); 

                        c->mainFunc = 1;
                        codeGen(t->children[3], c);

                        for (int x=0;x<inc;x++) fprintf(fp, "POP R0\n");

                        fprintf(fp, "JMP EXIT\n");
                    }
                    else if ( t->Gentry ) {
                        fprintf(fp, "F%d:\n", t->Gentry->flabel);
                        fprintf(fp, "PUSH BP\n");
                        fprintf(fp, "MOV BP, SP\n");
                        
                        // Arguments getting pushed in 
                        struct Lsymbol * l = t->Lentry;
                        int paramnum = 0;
                        int inc = 0;
                        i = getReg();
                        while ( l ){
                            inc++;
                            if ( findParam(t->Gentry->paramlist, l->name) ){
                                paramnum++;
                                fprintf(fp, "MOV R%d, BP\n", i);
                                fprintf(fp, "SUB R%d, %d\n", i, paramnum+2);
                                fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                            }
                            fprintf(fp, "PUSH R%d\n", i);
                            l = l->next;
                        }
                        freeReg();

                        // Function Body Code Generated
                        c->mainFunc = 0;
                        codeGen(t->children[3], c);
                        // Calculate the return expression and store the value in [BP-2]
                        // Done by Return Node

                        // Pop out the local variables from the stack
                        for (int x=0;x<inc;x++) fprintf(fp, "POP R0\n");

                        // set BP to the old value of BP in the stack
                        fprintf(fp, "POP BP\n");
                        fprintf(fp, "RET\n");
                    }
                    break;
                }
                // Checked -
                case FBodyNode: {
                    codeGen(t->children[1], c);
                    break;
                }
                
                // Checked - Adding a breakpoint
                case brkpNode: {
                    fprintf(fp, "BRKP\n" );
                    break;
                }

                // Assignment Operator ( evaluates RHS expression and frees that register )
                case assignNode:{
                    i = codeGen(t->children[0], c);
                    j = codeGen(t->children[1], c);
                    fprintf(fp, "MOV [R%d], R%d\n", i, j );
                    freeReg();
                    freeReg();
                    break;
                }

                // If then else construct
                case ifNode:{
                    int l1 = getLabel();
                    int l2;
                    int elseExists = (t->children[2] != NULL);
                    if (elseExists) l2 = getLabel();

                    // Code for condition check
                    i = codeGen(t->children[0], c);
                    // If condition is not true then jump to exit / else
                    fprintf(fp, "JZ  R%d, L%d\n", i, l1);
                    // If it is true then execute this part
                    codeGen(t->children[1], c);
                    // After true part is done exit
                    if (elseExists) fprintf(fp, "JMP L%d\n", l2);
                    // Else/Exit label
                    fprintf(fp, "L%d:\n", l1 );
                    // Else exists
                    codeGen(t->children[2], c);
                    // Exit label
                    if (elseExists) fprintf(fp, "L%d:\n", l2 );

                    freeReg();
                    break;
                }

                // While construct
                case whileNode:{     
                    int l1 = getLabel();
                    int l2 = getLabel();

                    // Label to restart loop
                    fprintf(fp, "L%d:\n", l1 );
                    // Check condition
                    i = codeGen(t->children[0], c);
                    // If condition fails jump to end
                    fprintf(fp, "JZ  R%d, L%d\n", i, l2);
                    // If condiiton suceeds execute code and pass labels
                    c->jumpLabels = (int *)malloc(2*sizeof(int));
                    c->jumpLabels[0] = l1;
                    c->jumpLabels[1] = l2;
                    codeGen(t->children[1], c);
                    // Jump to start
                    fprintf(fp, "JMP L%d\n", l1);
                    // Exit label
                    fprintf(fp, "L%d:\n", l2 );
                    freeReg();
                    break;
                }

                case breakNode:{     
                    fprintf(fp, "JMP L%d\n", c->jumpLabels[1]);
                    break;
                }

                case contNode:{     
                    fprintf(fp, "JMP L%d\n", c->jumpLabels[0]);
                    break;
                }      
                            
                // Do - While construct
                case dowhileNode:{   
                    int l1 = getLabel();
                    int l2 = getLabel();

                    // Label to restart loop
                    fprintf(fp, "L%d:\n", l1 );
                    // Execute code and pass labels
                    c->jumpLabels = (int *)malloc(2*sizeof(int));
                    c->jumpLabels[0] = l1;
                    c->jumpLabels[1] = l2;
                    codeGen(t->children[0], c);
                    // Check condition
                    i = codeGen(t->children[1], c);
                    // If condition succeeds jump to end
                    fprintf(fp, "JNZ  R%d, L%d\n", i, l1);
                    // Exit label
                    fprintf(fp, "L%d:\n", l2 );
                    freeReg();
                    break;
                }
                
                // Repeat-Until construct
                case repeatNode:{    
                    int l1 = getLabel();
                    int l2 = getLabel();

                    // Label to restart loop
                    fprintf(fp, "L%d:\n", l1 );
                    // Execute code and pass labels
                    c->jumpLabels = (int *)malloc(2*sizeof(int));
                    c->jumpLabels[0] = l1;
                    c->jumpLabels[1] = l2;
                    codeGen(t->children[0], c);
                    // Check condition
                    i = codeGen(t->children[1], c);
                    // If condition succeeds jump to end
                    fprintf(fp, "JZ  R%d, L%d\n", i, l1);
                    // Exit label
                    fprintf(fp, "L%d:\n", l2 );
                    freeReg();
                    break;
                }

                // Simple immediate MOV instruction
                case numNode:{       
                    i = getReg();
                    fprintf(fp, "MOV R%d, %d\n", i, t->val);
                    break;
                }
                case strConstNode:{       
                    i = getReg();   
                    fprintf(fp, "MOV R%d, %s\n", i, t->varname);
                    break;
                }
                
                case derefNode :{       
                    i = codeGen(t->children[0], c);   
                    fprintf(fp, "MOV R%d, [R%d]\n", i, i );
                    break;
                }

                // Checked
                case idNode :{       
                    i = getReg();
                    if ( t->Lentry != NULL ){
                        fprintf(fp, "MOV R%d, BP\n", i);
                        fprintf(fp, "ADD R%d, %d\n", i, t->Lentry->binding );
                    }
                    else if( t->Gentry != NULL )
                        fprintf(fp, "MOV R%d, %d\n", i, t->Gentry->binding  );
                    else{
                        printf("Error: No symbol entry\n");
                        exit(1);
                    }
                    break;
                }

                // Checked - Value read from memory location (pointed by this node ) to new register
                case arrTypeNode :{   
                    if ( t->Gentry == NULL ){
                        printf("Error: No symbol entry\n");
                        exit(1);
                    }
                    i = getReg();
                    j = codeGen(t->children[1], c);
                    fprintf(fp, "MOV R%d, %d\n", i, t->Gentry->binding );
                    fprintf(fp, "ADD R%d, R%d\n", i, j );
                    freeReg();
                    break;
                }

                // Checked - Read() from console to memory location (pointed by children[0] and only child)
                case readNode:{      
                    i = codeGen(t->children[0], c);
                    fprintf(fp, "MOV R19, R%d\n", i );
                    fprintf(fp, "CALL READ\n");
                    freeReg();
                    break;
                }
                                    
                // Checked - Write() to console from RHS expression (pointed by children[0] and only child)
                case writeNode:{     
                    i = codeGen(t->children[0], c);
                    fprintf(fp, "MOV R19, R%d\n", i);
                    fprintf(fp, "CALL WRITE\n");
                    freeReg();
                    break;
                }
            
                case funcCallNode:{
                    // Pushing Registers in Use into Stack
                    t->val = register_index;
                    for (int reg=0;reg<t->val;reg++){
                        fprintf(fp, "PUSH R%d\n", reg);
                    }
                    // If arglist is present Call CodeGen again to push Args
                    if ( t->children[1] ) codeGen(t->children[1], c);
                    
                    // Empty space for Ret Val
                    fprintf(fp, "PUSH R0\n");
                    
                    // Function Call 
                    register_index=0;
                    fprintf(fp, "CALL F%d\n", t->children[0]->Gentry->flabel);
                    register_index = t->val;
                    
                    // Storing Retval
                    i = getReg();
                    fprintf(fp, "POP R%d\n", i);
                    
                    // Calculate number of args to pop
                    struct Paramstruct * g = t->children[0]->Gentry->paramlist;
                    while (g){
                        fprintf(fp, "POP R19\n");
                        g = g->next;
                    }

                    // Restore Register Context
                    for (int x=t->val-1; x >=0 ; x -- ) fprintf(fp, "POP R%d\n", x);

                    break;
                }
                
                case argNode:{
                    i = codeGen(t->children[t->childcount-1], c);
                    fprintf(fp, "PUSH R%d\n", i);
                    freeReg();
                    if ( t->childcount == 2 ){
                        codeGen(t->children[0], c);
                    }
                    break;
                }
                
                case returnNode:{
                    i =  codeGen(t->children[0], c);
                    j = getReg();
                    if ( c->mainFunc == 0 ){
                        fprintf(fp, "MOV R%d, BP\n", j);
                        fprintf(fp, "SUB R%d, 2\n", j);
                        fprintf(fp, "MOV [R%d], R%d\n", j, i);
                    } 
                    freeReg();
                    break;
                }
            }
        }
        // Instructions requiring 2 registers
        else if ( (t->nodetype & 1) == 1){
            i = codeGen(t->children[0], c);
            j = codeGen(t->children[1], c);
            switch( t->nodetype ){
                case addNode:   fprintf(fp, "ADD R%d, R%d\n", i, j);
                                break;
                case mulNode:   fprintf(fp, "MUL R%d, R%d\n", i, j);
                                break;
                case subNode:   fprintf(fp, "SUB R%d, R%d\n", i, j);
                                break;
                case divNode:   fprintf(fp, "DIV R%d, R%d\n", i, j);
                                break;
                case modNode:   fprintf(fp, "MOD R%d, R%d\n", i, j);
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
                case andNode:   fprintf(fp, "MUL  R%d, R%d\n", i, j);
                                break;
                case orNode:    fprintf(fp, "ADD  R%d, R%d\n", i, j);
                                break;
            }
            freeReg();
        }
    }
    return i;
}
