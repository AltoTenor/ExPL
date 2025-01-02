// typedef struct tnode{
//     int val; //value of the expression tree
//     char *op; //indicates the opertor
//     struct tnode *left,*right; //left and right branches
// }tnode;



typedef enum {
    readNode = 2,
    writeNode = 4,
    connectorNode = 0,
    assignNode = 5,

    idNode = 10,
    numNode = 12,

    addNode = 1,
    mulNode = 3,
    subNode = 7,
    divNode = 9,

    geNode = 11, 
    leNode = 13,
    gtNode = 15,
    ltNode = 17,
    eqNode = 19, 
    neNode = 21,

    ifNode = 23,
    ifstmtNode = 25,
    whileNode = 27,
    breakNode = 22,
    contNode = 24,

} NodeType;

typedef enum{
    intType,
    boolType,
    invalidType
} VarType;

typedef struct tnode {
    int val;        // value of a number for NUM nodes.
    int type;       // type of variable
    char* varname;  // name of a variable for ID nodes
    int nodetype;   // information about non-leaf nodes - read/write/connector/+/* etc.
    struct tnode **children;
    int childcount;
}tnode;

/*Create a node tnode*/
struct tnode* createTree(   int val, 
                            int nodetype, 
                            char* c,  
                            int type, 
                            struct tnode **children, 
                            int childcount);

void printTree(struct tnode* t);
char * printNode( struct tnode* t );

/*To evaluate an expression tree*/
int evaluate(struct tnode *t, int * ans);

