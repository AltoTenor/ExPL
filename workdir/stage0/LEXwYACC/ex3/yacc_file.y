%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    void yyerror(char const *s);
    int yylex();
%}

%union{
    char * str;
};
%token <str> LETTER
%left '+' 
%left '*'

%%

start 	: expr '\n'					{ 	
										printf("Prefix: %s\n", $<str>$); 
										exit(1);
									}
  ;

expr	: expr '+' expr           	{ 
										$<str>$ = (char*)malloc(sizeof(char)*100);
										strcpy($<str>$, "+ ");
										strcat($<str>$, $<str>1);
										strcat($<str>$, $<str>3);
									}
  		| expr '*' expr           	{ 
										$<str>$ = (char*)malloc(sizeof(char)*100);
										strcpy($<str>$, "* ");
										strcat($<str>$, $<str>1);
										strcat($<str>$, $<str>3);
									}
  		| '(' expr ')'				{	
										$<str>$ = (char*)malloc(sizeof(char)*100);
										strcpy($<str>$, $<str>2);
									}
		| LETTER					{	
										$<str>$ = (char*)malloc(sizeof(char)*100);
										strcpy($<str>$, $<str>1);
										strcat($<str>$, " ");
									}
;

%%

void yyerror(char const *s)
{
    printf("yyerror %s\n",s);
    return ;
}
int main()
{
    yyparse();
    return 1;
}