%{
   #include<stdio.h>
   int inFile;
   char * file1;
%}

%%

[0-9]+  {printf("number: %d\n", atoi(yytext)); }
(.)     {printf("Unaccounted %s\n", yytext); }

%%

int yywrap()
{
   if( inFile == 1){
      inFile=0;
      yyin = stdin;
      return 0;
   }
   else return 1;
}

int main()
{
   inFile = 1;
   file1="inputs/input_file.l";
   yyin = fopen(file1,"r");
   yylex();
   return 1;
}