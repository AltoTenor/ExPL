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
    if (consoleReadCount < 1) {  // Allow reading from console twice
        consoleReadCount++;
        printf("Enter input (Second time):\n");
        close(fileno(stdin));         // Close stdin
        freopen("/dev/tty", "r", stdin); 
        yyin = stdin;           // Reset yyin to the console
        return 0;               // Continue scanning
    }
    return 1;                   // Stop scanning after two console reads
}

int main()
{
    consoleReadCount = 0;  // Initialize the console read counter

    printf("Enter input (First time):\n");
    yylex();               // First read from console

    yylex();               // Second read from console

    return 0;
}