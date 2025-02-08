union Constant{
    int intval;
    char* strval;
};

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
    andNode = 25,
    orNode = 27,

    connectorNode = 0,
    readNode = 2,
    writeNode = 4,
    assignNode = 6,

    exprNode = 16,
    derefOpNode = 58,
    idNode = 8,
    numNode = 10,
    strConstNode = 12,

    ifNode = 14,
    whileNode = 18,
    dowhileNode = 20,
    repeatNode = 22,
    breakNode = 24,
    contNode = 26,

    typeNode = 28,

    brkpNode = 30,
    arrTypeNode = 32,
    ptrNode = 34,
    addrNode = 56,


    LDeclNode = 36,
    funcTypeGDeclNode = 38,
    funcPtrTypeGDeclNode = 58,
    paramNode = 40,
    GDeclNode = 42,

    argNode = 44,
    FDefNode = 46,
    FBodyNode = 48,
    returnNode = 52,
    funcCallNode = 54,
    tupleNode = 60,
    memberNode = 62,

    typeDefNode = 64,
    fieldDeclNode = 66,

    initNode = 68,
    allocNode = 70,
    nullNode = 72,
    freeNode = 74,

    classDefNode = 76,
    MDeclNode = 78,
    MDefNode = 80,

    CNameNode = 82,
    selfNode = 84,
    newNode = 86,
    deleteNode = 88,

    rootNode = 50,

} NodeType;

typedef enum {
    PRIMITIVE = 0,
    TYPE_TUPLE = 1, 
    POINTER = 2,
    USER_DEF = 3,
    CLASS_TYPE = 4,
} GeneralType;

/* ---------------------------------------------------------------------------------------------- */
/* STRUCTS */
typedef struct tnode {
    char* name;
    union Constant value;
    struct Typetable* type;     // type of variable
    int nodetype;               // information about non-leaf nodes - read/write/connector/+/* etc.
    struct Gsymbol *Gentry;     // pointer to GST entry for global variables and functions
    struct Lsymbol *Lentry;
    struct tnode **children;
    struct Classtable *Ctype;
    int childcount;
}tnode;

struct Typetable{
    char *name;                 //type name
    int size;                   //size of the type
    int generalType;              //
    struct Fieldlist *fields;   //pointer to the head of fields list
    struct Typetable *next;     // pointer to the next type table entry
    struct Classtable *Ctype;
};

struct Gsymbol{
    char *name;                         //name of the variable or function
    struct Typetable *type;             //type of the variable:(Integer / String)
    int size;                           //size of an array
    int binding;                        //static binding of global variables
    struct Paramstruct *paramlist;      //pointer to the head of the formal parameter list
    int flabel;                         //a label for identifying the starting address of a function's code
    struct Gsymbol *next;               //points to the next Global Symbol Table entry
}; 

struct Lsymbol{
    char *name;                 //name of the variable
    struct Typetable* type;     //pointer to the Typetable entry of variable type
    int binding;                //stores memory address allocated to the variable
    struct Lsymbol *next;       //points to the next Local Symbol Table entry
};

struct Fieldlist{
  char *name;              //name of the field
  int fieldIndex;          //the position of the field in the field list
  struct Typetable *type;  //pointer to type table entry of the field's type
  struct Fieldlist *next;  //pointer to the next field
};

struct Paramstruct{
    char *name;                     //stores the name of the parameter
    struct Typetable* type;                       //pointer to type table entry of parameter type
    int checked;                    //pointer to type table entry of parameter type
    struct Paramstruct *next;       //pointer to the next parameter
};

struct MemberFunclist {
    char * methodName;                         //name of the member function in the class
    struct Typetable * type;             //pointer to typetable
    struct Paramstruct * paramlist;      //pointer to the head of the formal parameter list
    int funcposition;                   //position of the function in the class table
    int flabel;                         //A label for identifying the starting address of the function's code in the memory
    struct MemberFunclist *next;        //pointer to next Memberfunclist entry
};

struct Classtable {
    char * className;                         //name of the class
    struct Fieldlist *memberField;      //pointer to Fieldlist
    struct MemberFunclist *vFuncptr;    //pointer to Memberfunclist
    struct Classtable *parentPtr;       //pointer to the parent's class table
    int classIndex;                    //position of the class in the virtual function table
    int fieldCount;                     //count of fields
    int methodCount;                    //count of methods
    struct Classtable *next;            //pointer to next class table entry
};


/* ---------------------------------------------------------------------------------------------- */
/*Create a node tnode*/
struct tnode* createTree(   char* name,
                            union Constant* c,  
                            char* type, 
                            int nodetype, 
                            struct tnode **children, 
                            int childcount,
                            struct Gsymbol * Gentry,
                            struct Lsymbol * Lentry  );

/* ---------------------------------------------------------------------------------------------- */
/* GLOBAL SYMBOL TABLE FUNCTIONS */

// Creates a Global symbol table entry.
void GInstall(char *name, struct Typetable * typename, int size, int fl);
// Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.
struct Gsymbol *GLookup(char * name);
// Sets up the types in a global declaration
void setGTypes(struct tnode* t, struct Typetable * type, struct Gsymbol* paramHead);
// Prints the Global Symbol Table
void printGSymbolTable();

/* ---------------------------------------------------------------------------------------------- */
/* LOCAL SYMBOL TABLE FUNCTIONS */

// Creates a local symbol table entry.
struct Lsymbol * LInstall(char *name, struct Typetable* type, struct tnode* head);
// Searches a specific entry in the local.
struct Lsymbol *LLookup(char * name, struct Lsymbol * head);
// Sets up the types in a local declaration
void setLTypes(struct Typetable* type, struct tnode* t, struct tnode* head);
// Prints the Local Symbol Table
void printLSymbolTable(struct Lsymbol * l );
// Join the two lists of Lsymbols
struct Lsymbol * joinLsymbols(struct Lsymbol * Lentry1, struct Lsymbol * Lentry2);
// Adding binding addresses for all symbols in LST
void addBindingAddr(struct Lsymbol* Lentry);
/* ---------------------------------------------------------------------------------------------- */
/* TYPE TABLE */

// Insert a new type record
struct Typetable* TInstall( char *name, int size, 
                            struct Fieldlist *fields, 
                            GeneralType gt, 
                            struct Classtable * Ctype);
// Search for a type record
struct Typetable* TLookup(char *name);
// Initialize INT / STR / BOOLEAN / NULL / VOID 
void TypeTableCreate();
// Making a pointer of a type
struct Typetable* make_pointer(struct Typetable* type);
// setting up a new user defined type
struct Typetable* setUserDefType(struct tnode* t);
// Printing the entire type table
void printTypeTable();

// Making a field list from the parameters
struct Fieldlist* fetchFieldList(struct tnode * t);
struct Fieldlist * FLookup(struct Typetable* type, char * name);

/* ---------------------------------------------------------------------------------------------- */
/* TUPLES */
// Set up a tuple in the GST
void initTuple(struct tnode **children, struct tnode * LSThead);
// Setting up IDs
void setTupleIDinGST(struct tnode * t, struct Typetable* type);
void setTupleIDinLST(struct tnode * t, struct Typetable* type, struct tnode * LSThead);

/* ---------------------------------------------------------------------------------------------- */
/* FUNCTION VERIFICATION */
void checkFDef(struct tnode ** c, struct Gsymbol *  funcEntry);
// Checking all variables and statements are of valid types or not
void assignVarTypes(struct tnode* t, 
                    struct Lsymbol * Lentry, 
                    struct Typetable* retType,
                    struct Classtable * classEntry );
// Find type of the node in a tree
struct Typetable* findType(struct tnode * t);

/* ---------------------------------------------------------------------------------------------- */
/* ARG LIST */
struct Paramstruct* fetchArgList(struct tnode * t);
int verifyArgTypes(struct Paramstruct* argList, struct Paramstruct* paramList );

/* ---------------------------------------------------------------------------------------------- */
/* PARAM LIST FUNCTIONS */

// Given a GST Function entry, insert a parameter to the function 
void insertIntoParamList(struct Gsymbol* paramHead, struct Paramstruct* cur);
// Given a paramlist find if a param exists or not
struct Paramstruct* searchParamList(struct Paramstruct* head, char* name);
// Used to check if the argument list matches with the functions previously declared params
void compareParamList(struct tnode * t, struct Paramstruct *paramlist);
// Adds the parameters as variables to the local symbol table
struct Lsymbol* addParamListToLsymbolTable(struct Paramstruct * pl, struct Lsymbol* table);

/* ---------------------------------------------------------------------------------------------- */
/* OExpl Functions - Related to Classes */

// Install a new entry into the class table
struct Classtable* CInstall(char *name, struct Classtable * parent);
// Looking up a new Class Table Entry 
struct Classtable* CLookup(char *name);
// To Fetch the list of function Methods from the AST
struct MemberFunclist * constructMethodList(struct tnode * t);
// To Fetch the list of parameters for a method of a class in AST
struct Paramstruct* constructMethodParamList(struct tnode* t);
// To make a new method to add to the list
struct MemberFunclist * MInstall(char * memberName, 
                                 struct Typetable * type, 
                                 struct Paramstruct * paramlist);
// Check the defintion matches the declaration 
// and checks return type and arglist and also checks the Fbody
void checkMDef(struct tnode* t, struct Classtable* CTEntry);
// Look up a method for a class
struct MemberFunclist * MethodLookup(char * name, struct Classtable* CT);
// Look up a member for a class based on name
struct Fieldlist * MemberLookup(char * name, struct Classtable* CT);
// Checking if second class is descendant of first
int checkDescendant(struct Classtable * parent, struct Classtable * child );
// Helper Function to print the class table
void printClassTable();
/* ---------------------------------------------------------------------------------------------- */
/* Printing Tree Nodes */
void printTree(struct tnode* t, struct tnode* p, int depth);
char * printNode( struct tnode* t );
void printX(char * s, int X, int val, int type);
char * printType( struct Typetable* t );
char * printClass( struct Classtable* t );

// Codegen Function
void compile(struct tnode * t);