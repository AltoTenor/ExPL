%{
   #include<stdio.h>
   #include<stdlib.h>
   int consoleReadCount;  // Counter to track the number of console reads
%}

%%

[0-9]+  { printf("number: %d\n", atoi(yytext)); }
(.)     { printf("Unaccounted %s\n", yytext); }

%%

int yywrap()
{
    if (consoleReadCount < 1) {
        consoleReadCount++;
        printf("Enter input (Second time):\n");
        // Close console
        close(fileno(stdin));
        freopen("/dev/tty", "r", stdin); 
        // Reset yyin to the console
        yyin = stdin;           
        return 0;
    }
    return 1;
}

int main()
{
    consoleReadCount = 0;
    printf("Enter input (First time):\n");
    yylex();
    yylex();
    return 0;
}