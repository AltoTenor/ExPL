%{
   #include<stdio.h>
   int console;
%}

%%

[0-9]+  {printf("number: %d\n", atoi(yytext)); }
(.)     {printf("Unaccounted %s\n", yytext); }

%%

int yywrap()
{
   FILE *newfile_pointer;
   char *file2="inputs/input_file_2.l";
   newfile_pointer = fopen("inputs/input_file_2.l","r");
   if( console == 1){
      console=0;
      yyin = newfile_pointer;
      return 0;
   }
   else return 1;
}

int main()
{
   int x1,x2;
   console = 1;
   x1 = yylex();
   return 1;
}