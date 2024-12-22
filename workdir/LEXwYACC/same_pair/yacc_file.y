%{
  #include<stdio.h>
  #include<stdlib.h>
  #include<ctype.h>
  int yyerror();
  int yylex();
%}

%token DIGIT

%%

start : pair '\n'  {printf("\nComplete");exit(0); }
 ;

pair: num ',' num { printf("pair(%d,%d)",$1,$3); }
  ;
num: DIGIT   { $$=$1; }
  ;

%%

int yyerror()
{
  printf("Error");
}

int main()
{
  yyparse();
  return 1;
}
