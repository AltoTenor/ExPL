%{
   #include<stdio.h>
   int c1,c2;
%}

%%

[0-9]               {  c2++;  }
[0-9]*[.][0-9]+     {  c1++; }
(.)                 {  continue; }

%%

int yywrap()
{
    printf("#Float = %d\n", c1);
    printf("#Integers = %d\n", c2);
    return 1;
}

int main()
{
   yylex();
   return 1;
}