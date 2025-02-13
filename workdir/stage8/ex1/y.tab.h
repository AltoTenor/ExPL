/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    START = 258,
    END = 259,
    DECL = 260,
    ENDDECL = 261,
    BRKP = 262,
    MAIN = 263,
    RETURN = 264,
    TUPLE = 265,
    TYPE = 266,
    ENDTYPE = 267,
    ID = 268,
    STR = 269,
    INT = 270,
    NUM = 271,
    STRING = 272,
    READ = 273,
    WRITE = 274,
    INITIALIZE = 275,
    ALLOC = 276,
    NULL_KEYWORD = 277,
    FREE = 278,
    SELF = 279,
    NEW = 280,
    DELETE = 281,
    IF = 282,
    ENDIF = 283,
    ELSE = 284,
    THEN = 285,
    CLASS = 286,
    ENDCLASS = 287,
    EXTENDS = 288,
    REPEAT = 289,
    UNTIL = 290,
    WHILE = 291,
    DO = 292,
    ENDWHILE = 293,
    BREAK = 294,
    CONTINUE = 295,
    GE = 296,
    GT = 297,
    LE = 298,
    LT = 299,
    EQ = 300,
    NE = 301,
    AND = 302,
    OR = 303
  };
#endif
/* Tokens.  */
#define START 258
#define END 259
#define DECL 260
#define ENDDECL 261
#define BRKP 262
#define MAIN 263
#define RETURN 264
#define TUPLE 265
#define TYPE 266
#define ENDTYPE 267
#define ID 268
#define STR 269
#define INT 270
#define NUM 271
#define STRING 272
#define READ 273
#define WRITE 274
#define INITIALIZE 275
#define ALLOC 276
#define NULL_KEYWORD 277
#define FREE 278
#define SELF 279
#define NEW 280
#define DELETE 281
#define IF 282
#define ENDIF 283
#define ELSE 284
#define THEN 285
#define CLASS 286
#define ENDCLASS 287
#define EXTENDS 288
#define REPEAT 289
#define UNTIL 290
#define WHILE 291
#define DO 292
#define ENDWHILE 293
#define BREAK 294
#define CONTINUE 295
#define GE 296
#define GT 297
#define LE 298
#define LT 299
#define EQ 300
#define NE 301
#define AND 302
#define OR 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 30 "yacc_file.y"

    struct tnode * no;

#line 157 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
