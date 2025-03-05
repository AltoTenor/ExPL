union Constant{
    int intval;
    char* strval;
};

// ENUMS
typedef enum {
    NODE_ADD = 1,
    NODE_MUL = 3,
    NODE_SUB = 7,
    NODE_DIV = 9,
    NODE_MOD = 23,

    NODE_GE = 11, 
    NODE_LE = 13,
    NODE_GT = 15,
    NODE_LT = 17,
    NODE_EQ = 19, 
    NODE_NE = 21,
    NODE_AND = 25,
    NODE_OR = 27,

    NODE_CONNECTOR = 0,
    NODE_READ = 2,
    NODE_WRITE = 4,
    NODE_ASSIGN = 6,

    NODE_EXPR = 16,
    NODE_DEREF = 58,
    NODE_ID = 8,
    NODE_CONST_NUM = 10,
    NODE_CONST_STR = 12,

    NODE_IF = 14,
    NODE_WHILE = 18,
    NODE_DOWHILE = 20,
    NODE_REPEAT = 22,
    NODE_BREAK = 24,
    NODE_CONTINUE = 26,

    NODE_TYPE = 28,

    NODE_BRKP = 30,
    NODE_ARR_TYPE = 32,
    NODE_PTR = 34,
    NODE_ADDR = 56,


    NODE_LDECL = 36,
    NODE_GDECL_FUNC = 38,
    NODE_GDECL_PTRFUNC = 58,
    NODE_PARAM = 40,
    NODE_GDECL = 42,

    NODE_ARG = 44,
    NODE_FDEF = 46,
    NODE_FBODY = 48,
    NODE_RETURN = 52,
    NODE_FUNC_CALL = 54,
    NODE_TUPLE = 60,
    NODE_MEMBER = 62,

    NODE_TYPEDEF = 64,
    NODE_FIELD_DECL = 66,

    NODE_INIT = 68,
    NODE_ALLOC = 70,
    NODE_NULL = 72,
    NODE_FREE = 74,

    NODE_CLASS_DEF = 76,
    NODE_MDECL = 78,
    NODE_MDEF = 80,

    NODE_CNAME = 82,
    NODE_SELF = 84,
    NODE_NEW = 86,
    NODE_DELETE = 88,

    NODE_ROOT = 50,

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
    struct Typetable* type;             // type of variable
    int nodetype;                       // information non-leaf nodes - read/write/connector/+/* etc
    struct Gsymbol *Gentry;             // pointer to GST entry for global variables and functions
    struct Lsymbol *Lentry;
    struct tnode **children;
    struct Classtable *Ctype;
    struct Paramstruct * paramlist;
    int childcount;
}tnode;

struct Typetable{
    char *name;                         // type name
    int size;                           // size of the type
    int generalType;                    /* Add a custom field for differentiating 
                                           between CLASS_TYPE / USER_DEF/ TYPE_TUPLE */
    struct Fieldlist *fields;           // pointer to the head of fields list
    struct Classtable *Ctype;
    struct Typetable *next;             // pointer to the next type table entry
};

struct Gsymbol{
    char *name;                         // name of the variable or function
    struct Typetable *type;             // type of the variable:(Integer / String)
    int size;                           // size of an array
    int binding;                        // static binding of global variables
    struct Paramstruct *paramlist;      // pointer to the head of the formal parameter list
    int flabel;                         // A label for identifying the starting addr of the function
    struct Gsymbol *next;               // points to the next Global Symbol Table entry
}; 

struct Lsymbol{
    char *name;                         // name of the variable
    struct Typetable* type;             // pointer to the Typetable entry of variable type
    int binding;                        // stores memory address allocated to the variable
    struct Lsymbol *next;               // points to the next Local Symbol Table entry
};

struct Fieldlist{
  char *name;                           // name of the field
  int fieldIndex;                       // the position of the field in the field list
  struct Typetable *type;               // pointer to type table entry of the field's type
  struct Fieldlist *next;               // pointer to the next field
};

struct Paramstruct{
    char *name;                         // stores the name of the parameter
    struct Typetable* type;             // pointer to type table entry of parameter type
    int checked;                        // pointer to type table entry of parameter type
    struct Paramstruct *next;           // pointer to the next parameter
};

struct MemberFunclist {
    char * methodName;                  // name of the member function in the class
    struct Typetable * type;            // pointer to typetable
    struct Paramstruct * paramlist;     // pointer to the head of the formal parameter list
    int funcposition;                   // position of the function in the class table
    int flabel;                         // A label for identifying the starting addr of the function
    struct MemberFunclist *next;        // pointer to next Memberfunclist entry
};

struct Classtable {
    char * className;                   // name of the class
    struct Fieldlist *memberField;      // pointer to Fieldlist
    struct MemberFunclist *vFuncptr;    // pointer to Memberfunclist
    struct Classtable *parentPtr;       // pointer to the parent's class table
    int classIndex;                     // position of the class in the virtual function table
    int fieldCount;                     // count of fields
    int methodCount;                    // count of methods
    struct Classtable *next;            // pointer to next class table entry
};

struct Context{
    int * jumpLabels;
    int mainFunc;
    int localvars;
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
// Searching for a specific field
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
// Fetch the argument list from the tnode
struct Paramstruct* fetchArgList(struct tnode * t);
// Verify the argument matches or not
int verifyArgTypes(struct Paramstruct* argList, struct Paramstruct* paramList );

/* ---------------------------------------------------------------------------------------------- */
/* PARAM LIST FUNCTIONS */

// Given a GST Function entry, insert a parameter to the function 
void insertIntoParamList(struct Gsymbol* paramHead, struct Paramstruct* cur);
// Given a paramlist find if a param exists or not
struct Paramstruct* searchParamList(struct Paramstruct* head, char* name);
// Used to check if the argument list matches with the functions previously declared params
void compareLaxParamList(struct tnode * t, struct Paramstruct *paramlist);
// Adds the parameters as variables to the local symbol table
struct Lsymbol* addParamListToLsymbolTable(struct Paramstruct * pl, struct Lsymbol* table);
// Verify if two paramStructs match or not
int verifyParamList(struct Paramstruct* a, struct Paramstruct* b);
// Fetch the paramlist from a subtree of paramnodes
struct Paramstruct * fetchParamList(struct tnode * t);

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
struct MemberFunclist * MethodLookup(char * name, struct Classtable* CT, struct Paramstruct * p);
// Look up a member for a class based on name
struct Fieldlist * MemberLookup(char * name, struct Classtable* CT);
// Checking if second class is descendant of first
int checkDescendant(struct Classtable * parent, struct Classtable * child );
// Helper Function to print the class table
void printClassTable();
// Adding the parent field list to the child if inherited
struct Fieldlist * addParentFieldList(struct Classtable * CTEntry);
// Adding the parent methods to the child if inherited
struct MemberFunclist * addParentMethods(struct Classtable * CTEntry);

/* ---------------------------------------------------------------------------------------------- */
/* Printing Tree Nodes */
void printTree(struct tnode* t, struct tnode* p, int depth);
char * printNode( struct tnode* t );
void printX(char * s, int X, int val, int type);
char * printType( struct Typetable* t );
char * printClass( struct Classtable* t );

/* ---------------------------------------------------------------------------------------------- */
// Codegen Function
void compile(struct tnode * t);
