%{
    #include <stdlib.h>
    #include <stdio.h>
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
    char* find_addr_from_label(char * result_str, struct node * head);
    void yyerror(char const *);
    int addr_compute(int line);
%}

letter  [a-zA-Z]
digit   [0-9]

%%
({letter})({letter}|{digit})+:          {   struct node * t = (struct node *)malloc(sizeof(struct node));
                                            t->label = (char *)malloc(10*sizeof(char*));
                                            strncpy(t->label, yytext, yyleng-1);
                                            t->addr =  addr_compute(line);
                                            if ( tail == NULL ){
                                                head = t;
                                                tail = t;
                                            } 
                                            else {
                                                tail ->next =  t;
                                                tail = t;
                                            }
                                            line--;
                                        }
[\n]                                    {   line ++;    }
("CALL"|"JMP"|"JNZ"|"JZ")(.)+\n              {   line ++;
                                            if (secondpass){
                                                char result_str[50] = "";
                                                fprintf(fp1, "%s\n", find_addr_from_label(result_str, head));
                                            }
                                        }
(.)+                                    {   if (secondpass) fprintf(fp1, "%s\n", yytext); }

%%

int addr_compute(int line){
    return 2*(line - 9) + 2056;
}


char* find_addr_from_label(char * result_str, struct node * head){
    int l = strlen(yytext);
    int key = -1;
    for (int i=0;i<l;i++){
        if ( yytext[i] == ' ' || yytext[i] == ',' ) key = i+1;
        if ( yytext[i] == '\n' ) yytext[i] = '\0';
    }
    char * s = (yytext+key);
    // printf("Label: %s of length: %d\n", yytext+key, strlen(yytext+key));
    int ans = -2;
    struct node * t = head;
    while ( t ){
        if ( strcmp("0", s) == 0 ) {ans = 0;break;}
        if ( strcmp( t->label, s ) == 0 ){ ans = t-> addr; break;}
        // printf("%d\n", t->addr);
        t = t->next;
    }
    if ( ans == -2 ) {  
        printf("Incorrect Label Generated\n");
        exit(1);
    }
    char addr_str[50] = "";
    strcpy(result_str, yytext);

    result_str[key]='\0';
    snprintf(addr_str, sizeof(addr_str), "%d", ans);
    strcat(result_str, addr_str);
    // printf("%s\n", result_str);

    // printf("Adding to %s\n", result_str);
    return result_str;
}

int main(){
    line = 1;
    head = NULL;
    tail = NULL;
    yyin = fopen("temp.xsm","r");
    yylex();

    struct node *t = head;
    secondpass++;
    rewind(yyin);
    fp1 = fopen("output.xsm", "w+");
    yylex();
    
    printf("Changed labels to addresses!\n");
    return 0;
}

int yywrap(void) {
    return 1;
}