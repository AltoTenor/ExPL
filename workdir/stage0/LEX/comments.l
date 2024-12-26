%{
    #include <stdio.h>
    //      1. [^*]
    //          [^*] is a negated character class that matches any character except *.
    //              ^ inside the square brackets negates the set, meaning it matches any character except the ones listed (in this case, *).
    //              This part ensures that we don't mistakenly match a * inside the comment, which might be part of the */ end sequence.

    //      2. |\*+[^*]

    //      The | is the alternation operator, which means "or."
    //          \*+[^*] matches sequences of one or more * characters followed by any character except *.
    //              \*+ matches one or more * characters.
    //              [^*] matches any character except *.
    //              This part is necessary to ensure that you can match * characters within the comment, which is valid in many multi-line comments (e.g., /* * comment */), but we don't want to incorrectly end the comment when encountering */
%}

SingleComment ^\/\/(.)*
MultiComment \/\*([^*]|\*+[^*])*\*\/
%%

{SingleComment} {   printf("Single Line Commment: %s", yytext);}
{MultiComment}  {   printf("Multi Line Commment: %s", yytext);continue;}
(.)             {   continue;}

%%

int yywrap()
{
    return 1;
}

int main(){
    yyin = fopen("inputs/commentFile.txt","r");
    yylex();
    return 1;
}