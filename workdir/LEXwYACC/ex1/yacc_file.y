%{
    #include <stdio.h>
    #include <stdlib.h>

%}

%union{
    char c;
};
%token LETTER
%type <c> LETTER
%left '+' '-'

%%

start : expr '\n'  {
                        printf("\nComplete\n");
                        exit(1);
                      }
  ;

expr:  expr '+' expr        {printf("+ ");}
  | expr '*' expr           {printf("* ");}
  | '(' expr ')'
  | LETTER                    {printf("%c ",$<c>1);}
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