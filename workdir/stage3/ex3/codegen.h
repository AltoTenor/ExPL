// Generates the target code for the given syntax tree
int codeGen( struct tnode *t , int* jmpLabels);

// Allocates a free register for use in code generation
int getReg();

// Frees a previously allocated register
int freeReg();

// Initializes the SP and sets up the function calls for Read Write and Exit
void initialize();

// Generates a new unique label to be used in jump and loop constructs
int getLabel();