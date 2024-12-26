%{
    #include <stdio.h>
    #include <stdlib.h>
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

start : expr '\n'  {
                        printf("\nComplete\n");
                        exit(1);
                      }
  ;

expr:  expr '+' expr        {printf("+ ");}
  | expr '*' expr           {printf("* ");}
  | '(' expr ')'
  | LETTER                    {printf("%s ",$<str>1);}
  ;

%%

void yyerror(char const *s)
{
    printf("yyerror  %s\n",s);
    return ;
}
int main()
{
    yyparse();
    return 1;
}