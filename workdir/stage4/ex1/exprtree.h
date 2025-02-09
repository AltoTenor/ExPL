
typedef enum {
    addNode = 1,
    mulNode = 3,
    subNode = 7,
    divNode = 9,
    modNode = 23,

    geNode = 11, 
    leNode = 13,
    gtNode = 15,
    ltNode = 17,
    eqNode = 19, 
    neNode = 21,

    connectorNode = 0,
    readNode = 2,
    writeNode = 4,
    assignNode = 6,

    derefNode = 16,
    idNode = 8,
    numNode = 10,
    strConstNode = 12,

    ifNode = 14,
    whileNode = 18,
    dowhileNode = 20,
    repeatNode = 22,
    breakNode = 24,
    contNode = 26,

    intTypeNode = 28,
    strTypeNode = 30,

    arrTypeNode = 32,
    arr2dTypeNode = 34,

} NodeType;

typedef enum{
    strType,
    intType,
    strArrType,
    intArrType,
    boolType,
    invalidType
} VarType;

typedef struct tnode {
    int val;        // value of a number for NUM nodes.
    int type;       // type of variable
    char* varname;  // name of a variable for ID nodes
    int nodetype;   // information about non-leaf nodes - read/write/connector/+/* etc.
    struct Gsymbol *Gentry;     // pointer to GST entry for global variables and functions
    struct tnode **children;
    int childcount;
}tnode;

/*Create a node tnode*/
struct tnode* createTree(   int val, 
                            int nodetype, 
                            char* c,  
                            int type, 
                            struct tnode **children, 
                            int childcount,
                            struct Gsymbol * entry );

struct Gsymbol {
    char * name;       // name of the variable
    int type;         // type of the variable
    int binding;      // stores the static memory address allocated to the variable
    int * size;         // size of the type of the variable
    struct Gsymbol *next;
};

// Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.
struct Gsymbol *Lookup(char * name);

// Creates a symbol table entry.
void Install(char *name, int type, int first, int second);

// 
void setTypes(struct tnode* t, int type);

void printTree(struct tnode* t);
char * printNode( struct tnode* t );

/*To evaluate an expression tree*/
int evaluate(struct tnode *t, int * ans);

void printSymbolTable();