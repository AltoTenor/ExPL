#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "exprtree.h"
// #include "codegen.h"

FILE *fp;

// int pos = 0;

// /* Getting a free register if available */
// int getReg(){ 
//     if ( pos == 19 ) {printf("Out of Registers");exit(1);}
//     return pos++;
// }

// /* Releasing the highest indexed register used */
// int freeReg(){ pos>0 && pos--; }

// /* Some generic start up code containing read write and exit function calls and setting up stack */
// void initialize(){
//     fp = fopen("temp.xsm", "w");
//     fprintf(fp, "0\n2146\n0\n0\n0\n0\n0\n0\n");

//     // Exit System Call
//     fprintf(fp,"MOV     R1,\"Exit\"\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"CALL    0\n");

//     // Read System Call 
//     fprintf(fp,"PUSH    R0\n");
//     fprintf(fp,"PUSH    R1\n");

//     fprintf(fp,"MOV     R1 , \"Read\"\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"MOV     R1 , -2\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"MOV     R1 , R19\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"CALL    0\n");
//     fprintf(fp,"POP     R0\n");
//     fprintf(fp,"POP     R1\n");
//     fprintf(fp,"POP     R1\n");
//     fprintf(fp,"POP     R1\n");
//     fprintf(fp,"POP     R1\n");

//     fprintf(fp,"POP     R1\n");
//     fprintf(fp,"POP     R0\n");
//     fprintf(fp,"RET\n");

//     // Write System Call 
//     fprintf(fp,"PUSH    R0\n");
//     fprintf(fp,"PUSH    R1\n");

//     fprintf(fp,"MOV     R1, \"Write\"\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"MOV     R1, -2\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"MOV     R1, R19\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"PUSH    R1\n");
//     fprintf(fp,"CALL    0\n");
//     fprintf(fp,"POP     R0\n");
//     fprintf(fp,"POP     R1\n");
//     fprintf(fp,"POP     R1\n");
//     fprintf(fp,"POP     R1\n");
//     fprintf(fp,"POP     R1\n");

//     fprintf(fp,"POP     R1\n");
//     fprintf(fp,"POP     R0\n");
//     fprintf(fp,"RET\n");

//     // Main function
//     fprintf(fp, "BRKP\n");
//     fprintf(fp, "MOV SP, 4121\n");
// }

// int addr( struct tnode * t ){
//     return 4096+*(t->varname)-'a';
// } 

// /* Code Generation to XSM */
// int codeGen( struct tnode *t ) {
//     int i=-2,j=-2;
//     if ( t != NULL ){
//         // printf("%d %d \n", t->nodetype, t->nodetype & 1);
        
//         // Just a connector node ( just propagates call )
//         if ( t->nodetype == connectorNode ){
//             codeGen(t->left);
//             codeGen(t->right);
//         }
//         // Assignment Operator ( evaluates RHS expression and frees that register )
//         else if ( t->nodetype == assignNode ){
//             j = codeGen(t->right);
//             fprintf(fp, "MOV [%d], R%d\n", addr(t->left), j);
//             freeReg();
//         }
//         // Unary Operators grouped together
//         else if( (t->nodetype & 1) == 0 ){
//             switch( t->nodetype ){
//                 // Simple immediate MOV instruction
//                 case numNode:   i = getReg();   
//                                 fprintf(fp, "MOV R%d, %d\n", i, t->val);
//                                 break;
//                 // Value read from memory location (pointed by this node ) to new register
//                 case idNode :   i = getReg();   
//                                 fprintf(fp, "MOV R%d, [%d]\n", i, addr(t) );
//                                 break;
//                 // Read() from console to memory location (pointed by left and only child)
//                 case readNode:  fprintf(fp, "MOV R19, %d\n", addr(t->left) );
//                                 fprintf(fp, "CALL 2070\n");
//                                 break;
//                 // Write() to console from RHS expression (pointed by left and only child)
//                 case writeNode: i = codeGen(t->left);
//                                 fprintf(fp, "MOV R19, R%d\n", i);
//                                 fprintf(fp, "CALL 2108\n");
//                                 break;
//             }
//         }
//         // Arithmetic Operators grouped together
//         else if ( (t->nodetype & 1) == 1){
//             i = codeGen(t->left);
//             j = codeGen(t->right);
//             switch( t->nodetype ){
//                 case addNode:   fprintf(fp, "ADD R%d, R%d\n", i, j);
//                                 break;
//                 case mulNode:   fprintf(fp, "MUL R%d, R%d\n", i, j);
//                                 break;
//                 case subNode:   fprintf(fp, "SUB R%d, R%d\n", i, j);
//                                 break;
//                 case divNode:   fprintf(fp, "DIV R%d, R%d\n", i, j);
//                                 break;
//             }
//             freeReg();
//         }
//     }
//     return i;
// }
