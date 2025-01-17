typedef struct tnode {
    int val;                    // value of a number for NUM nodes.
    int type;                   // type of variable
    char* varname;              // name of a variable for ID nodes
    int nodetype;               // information about non-leaf nodes - read/write/connector/+/* etc.
    struct Gsymbol *Gentry;     // pointer to GST entry for global variables and functions
    struct Lsymbol *Lentry;
    struct tnode **children;
    int childcount;
}tnode;

struct Gsymbol{
    char *name;                         //name of the variable or function
    int type;                           //type of the variable:(Integer / String)
    int size;                           //size of an array
    int binding;                        //static binding of global variables
    struct Paramstruct *paramlist;      //pointer to the head of the formal parameter list
    int flabel;                         //a label for identifying the starting address of a function's code
    struct Gsymbol *next;               //points to the next Global Symbol Table entry
}; 

struct Lsymbol{
    char *name;                 //name of the variable
    int type;                   //pointer to the Typetable entry of variable type
    int binding;                //stores memory address allocated to the variable
    struct Lsymbol *next;       //points to the next Local Symbol Table entry
};

struct Paramstruct{
    char *name;                     //stores the name of the parameter
    int type;                       //pointer to type table entry of parameter type
    int checked;                    //pointer to type table entry of parameter type
    struct Paramstruct *next;       //pointer to the next parameter
};

/*Create a node tnode*/
struct tnode* createTree(   int val, 
                            int nodetype, 
                            char* c,  
                            int type, 
                            struct tnode **children, 
                            int childcount,
                            struct Gsymbol * Gentry,
                            struct Lsymbol * Lentry  );

/* GLOBAL SYMBOL TABLE */
// Creates a Global symbol table entry.
void GInstall(char *name, int type, int size, int fl);
// Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.
struct Gsymbol *GLookup(char * name);
// Sets up the types in a global declaration
void setGTypes(struct tnode* t, int type, struct Gsymbol* paramHead);
// Prints the Global Symbol Table
void printGSymbolTable();
// helpers
void check(int type1, int type2, char * mssg);

/* LOCAL SYMBOL TABLE */
// Creates a local symbol table entry.
struct Lsymbol * LInstall(char *name, int type, struct tnode* head);
// Searches a specific entry in the local.
struct Lsymbol *LLookup(char * name, struct Lsymbol * head);
// Sets up the types in a local declaration
void setLTypes(int type, struct tnode* t, struct tnode* head);
// Prints the Local Symbol Table
void printLSymbolTable(struct Lsymbol * l );
// Join the two lists of Lsymbols
struct Lsymbol * joinLsymbols(struct Lsymbol * Lentry1, struct Lsymbol * Lentry2);
// Adding binding addresses for all symbols in LST
void addBindingAddr(struct Lsymbol* Lentry);

/* FUNCTION VERIFICATION */
void checkFDef(struct tnode ** c, struct Gsymbol *  funcEntry);
// Checking all variables and statements are of valid types or not
void assignVarTypes(struct tnode* t, struct Lsymbol * Lentry, int retType);
// Find type of the node in a tree
int findType(struct tnode * t);

// ARG LIST
struct Paramstruct* fetchArgList(struct tnode * t);
int verifyArgTypes(struct Paramstruct* argList, struct Paramstruct* paramList );

/* PARAM LIST FUNCTIONS */
void insertIntoParamList(struct Gsymbol* paramHead, struct Paramstruct* cur);
struct Paramstruct* searchParamList(struct Paramstruct* head, char* name);
void compareParamList(struct tnode * t, struct Paramstruct *paramlist);
struct Lsymbol* addParamListToLsymbolTable(struct Paramstruct * pl, struct Lsymbol* table);

/* Printing Tree Nodes */
void printTree(struct tnode* t, struct tnode* p, int depth);
char * printNode( struct tnode* t );
void printX(char * s, int X, int val, int type);
char * printType( int t );

// ENUMS
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

    brkpNode = 36,

    funcTypeGDeclNode = 38,
    paramNode = 40,
    GDeclNode = 42,
    LDeclNode = 44,

    FDefNode = 46,
    FBodyNode = 48,
    returnNode = 52,
    funcCallNode = 54,
    argNode = 56,

    rootNode = 50,

} NodeType;

typedef enum{
    strType = 100,
    intType = 101,
    strArrType = 102,
    intArrType = 103,
    boolType = 104,
    invalidType = 105
} VarType;