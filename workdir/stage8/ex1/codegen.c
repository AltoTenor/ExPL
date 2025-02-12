#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "analyser.h"
#include "codegen.h"

FILE *fp;

int register_index = 0;
int label_index = 0;
extern struct Classtable * classTable;
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

    // Setting up VFuncTable
    struct Classtable * CEntry = classTable;
    while( CEntry ){
        int index = 0;
        struct MemberFunclist * method = CEntry->vFuncptr;
        // Fetching the base of the class
        int classBase = 4096 + CEntry->classIndex*8;
        while(method){
            fprintf(fp, "MOV [%d], F%d\n", classBase+(index++), method->flabel);
            method = method -> next;
        }
        // Unused entries
        while(index<8){ fprintf(fp, "MOV [%d], -1\n", classBase+(index++)); }
        CEntry = CEntry->next;
    }

    // Jmp to Main Label
    fprintf(fp, "MOV SP, %d\n", SP);
    fprintf(fp, "CALL MAIN\n");


    // Exit System Call
    fprintf(fp,"EXIT:\n");
    fprintf(fp,"MOV     R1,\"Exit\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"ADD     SP, 4\n");

    // fprintf(fp,"PUSH    R1\n");
    // fprintf(fp,"PUSH    R1\n");
    // fprintf(fp,"PUSH    R1\n");
    // fprintf(fp,"PUSH    R1\n");
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
    // Same poping Registers
    fprintf(fp,"SUB     SP, 5\n");
    
    // fprintf(fp,"POP     R0\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");

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
    fprintf(fp,"SUB     SP, 5\n");
    
    // fprintf(fp,"POP     R0\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");

    fprintf(fp,"POP     R1\n");
    fprintf(fp,"POP     R0\n");
    fprintf(fp,"RET\n");

    // Initialize Call
    fprintf(fp,"INIT:\n");

    fprintf(fp,"MOV     R1, \"Heapset\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"CALL    0\n");
    fprintf(fp,"SUB     SP, 5\n");
    
    // fprintf(fp,"POP     R0\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    fprintf(fp,"RET\n");

    // Alloc Call
    fprintf(fp,"ALLOC:\n");

    fprintf(fp,"MOV     R1, \"Alloc\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1, \"8\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"CALL    0\n");
    fprintf(fp,"POP     R0\n");
    fprintf(fp,"SUB     SP, 4\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    

    fprintf(fp,"RET\n");

    // Free Call
    fprintf(fp,"FREE:\n");

    fprintf(fp,"MOV     R1, \"Free\"\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"MOV     R1, R19\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"PUSH    R1\n");
    fprintf(fp,"CALL    0\n");
    fprintf(fp,"SUB     SP, 5\n");
    
    // fprintf(fp,"POP     R0\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");
    // fprintf(fp,"POP     R1\n");

    fprintf(fp,"RET\n");

}

/* Finding if a parameter exists or not */
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
        if( (t->nodetype & 1) == 0 ){
            switch( t->nodetype ){

                // Propagate call to children
                case NODE_ADDR:
                case NODE_CONNECTOR: {
                    for (int x=0;x<t->childcount;x++)
                        i = codeGen(t->children[x], c);
                    break;
                }
                case NODE_CLASS_DEF: {
                    i = codeGen(t->children[3], c);
                    break;
                }
                case NODE_FBODY: {
                    codeGen(t->children[1], c);
                    break;
                }

                // Method Definition for a class
                case NODE_MDEF: {
                    // Find label for the function
                    struct MemberFunclist * method 
                        = MethodLookup(t->children[1]->name, t->Ctype, t->paramlist);
                    fprintf(fp, "F%d:\n", method->flabel);
                    fprintf(fp, "PUSH BP\n");
                    fprintf(fp, "MOV BP, SP\n");
                    
                    // Arguments & Parameters getting pushed in 
                    struct Lsymbol * l = t->Lentry;
                    int paramnum = 0;
                    int inc = 0;
                    i = getReg();
                    while ( l ){
                        // Fetching value of arguments to push 
                        if ( findParam(method->paramlist, l->name)
                            && l->type->generalType != CLASS_TYPE ){
                            paramnum++;
                            fprintf(fp, "MOV R%d, BP\n", i);
                            fprintf(fp, "SUB R%d, %d\n", i, paramnum+2);
                            fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                        }
                        // Even though USER_DEF has big size,
                        // only 1 field is required as they are stored to the heap memory
                        if ( l->type->generalType == USER_DEF ){
                            fprintf(fp, "PUSH R%d\n", i);
                            inc++;
                        }
                        // SELF / Class objects have to be fetched and pushed
                        else if ( l->type->generalType == CLASS_TYPE ){
                            paramnum+=2;
                            // Pushing Member Field Pointer
                            fprintf(fp, "MOV R%d, BP\n", i);
                            fprintf(fp, "SUB R%d, %d\n", i, paramnum+2);
                            fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                            fprintf(fp, "PUSH R%d\n", i);
                            // Pushing VFuncPtr
                            fprintf(fp, "MOV R%d, BP\n", i);
                            fprintf(fp, "SUB R%d, %d\n", i, paramnum+1);
                            fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                            fprintf(fp, "PUSH R%d\n", i);
                            inc+=2;
                        }
                        // Pushing tuples and Local Body Declarations and Args
                        else{
                            for (int x=0;x<l->type->size;x++){ 
                                fprintf(fp, "PUSH R%d\n", i);
                                inc++;
                            }
                        }
                        l = l->next;
                    }
                    freeReg();

                    // Function Body Code Generated
                    c->mainFunc = 0;
                    c->localvars = inc;
                    codeGen(t->children[3], c);

                    // In case of no return statement
                    // Pop out the local variables from the stack
                  
                    /* OPTIONAL CODE TO ALLOW VOID RETURNS */
                    for (int x=0; x<c->localvars; x++) fprintf(fp, "POP R0\n");

                    // set BP to the old value of BP in the stack
                    fprintf(fp, "POP BP\n");
                    fprintf(fp, "RET\n");
                    break;
                }
                
                // Function Definition
                case NODE_FDEF: {
                    if ( strcmp(t->children[1]->name, "main") == 0 ){
                        fprintf(fp, "MAIN:\n");
                        fprintf(fp, "MOV BP, SP\n");

                        struct Lsymbol * l = t->Lentry;
                        int inc = 0;
                        while ( l ){ 
                            if ( l->type->generalType == TYPE_TUPLE 
                                || l->type->generalType == CLASS_TYPE) inc += l->type->size; 
                            else inc++;
                            l = l->next; 
                        }
                        // Pushing Local Declaration Varaiables
                        fprintf(fp, "ADD SP, %d\n", inc); 

                        // Propagating to Function Body
                        c->mainFunc = 1;
                        c->localvars = inc;
                        codeGen(t->children[3], c);

                        // Popping the local declarations
                        fprintf(fp, "SUB SP, %d\n", c->localvars);
                        fprintf(fp, "RET\n");
                    }
                    else if ( t->Gentry ) {

                        fprintf(fp, "F%d:\n", t->Gentry->flabel);
                        fprintf(fp, "PUSH BP\n");
                        fprintf(fp, "MOV BP, SP\n");
                        
                        // Arguments & Parameters getting pushed in 
                        struct Lsymbol * l = t->Lentry;
                        int paramnum = 0;
                        int inc = 0;
                        i = getReg();
                        while ( l ){
                            // Fetching value of arguments to push 
                            if ( findParam(t->Gentry->paramlist, l->name) 
                                && l->type->generalType != CLASS_TYPE){
                                paramnum++;
                                fprintf(fp, "MOV R%d, BP\n", i);
                                fprintf(fp, "SUB R%d, %d\n", i, paramnum+2);
                                fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                            }
                            // Even though USER_DEF has big size,
                            // only 1 field is required as they are stored to the heap memory
                            if ( l->type->generalType == USER_DEF ){
                                fprintf(fp, "PUSH R%d\n", i);
                                inc++;
                            }
                            // SELF / Class objects have to be fetched and pushed
                            else if ( l->type->generalType == CLASS_TYPE ){
                                paramnum+=2;
                                // Pushing Member Field Pointer
                                fprintf(fp, "MOV R%d, BP\n", i);
                                fprintf(fp, "SUB R%d, %d\n", i, paramnum+2);
                                fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                                fprintf(fp, "PUSH R%d\n", i);
                                // Pushing VFuncPtr
                                fprintf(fp, "MOV R%d, BP\n", i);
                                fprintf(fp, "SUB R%d, %d\n", i, paramnum+1);
                                fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                                fprintf(fp, "PUSH R%d\n", i);
                                inc+=2;
                            }
                            // Pushing tuples and Local Body Declarations and Args
                            else{
                                for (int x=0;x<l->type->size;x++){ 
                                    fprintf(fp, "PUSH R%d\n", i);
                                    inc++;
                                }
                            }
                            l = l->next;
                        }
                        freeReg();

                        // Function Body Code Generated
                        c->mainFunc = 0;
                        c->localvars = inc;
                        codeGen(t->children[3], c);

                        // In case of no return statement
                        // Pop out the local variables from the stack
                        fprintf(fp, "SUB SP, %d\n", c->localvars);

                        // set BP to the old value of BP in the stack
                        fprintf(fp, "POP BP\n");
                        fprintf(fp, "RET\n");
                    }
                    break;
                }
                
                // Adding a breakpoint
                case NODE_BRKP: {
                    fprintf(fp, "BRKP\n" );
                    break;
                }

                // Assignment Operator ( evaluates RHS expression and frees that register )
                case NODE_ASSIGN:{
                    i = codeGen(t->children[0], c);

                    // When one tuple is being assigned completely to another
                    if ( t->children[0]->type->generalType == TYPE_TUPLE ){
                        struct Fieldlist * f = t->children[0]->type->fields;
                        j = codeGen(t->children[1]->children[0], c);
                        k = getReg();
                        while (f){
                            fprintf(fp, "MOV R%d, [R%d]\n", k, j );
                            fprintf(fp, "MOV [R%d], R%d\n", i, k );
                            fprintf(fp, "ADD R%d, 1\n", i );
                            fprintf(fp, "ADD R%d, 1\n", j );
                            f = f->next;
                        }
                        freeReg();
                    }
                    // When object is allocated new space in the heap and a new VFuncPtr
                    else if ( t->children[1]->nodetype == NODE_NEW ){
                        // Allocating space for Members and storing in Rj
                        for (int x = 0; x < register_index; x ++ ){
                            fprintf(fp, "PUSH R%d\n", x);
                        }
                        fprintf(fp, "CALL ALLOC\n");
                        j = getReg();
                        fprintf(fp, "MOV R%d, R0\n", j);

                        // Restoring Register Context
                        for (int x = register_index-2; x >= 0; x -- ){
                            fprintf(fp, "POP R%d\n", x);
                        }
                        // Move Heap address to first field
                        fprintf(fp, "MOV [R%d], R%d\n", i, j);
                        // Move VFuncPtr to second field
                        int ci = 4096 + t->children[1]->Ctype->classIndex*8;
                        fprintf(fp, "ADD R%d, 1\n", i);
                        fprintf(fp, "MOV [R%d], %d\n", i, ci);
                    }
                    // When one class is being assigned to another. Changes the heap base address
                    // and the VFuncPtr
                    else if ( t->children[1]->nodetype == NODE_EXPR
                            && t->children[1]->children
                            && t->children[1]->children[0]->Ctype
                            && t->children[1]->children[0]->nodetype == NODE_ID ){
                        j = codeGen(t->children[1]->children[0], c);
                        fprintf(fp, "MOV [R%d], [R%d]\n", i, j );
                        fprintf(fp, "ADD R%d,   1\n", i );
                        fprintf(fp, "ADD R%d,   1\n", j );
                        fprintf(fp, "MOV [R%d], [R%d]\n", i, j );
                    }
                    // Normal one variable assignment
                    else{
                        j = codeGen(t->children[1], c);
                        fprintf(fp, "MOV [R%d], R%d\n", i, j );
                    }
                    freeReg();
                    freeReg();
                    break;
                }

                // If then else construct
                case NODE_IF:{
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
                case NODE_WHILE:{     
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

                // Break JMP Condition
                case NODE_BREAK:{     
                    fprintf(fp, "JMP L%d\n", c->jumpLabels[1]);
                    break;
                }
                // Continue JMP Condition
                case NODE_CONTINUE:{     
                    fprintf(fp, "JMP L%d\n", c->jumpLabels[0]);
                    break;
                }      
                            
                // Do - While construct
                case NODE_DOWHILE:{   
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
                case NODE_REPEAT:{    
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
                case NODE_CONST_NUM:{       
                    i = getReg();
                    fprintf(fp, "MOV R%d, %d\n", i, t->value.intval);
                    break;
                }
                case NODE_CONST_STR:{       
                    i = getReg();   
                    fprintf(fp, "MOV R%d, %s\n", i, t->value.strval);
                    break;
                }
                
                // To get the value from the address
                case NODE_DEREF:
                case NODE_EXPR :{       
                    i = codeGen(t->children[0], c);   
                    fprintf(fp, "MOV R%d, [R%d]\n", i, i );
                    break;
                }

                // Retrieve the binding address for a variable
                case NODE_SELF:
                case NODE_ID :{       
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
                case NODE_ARR_TYPE :{   
                    if ( t->children[0]->Gentry == NULL ){
                        printf("Error: No symbol entry\n");
                        exit(1);
                    }
                    i = getReg();
                    j = codeGen(t->children[1], c);
                    fprintf(fp, "MOV R%d, %d\n", i, t->children[0]->Gentry->binding );
                    fprintf(fp, "ADD R%d, R%d\n", i, j );
                    freeReg();
                    break;
                }

                // Read Function Call
                case NODE_READ:{      
                    i = codeGen(t->children[0], c);
                    fprintf(fp, "MOV R19, R%d\n", i );
                    fprintf(fp, "CALL READ\n");
                    freeReg();
                    break;
                }
                // Write Function Call
                case NODE_WRITE:{     
                    i = codeGen(t->children[0], c);
                    fprintf(fp, "MOV R19, R%d\n", i);
                    fprintf(fp, "CALL WRITE\n");
                    freeReg();
                    break;
                }
            
                // Function/Method Calls
                case NODE_FUNC_CALL:{
                    // Pushing Registers in Use into Stack
                    t->value.intval = register_index;
                    for (int reg=0;reg<t->value.intval;reg++){
                        fprintf(fp, "PUSH R%d\n", reg);
                    }
                    // Will be used for popping stuff later
                    struct Paramstruct * g;

                    // Class method calls
                    if ( t->children[0]->Ctype ){
                        
                        // Pushing in the heap location of the object and VFuncPtr of the object
                        i = codeGen(t->children[0], c);
                        j = getReg();
                        fprintf(fp, "MOV R%d, [R%d]\n",j, i);
                        fprintf(fp, "PUSH R%d\n", j);
                        fprintf(fp, "ADD R%d, 1\n", i);
                        fprintf(fp, "MOV R%d, [R%d]\n",j, i);
                        fprintf(fp, "PUSH R%d\n", j);

                        // Finding out to which addr the call has to be made from the VFuncTable
                        struct MemberFunclist * method
                         = MethodLookup(t->children[0]->children[1]->name, 
                                        t->children[0]->Ctype, t->paramlist);
                        fprintf(fp, "ADD R%d, %d\n", j, method->funcposition);
                        
                        // If arglist is present Call CodeGen again to push Args
                        if ( t->children[1] ) codeGen(t->children[1], c);
                        
                        // Empty space for Ret Val
                        fprintf(fp, "PUSH R0\n");
                        
                        // Function Call 
                        register_index=0;
                        fprintf(fp, "MOV R%d, [R%d]\n",j, j);
                        fprintf(fp, "CALL R%d\n", j);

                        // Reset register to start (essentially deleting new regs)
                        register_index = t->value.intval;
                        
                        // Storing Retval
                        i = getReg();
                        fprintf(fp, "POP R%d\n", i);
                        
                        // Calculate number of args to pop
                        g = method->paramlist;

                        // Popping the object member and VFunctPtr 
                        fprintf(fp, "SUB SP, 2\n");
                    }
                    // Normal Function Calls
                    else{
                        // If arglist is present Call CodeGen again to push Args
                        if ( t->children[1] ) codeGen(t->children[1], c);
                        
                        // Empty space for Ret Val
                        fprintf(fp, "PUSH R0\n");
                        
                        // Function Call 
                        register_index=0;
                        fprintf(fp, "CALL F%d\n", t->children[0]->Gentry->flabel);
                        register_index = t->value.intval;
                        
                        // Storing Retval
                        i = getReg();
                        fprintf(fp, "POP R%d\n", i);
                        
                        // Calculate number of args to pop
                        g = t->children[0]->Gentry->paramlist;
                    }
                    
                    // Calculating number of arguments to pop
                    int argCnt = 0;
                    while (g){
                        if (g->type->generalType == CLASS_TYPE) argCnt+= g->type->size;
                        else argCnt++;
                        g = g->next;
                    }
                    // Popping Arguments
                    fprintf(fp, "SUB SP, %d\n", argCnt);
                    // Restore Register Context
                    for (int x=t->value.intval-1; x >=0 ; x -- ) fprintf(fp, "POP R%d\n", x);
                    break;
                }
                
                // Used for pushing arguments
                case NODE_ARG:{
                    // To handle objects being passed as arguments
                    if ( t->children[t->childcount-1]->children
                        && t->children[t->childcount-1]->nodetype == NODE_EXPR
                        && t->children[t->childcount-1]->children[0]->nodetype == NODE_ID
                        && t->children[t->childcount-1]->children[0]->Ctype ){
                        struct Classtable * CTEntry 
                            = t->children[t->childcount-1]->children[0]->Ctype;
                        i = codeGen(t->children[t->childcount-1]->children[0], c);
                        j = getReg();
                        fprintf(fp, "MOV  R%d, [R%d]\n", j, i);
                        fprintf(fp, "PUSH R%d\n", j);
                        fprintf(fp, "ADD  R%d, 1\n", i);
                        fprintf(fp, "MOV  R%d, [R%d]\n", j, i);
                        fprintf(fp, "PUSH R%d\n", j);
                        freeReg();
                        freeReg();
                    }
                    // Any other arguments are considered as 1 size 
                    else{
                        i = codeGen(t->children[t->childcount-1], c);
                        fprintf(fp, "PUSH R%d\n", i);
                        freeReg();
                        if ( t->childcount == 2 ){
                            codeGen(t->children[0], c);
                        }
                    }
                    break;
                }
                
                // Return Statements
                case NODE_RETURN:{
                    i =  codeGen(t->children[0], c);
                    if ( c->mainFunc == 0 ){
                        j = getReg();
                        // Store the return value
                        fprintf(fp, "MOV R%d, BP\n", j);
                        fprintf(fp, "SUB R%d, 2\n", j);
                        fprintf(fp, "MOV [R%d], R%d\n", j, i);
                        freeReg();
                    } 
                    // Pop out the local variables from the stack
                    for (int x=0;x<c->localvars;x++) fprintf(fp, "POP R0\n");
                    
                    // set BP to the old value of BP in the stack
                    if ( c->mainFunc == 0 ) fprintf(fp, "POP BP\n");
                    fprintf(fp, "RET\n");
                    freeReg();
                    break;
                }
                
                // Member access - TUPLE / CLASS / USER_DEFINED
                case NODE_MEMBER:{
                    // Handling Member access for Tuples
                    if ( t->children[0]->type->generalType == TYPE_TUPLE ){
                        if ( t->children[0]->nodetype == NODE_ID ){
                            // If it is the first variable then access the GST or LST to get address
                            // Store in the register "i" and pass it up
                            i = getReg();
                            if ( t->children[0]->Lentry != NULL ){
                                fprintf(fp, "MOV R%d, BP\n", i);
                                fprintf(fp, "ADD R%d, %d\n", i, t->children[0]->Lentry->binding );
                            }
                            else if( t->children[0]->Gentry != NULL ){
                                fprintf(fp, "MOV R%d, %d\n", i, t->children[0]->Gentry->binding  );
                            }
                            else{
                                printf("Error: No symbol entry\n");
                                exit(1);
                            }
                        }
                        else if ( t->children[0]->nodetype == NODE_MEMBER ){
                            // If it is a member then i has the address just need to access it
                            i = codeGen(t->children[0], c);
                            fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                        }
                        else { printf("Error in member access subtree\n"); exit(1);}
                        // Add the fieldindex value to the address 
                        struct Fieldlist * f = FLookup(t->children[0]->type, t->children[1]->name);
                        fprintf(fp, "ADD R%d, %d\n", i, f->fieldIndex );
                        break;
                    }
                    // Handling member access for User Defined Types
                    else if ( t->children[0]->type->generalType == USER_DEF ) {
                        // If it is a member then i has the address just need to access it
                        i = codeGen(t->children[0], c);
                        fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                        // Add the fieldindex value to the address 
                        struct Fieldlist * f = FLookup(t->children[0]->type, t->children[1]->name);
                        fprintf(fp, "ADD R%d, %d\n", i, f->fieldIndex );
                        break;
                    }
                    // Handling Object Member access and function calls
                    else if ( t->Ctype ){
                        i = codeGen(t->children[0], c);
                        struct Fieldlist * member = MemberLookup(t->children[1]->name, t->Ctype);
                        struct MemberFunclist * method
                            = MethodLookup(t->children[1]->name, t->Ctype, NULL);
                        if ( member != NULL ){
                            fprintf(fp, "MOV R%d, [R%d]\n", i, i);
                            fprintf(fp, "ADD R%d, %d\n", i, member->fieldIndex);
                        }
                        else if (method == NULL) { printf("Invalid Member Access\n"); exit(1); }
                        break;
                    }
                }

                // NULL is represented as -1 in the pointer location
                case NODE_NULL: {
                    i = getReg();
                    fprintf(fp, "MOV R%d, -1\n", i);
                    break;
                }

                // Call to Initialize Function
                case NODE_INIT: {
                    for (int x = 0; x < register_index; x ++ ){
                        fprintf(fp, "PUSH R%d\n", x);
                    }
                    fprintf(fp, "CALL INIT\n");
                    i = getReg();
                    fprintf(fp, "MOV R%d, R0\n", i);
                    for (int x = register_index-2; x >= 0; x -- ){
                        fprintf(fp, "POP R%d\n", x);
                    }
                    freeReg();
                    break;
                }
                // Call to Alloc Function
                case NODE_ALLOC: {
                    for (int x = 0; x < register_index; x ++ ){
                        fprintf(fp, "PUSH R%d\n", x);
                    }
                    fprintf(fp, "CALL ALLOC\n");
                    i = getReg();
                    fprintf(fp, "MOV R%d, R0\n", i);
                    for (int x = register_index-2; x >= 0; x -- ){
                        fprintf(fp, "POP R%d\n", x);
                    }
                    freeReg();
                    break;
                }
                // Call to Free Function
                case NODE_FREE: {
                    for (int x = 0; x < register_index; x ++ ){
                        fprintf(fp, "PUSH R%d\n", x);
                    }
                    i = codeGen(t->children[0], c);
                    fprintf(fp, "MOV R19, [R%d]\n", i);
                    fprintf(fp, "MOV [R%d], -1\n", i);
                    fprintf(fp, "CALL FREE\n");
                    fprintf(fp, "MOV R%d, R0\n", i);
                    for (int x = register_index-2; x >= 0; x -- ){
                        fprintf(fp, "POP R%d\n", x);
                    }
                    break;
                }
            
                // Deleting an object - Storing NULL in both locations
                case NODE_DELETE: {
                    for (int x = 0; x < register_index; x ++ ){
                        fprintf(fp, "PUSH R%d\n", x);
                    }
                    i = codeGen(t->children[0], c);
                    fprintf(fp, "MOV R19, [R%d]\n", i);
                    fprintf(fp, "MOV [R%d], -1\n", i);
                    fprintf(fp, "ADD  R%d,   1\n", i);
                    fprintf(fp, "MOV [R%d], -1\n", i);
                    fprintf(fp, "CALL FREE\n");
                    fprintf(fp, "MOV R%d, R0\n", i);
                    for (int x = register_index-2; x >= 0; x -- ){
                        fprintf(fp, "POP R%d\n", x);
                    }
                }
            }
        }
        // Instructions requiring 2 registers
        else if ( (t->nodetype & 1) == 1){
            i = codeGen(t->children[0], c);
            j = codeGen(t->children[1], c);
            switch( t->nodetype ){
                case NODE_ADD:   fprintf(fp, "ADD R%d, R%d\n", i, j);
                                break;
                case NODE_MUL:   fprintf(fp, "MUL R%d, R%d\n", i, j);
                                break;
                case NODE_SUB:   fprintf(fp, "SUB R%d, R%d\n", i, j);
                                break;
                case NODE_DIV:   fprintf(fp, "DIV R%d, R%d\n", i, j);
                                break;
                case NODE_MOD:   fprintf(fp, "MOD R%d, R%d\n", i, j);
                                break;
                case NODE_GE:    fprintf(fp, "GE  R%d, R%d\n", i, j);
                                break;
                case NODE_LE:    fprintf(fp, "LE  R%d, R%d\n", i, j);
                                break;
                case NODE_GT:    fprintf(fp, "GT  R%d, R%d\n", i, j);
                                break;
                case NODE_LT:    fprintf(fp, "LT  R%d, R%d\n", i, j);
                                break;
                case NODE_EQ:    fprintf(fp, "EQ  R%d, R%d\n", i, j);
                                break;
                case NODE_NE:    fprintf(fp, "NE  R%d, R%d\n", i, j);
                                break;
                case NODE_AND:   fprintf(fp, "MUL  R%d, R%d\n", i, j);
                                break;
                case NODE_OR:    fprintf(fp, "ADD  R%d, R%d\n", i, j);
                                break;
            }
            freeReg();
        }
    }
    return i;
}
