%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <ctype.h>
    struct node{
        char * label;
        int addr;
        struct node * next;
    };
    struct node * head; 
    struct node * tail;
    int line;
    int secondpass = 0;
    FILE * fp1;
    int isnumber(char * str);
    char* find_addr_from_label(char * result_str, struct node * head);
    void yyerror(char const *);
    int addr_compute(int line);
    void convert(char * str);
%}

letter  [a-zA-Z]
digit   [0-9]
space   [ \t]

%%
({letter})({letter}|{digit})+:({space})*\n  {   
                                                if (!secondpass){
                                                    struct node * t = (struct node *)malloc(sizeof(struct node));
                                                    t->label = (char *)malloc(10*sizeof(char*));
                                                    strncpy(t->label, yytext, yyleng-2);
                                                    t->addr =  addr_compute(line);
                                                    if ( tail == NULL ){
                                                        head = t;
                                                        tail = t;
                                                    } 
                                                    else {
                                                        tail ->next =  t;
                                                        tail = t;
                                                    }
                                                }
                                            }
[\n]                                        {   if (!secondpass) line ++;
                                                else fprintf(fp1, "%s", yytext);    }
({letter})({letter}|{digit})+               {   if (secondpass){ 
                                                    convert(yytext);
                                                } 
                                            }
(.)                                         {   if (secondpass) fprintf(fp1, "%s", yytext); }

%%

int addr_compute(int line){
    return 2*(line - 9) + 2056;
}

void convert(char * str){
    struct node * t = head;
    while ( t ){
        if ( strcmp( t->label, str ) == 0 ){
            char addr_str[5];
            sprintf(addr_str, "%d", t->addr);
            fprintf(fp1, "%s", addr_str);
            return;
        }
        t = t->next;
    }
    fprintf(fp1, "%s", yytext);
}

int isnumber(char * str){
    int n = strlen(str);
    for (int i=0;i<n;i++){
        if ( !isdigit(str[i]) ) return 0;
    }
    return 1;
}

int main(){
    line = 1; head = NULL; tail = NULL;
    yyin = fopen("temp.xsm","r");
    fp1 = fopen("output.xsm", "w+");
    yylex();

    struct node *t = head;
    secondpass++;
    rewind(yyin);
    yylex();

    // Print Labels and their addresses
    printf("\nLabel Translator: \n");
    while (t){
        printf("%s %d\n", t->label, t->addr);
        t = t->next;
    }
    printf("Label Translator: Changed labels to addresses!\n");
    return 0;
}

int yywrap(void) {
    return 1;
}