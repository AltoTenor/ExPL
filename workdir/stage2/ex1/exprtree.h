// typedef struct tnode{
//     int val; //value of the expression tree
//     char *op; //indicates the opertor
//     struct tnode *left,*right; //left and right branches
// }tnode;



typedef enum {
    Read = 2,
    Write = 4,
    Connector = 0,
    Assign = 5,
    Add = 1,
    Mul = 3,
    Id = 10,
    Num = 12
} NodeType;

typedef struct tnode {
    int val;        // value of a number for NUM nodes.
    int type;       // type of variable
    char* varname;  // name of a variable for ID nodes
    int nodetype;   // information about non-leaf nodes - read/write/connector/+/* etc.
    struct tnode *left,*right;  // left and right branches
}tnode;

/*Create a node tnode*/
struct tnode* createTree(int val, int type, char* c, struct tnode *l, struct tnode *r);

void printTree(struct tnode* t);
char * printNode( struct tnode* t );

/*To evaluate an expression tree*/
int evaluate(struct tnode *t, int * ans);

