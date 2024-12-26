%{
    #include <stdio.h>

%}

match (A[a-zA-Z][a-zA-Z]([ \n]))

%%

{match} {
    printf("Matched: %s", yytext);
}
<<EOF>> {   
        printf("End");
        return 1;
}
(.) { continue; }

%%

int yywrap(){
    return 1;
}

int main(){
    yyin = fopen("inputs/3lettter.txt","r");
    yylex();
    return 1;
}