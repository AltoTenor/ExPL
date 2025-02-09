%{
   #include<stdio.h>
   char *file1;
   int c1,c2,c3;
%}

%%

([a-zA-Z]|[0-9])+    {  c2++; c3 += yyleng;}
[\n]                 {  c1++; }
(.)                  {  continue; }

%%

int yywrap()
{
   printf("Newline Count: %d\n", c1+1);
   printf("Words Count: %d\n", c2);
   printf("Character Count: %d\n", c3);
   return 1;
}

int main()
{
   file1="inputs/text_file.txt";
   yyin = fopen(file1,"r");
   c1 = 0;
   c2 = 0;
   c3 = 0;
   yylex();
   return 1;
}