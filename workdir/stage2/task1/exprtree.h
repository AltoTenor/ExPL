// typedef struct tnode{
//     int val; //value of the expression tree
//     char *op; //indicates the opertor
//     struct tnode *left,*right; //left and right branches
// }tnode;



typedef enum {
    Read,
    Write,
    Connector,
    Assign,
    Add,
    Mul,
    Id,
    Num
} NodeType;

typedef struct tnode {
    int val;        // value of a number for NUM nodes.
    int type;       // type of variable
    char* varname;  // name of a variable for ID nodes
    int nodetype;   // information about non-leaf nodes - read/write/connector/+/* etc.
    struct tnode *left,*right;  // left and right branches
}tnode;

/*Create a node tnode*/
struct tnode* createTreeNode(int val, int type, char* c, struct tnode *l, struct tnode *r);

/* Print the entire tree preorder */
void printTree(struct tnode* t);
char * printNode( struct tnode* t );

