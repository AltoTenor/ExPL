/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "yacc_file.y"

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stddef.h>
    #include "analyser.h"
    int yylex(void);
    int yyerror(char const *s);
    struct tnode** alloc_1(struct tnode * t1);
    struct tnode** alloc_2(struct tnode * t1, struct tnode * t2);
    struct tnode** alloc_3(struct tnode * t1, struct tnode * t2,  struct tnode * t3);
    struct tnode** alloc_4( struct tnode * t1, 
                            struct tnode * t2,  
                            struct tnode * t3,  
                            struct tnode * t4);
    struct tnode** alloc_5( struct tnode * t1, 
                            struct tnode * t2,  
                            struct tnode * t3,  
                            struct tnode * t4,
                            struct tnode * t5);
    extern FILE *yyin;
    extern FILE *fp;
    extern int SP;
    extern int classIndex;
    extern int globalFlabel;
    extern struct Gsymbol * symbolTable;
    extern struct Classtable * classTable;

#line 99 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 251 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   698

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  127
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  316

#define YYUNDEFTOK  2
#define YYMAXUTOK   303


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    53,    64,     2,
      57,    58,    51,    49,    59,    50,    63,    52,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    56,
       2,    62,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    60,     2,    61,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    54,     2,    55,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    59,    59,    66,    76,    77,    79,    82,    84,    89,
      93,    96,    98,   104,   108,   110,   113,   115,   118,   121,
     127,   130,   135,   138,   140,   143,   147,   150,   152,   155,
     161,   162,   164,   167,   169,   173,   178,   181,   183,   184,
     187,   190,   193,   196,   199,   205,   208,   210,   215,   220,
     226,   234,   241,   246,   250,   251,   253,   257,   259,   263,
     268,   271,   275,   278,   282,   285,   287,   290,   295,   296,
     297,   302,   306,   307,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   323,   328,   331,   338,   339,
     342,   345,   350,   355,   360,   363,   368,   373,   378,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   405,
     408,   409,   410,   411,   412,   413,   416,   419
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "START", "END", "DECL", "ENDDECL",
  "BRKP", "MAIN", "RETURN", "TUPLE", "TYPE", "ENDTYPE", "ID", "STR", "INT",
  "NUM", "STRING", "READ", "WRITE", "INITIALIZE", "ALLOC", "NULL_KEYWORD",
  "FREE", "SELF", "NEW", "DELETE", "IF", "ENDIF", "ELSE", "THEN", "CLASS",
  "ENDCLASS", "EXTENDS", "REPEAT", "UNTIL", "WHILE", "DO", "ENDWHILE",
  "BREAK", "CONTINUE", "GE", "GT", "LE", "LT", "EQ", "NE", "AND", "OR",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'{'", "'}'", "';'", "'('", "')'",
  "','", "'['", "']'", "'='", "'.'", "'&'", "$accept", "Program",
  "TypeDefBlock", "TypeDefList", "TypeDef", "NewTypeID", "FieldDeclList",
  "FieldDecl", "ClassDefBlock", "ClassDefList", "ClassDef", "Cname",
  "MethodDecl", "MDecl", "MethodDefns", "Mdef", "GDeclBlock", "GDeclList",
  "GDecl", "GidList", "Gid", "FDefBlock", "Fdef", "MainBlock", "FuncBody",
  "LdeclBlock", "LDecList", "LDecl", "IdList", "ParamList", "Param",
  "Type", "Body", "slist", "stmt", "ReturnStmt", "AsgStmt", "identifier",
  "InputStmt", "OutputStmt", "IfStmt", "WhileStmt", "DoWhileStmt",
  "RepeatStmt", "expr", "ArgList", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,    43,
      45,    42,    47,    37,   123,   125,    59,    40,    41,    44,
      91,    93,    61,    46,    38
};
# endif

#define YYPACT_NINF (-157)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      49,    -2,    20,    56,  -157,   149,  -157,    38,  -157,    98,
     119,  -157,  -157,   224,   111,    -5,  -157,    96,   215,   224,
    -157,  -157,  -157,    36,  -157,   160,   162,  -157,  -157,   180,
     186,   102,  -157,    -4,   224,  -157,  -157,    17,  -157,  -157,
     146,  -157,   224,   190,  -157,  -157,    83,   237,   118,  -157,
    -157,  -157,   208,   212,   260,  -157,   143,   157,  -157,   264,
     224,     1,   275,   235,  -157,    -4,   236,     9,   239,   238,
     174,   224,  -157,   286,   156,   158,  -157,     4,  -157,   168,
     242,    26,  -157,   251,   252,   185,    39,  -157,   224,    58,
    -157,   294,   253,    48,    29,   224,  -157,   296,  -157,  -157,
    -157,   194,    80,    80,   257,   262,   201,    87,  -157,  -157,
     258,   265,   213,  -157,   304,   123,  -157,  -157,  -157,   397,
     108,   263,   317,  -157,   268,    80,    80,   270,  -157,    51,
    -157,   269,  -157,  -157,    69,   271,   198,   266,   276,   280,
    -157,   282,   397,   283,   397,   272,   285,    86,   227,  -157,
    -157,  -157,   170,  -157,  -157,  -157,  -157,  -157,  -157,  -157,
     330,   154,  -157,    29,  -157,  -157,  -157,   289,   290,    80,
     297,   217,  -157,  -157,   336,  -157,  -157,  -157,   293,   298,
    -157,   300,   301,   302,   198,   198,    86,    82,   571,   198,
      86,   198,   198,   295,   198,   431,  -157,  -157,   219,  -157,
    -157,   198,   339,   303,  -157,  -157,   135,  -157,  -157,   299,
      80,   307,  -157,   306,   313,   349,   224,   360,  -157,   463,
     311,   184,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,  -157,   442,    88,   481,   499,
     318,   517,   320,   198,   587,  -157,   224,  -157,  -157,   323,
      80,  -157,  -157,   321,   325,   326,  -157,  -157,   619,   225,
     632,   632,   632,   632,   632,   632,   645,   645,   205,   205,
    -157,  -157,  -157,  -157,   332,   333,   355,   198,   356,   198,
     603,  -157,   228,  -157,   331,  -157,  -157,  -157,  -157,   198,
    -157,  -157,   397,   535,   397,   553,  -157,    29,  -157,   619,
     261,   338,   329,     0,   136,   340,   397,  -157,   342,  -157,
    -157,  -157,   363,  -157,   345,  -157
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,    14,     9,     0,     7,     0,     1,     0,
      31,     4,     6,     0,    20,     0,    16,     0,     0,     0,
      70,    69,    68,     0,    11,     0,     0,    13,    15,     0,
       0,     0,    33,     0,     0,    46,     3,     0,     8,    10,
       0,    21,     0,     0,    30,    32,    38,     0,     0,    37,
      45,     2,     0,     0,     0,    12,     0,     0,    23,     0,
       0,     0,     0,    39,    34,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,    65,     0,    42,     0,
       0,     0,    36,     0,     0,     0,     0,    19,     0,     0,
      27,     0,     0,     0,     0,     0,    66,     0,    41,    40,
      44,     0,     0,     0,     0,     0,     0,     0,    18,    26,
       0,     0,     0,    63,     0,     0,    64,    67,    43,     0,
       0,     0,     0,    53,     0,     0,     0,     0,    17,     0,
      25,     0,    62,    35,     0,     0,     0,    88,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,    73,
      81,    74,     0,    75,    76,    77,    78,    79,    80,    55,
       0,     0,    57,     0,    51,    52,    48,     0,     0,     0,
       0,     0,    24,    60,     0,    84,   114,   115,     0,     0,
     122,     0,     0,     0,     0,     0,     0,   116,     0,     0,
       0,     0,     0,     0,     0,     0,    82,    83,     0,    71,
      72,     0,     0,     0,    54,    56,     0,    47,    50,     0,
       0,     0,    61,     0,     0,     0,     0,     0,   113,     0,
     117,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,     0,    58,    49,     0,
       0,   120,   121,     0,     0,     0,    99,   118,   127,     0,
     105,   108,   106,   107,   110,   109,   111,   112,   100,   102,
     101,   103,   104,    89,     0,     0,     0,     0,     0,     0,
       0,    86,     0,    29,     0,   123,   124,   125,   119,     0,
      92,    93,     0,     0,     0,     0,    87,     0,    28,   126,
       0,     0,     0,     0,     0,     0,     0,    98,     0,    97,
      59,    95,     0,    96,     0,    94
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -157,  -157,  -157,  -157,   387,  -157,   353,    13,  -157,  -157,
     390,  -157,   351,    33,   324,    -3,  -157,  -157,   377,  -157,
     344,  -157,   379,   383,   -91,  -157,  -157,   250,  -156,   -48,
     327,   -13,   305,  -140,  -147,  -157,  -157,  -116,  -157,  -157,
    -157,  -157,  -157,  -157,   -96,  -157
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     5,     6,     7,    23,    24,    10,    15,
      16,    17,    57,    58,    89,    90,    19,    31,    32,    48,
      49,    34,    35,    36,   121,   122,   161,   162,   115,    75,
      76,    77,   123,   148,   149,   150,   151,   187,   153,   154,
     155,   156,   157,   158,   188,   259
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,   200,   193,   152,   195,    33,    37,   206,    14,    46,
      25,     4,   124,    79,    20,    21,    22,    96,    33,    85,
       8,    37,    20,    21,    22,    52,   152,    27,   152,    59,
      53,   198,   152,   101,   167,   168,    39,   294,   106,    20,
      21,    22,   113,    59,    73,   112,   200,    47,   200,    20,
      21,    22,    20,    21,    22,    97,   309,    73,    91,    78,
       1,    20,    21,    22,    20,    21,    22,    84,    54,    39,
     220,    20,    21,    22,   237,    91,    91,   152,   209,   152,
     114,   171,   173,   119,   100,   120,   109,     9,   218,   219,
      72,    38,    13,   236,    91,   238,   239,   105,   241,   137,
      20,    21,    22,    72,   109,   244,   111,   163,    44,   170,
     140,    14,    30,   108,   159,    20,    21,    22,   160,   249,
     174,    20,    21,    22,    18,   258,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   221,
      61,   304,   128,    62,    26,   202,   274,   280,   163,    69,
      29,   202,   300,   200,   302,   200,    20,    21,    22,   284,
     204,    11,     4,    71,   160,   200,   312,    20,    21,    22,
      20,    21,    22,    40,    64,    41,   152,    65,   152,   133,
      88,   293,   134,   295,   152,    42,   152,    20,    21,    22,
     152,   247,   310,   299,   134,   134,   152,   137,   282,    43,
     176,   177,    55,   254,   178,   179,   180,   181,   140,   182,
     183,   137,    55,    93,   176,   177,    94,    95,   178,   179,
     180,   181,   140,   182,   183,    30,    98,    95,    20,    21,
      22,   199,   201,   202,   135,   184,   136,    20,    21,    22,
     137,   185,   257,   104,    95,   138,   139,    60,   186,   184,
      63,   140,   118,    95,   141,   185,   232,   233,   234,   127,
      95,   142,   186,   143,   144,    66,   145,   146,   135,    67,
     136,   131,    95,    68,   137,   211,    95,    74,   147,   138,
     139,   243,   202,   288,   289,   140,   297,    95,   141,   305,
     306,    80,    81,    87,    83,   142,    86,   143,   144,    92,
     145,   146,   135,    99,   136,   102,   103,   110,   137,   117,
      93,   125,   147,   138,   139,   129,   126,   132,   164,   140,
     119,   130,   141,   166,   169,   172,   189,   175,   196,   142,
     240,   143,   144,   190,   145,   146,   135,   191,   136,   192,
     194,   197,   137,   203,   207,   208,   147,   138,   139,   212,
     213,   210,   245,   140,   248,   214,   141,   215,   216,   217,
     246,   250,   253,   142,   251,   143,   144,   308,   145,   146,
     135,   252,   136,   255,   202,   277,   137,   279,   283,   285,
     147,   138,   139,   286,   287,   292,   298,   140,   290,   291,
     141,   314,    12,   294,   307,    56,   311,   142,   313,   143,
     144,   315,   145,   146,   135,    28,   136,    70,    45,    82,
     137,   205,   107,    50,   147,   138,   139,    51,     0,     0,
       0,   140,   116,     0,   141,     0,     0,   165,     0,     0,
       0,   142,     0,   143,   144,     0,   145,   146,   135,     0,
     136,     0,     0,     0,   137,     0,     0,     0,   147,   138,
     139,     0,     0,     0,     0,   140,     0,     0,   141,     0,
       0,     0,     0,     0,     0,   142,     0,   242,   144,     0,
     145,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,     0,     0,     0,     0,
       0,     0,     0,   273,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,     0,     0,     0,
       0,   256,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,     0,     0,     0,     0,   275,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,     0,     0,     0,     0,   276,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,     0,     0,     0,     0,   278,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,     0,
       0,     0,     0,   301,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,     0,     0,     0,
       0,   303,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,     0,     0,   235,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,     0,     0,   281,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,     0,     0,   296,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       0,   230,   231,   232,   233,   234,   222,   223,   224,   225,
     226,   227,     0,     0,   230,   231,   232,   233,   234
};

static const yytype_int16 yycheck[] =
{
      13,   148,   142,   119,   144,    18,    19,   163,    13,    13,
      23,    13,   103,    61,    13,    14,    15,    13,    31,    67,
       0,    34,    13,    14,    15,     8,   142,    32,   144,    42,
      13,   147,   148,    81,   125,   126,    23,    37,    86,    13,
      14,    15,    13,    56,    57,    93,   193,    51,   195,    13,
      14,    15,    13,    14,    15,    51,    56,    70,    71,    58,
      11,    13,    14,    15,    13,    14,    15,    58,    51,    56,
     186,    13,    14,    15,   190,    88,    89,   193,   169,   195,
      51,   129,    13,     3,    58,     5,    89,    31,   184,   185,
      57,    55,    54,   189,   107,   191,   192,    58,   194,    13,
      13,    14,    15,    70,   107,   201,    58,   120,     6,    58,
      24,    13,    10,    55,     6,    13,    14,    15,    10,   210,
      51,    13,    14,    15,     5,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,    57,
      57,   297,    55,    60,    33,    63,    58,   243,   161,     6,
      54,    63,   292,   300,   294,   302,    13,    14,    15,   250,
       6,    12,    13,     6,    10,   312,   306,    13,    14,    15,
      13,    14,    15,    13,    56,    13,   292,    59,   294,    56,
       6,   277,    59,   279,   300,     5,   302,    13,    14,    15,
     306,    56,    56,   289,    59,    59,   312,    13,   246,    13,
      16,    17,    56,   216,    20,    21,    22,    23,    24,    25,
      26,    13,    56,    57,    16,    17,    58,    59,    20,    21,
      22,    23,    24,    25,    26,    10,    58,    59,    13,    14,
      15,     4,    62,    63,     7,    51,     9,    13,    14,    15,
      13,    57,    58,    58,    59,    18,    19,    57,    64,    51,
      13,    24,    58,    59,    27,    57,    51,    52,    53,    58,
      59,    34,    64,    36,    37,    57,    39,    40,     7,    57,
       9,    58,    59,    13,    13,    58,    59,    13,    51,    18,
      19,    62,    63,    58,    59,    24,    58,    59,    27,    28,
      29,    16,    57,    55,    58,    34,    57,    36,    37,    13,
      39,    40,     7,    61,     9,    54,    54,    13,    13,    13,
      57,    54,    51,    18,    19,    57,    54,    13,    55,    24,
       3,    56,    27,    55,    54,    56,    60,    56,    56,    34,
      35,    36,    37,    57,    39,    40,     7,    57,     9,    57,
      57,    56,    13,    13,    55,    55,    51,    18,    19,    13,
      57,    54,    13,    24,    55,    57,    27,    57,    57,    57,
      57,    54,    13,    34,    58,    36,    37,    38,    39,    40,
       7,    58,     9,    13,    63,    57,    13,    57,    55,    58,
      51,    18,    19,    58,    58,    30,    55,    24,    56,    56,
      27,    28,     5,    37,    56,    42,    56,    34,    56,    36,
      37,    56,    39,    40,     7,    15,     9,    56,    31,    65,
      13,   161,    88,    34,    51,    18,    19,    34,    -1,    -1,
      -1,    24,    95,    -1,    27,    -1,    -1,   122,    -1,    -1,
      -1,    34,    -1,    36,    37,    -1,    39,    40,     7,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    51,    18,
      19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    37,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    -1,    -1,
      -1,    58,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    58,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    58,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    -1,    -1,    -1,    58,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    58,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    -1,    -1,
      -1,    58,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    -1,    56,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    -1,    56,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    -1,    56,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    41,    42,    43,    44,    45,    46,    -1,
      -1,    49,    50,    51,    52,    53,    41,    42,    43,    44,
      45,    46,    -1,    -1,    49,    50,    51,    52,    53
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    11,    66,    67,    13,    68,    69,    70,     0,    31,
      73,    12,    69,    54,    13,    74,    75,    76,     5,    81,
      13,    14,    15,    71,    72,    96,    33,    32,    75,    54,
      10,    82,    83,    96,    86,    87,    88,    96,    55,    72,
      13,    13,     5,    13,     6,    83,    13,    51,    84,    85,
      87,    88,     8,    13,    51,    56,    71,    77,    78,    96,
      57,    57,    60,    13,    56,    59,    57,    57,    13,     6,
      77,     6,    78,    96,    13,    94,    95,    96,    58,    94,
      16,    57,    85,    58,    58,    94,    57,    55,     6,    79,
      80,    96,    13,    57,    58,    59,    13,    51,    58,    61,
      58,    94,    54,    54,    58,    58,    94,    79,    55,    80,
      13,    58,    94,    13,    51,    93,    95,    13,    58,     3,
       5,    89,    90,    97,    89,    54,    54,    58,    55,    57,
      56,    58,    13,    56,    59,     7,     9,    13,    18,    19,
      24,    27,    34,    36,    37,    39,    40,    51,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,     6,
      10,    91,    92,    96,    55,    97,    55,    89,    89,    54,
      58,    94,    56,    13,    51,    56,    16,    17,    20,    21,
      22,    23,    25,    26,    51,    57,    64,   102,   109,    60,
      57,    57,    57,    98,    57,    98,    56,    56,   102,     4,
      99,    62,    63,    13,     6,    92,    93,    55,    55,    89,
      54,    58,    13,    57,    57,    57,    57,    57,   109,   109,
     102,    57,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    56,   109,   102,   109,   109,
      35,   109,    36,    62,   109,    13,    57,    56,    55,    89,
      54,    58,    58,    13,    96,    13,    58,    58,   109,   110,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,    61,    58,    58,    58,    57,    58,    57,
     109,    56,    94,    55,    89,    58,    58,    58,    58,    59,
      56,    56,    30,   109,    37,   109,    56,    58,    55,   109,
      98,    58,    98,    58,    93,    28,    29,    56,    38,    56,
      56,    56,    98,    56,    28,    56
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    65,    66,    66,    67,    67,    68,    68,    69,    70,
      71,    71,    72,    73,    73,    74,    74,    75,    75,    75,
      76,    76,    77,    77,    78,    78,    79,    79,    80,    80,
      81,    81,    82,    82,    83,    83,    84,    84,    85,    85,
      85,    85,    85,    85,    85,    86,    86,    87,    87,    87,
      87,    88,    89,    89,    90,    90,    91,    91,    92,    92,
      93,    93,    93,    93,    94,    94,    95,    95,    96,    96,
      96,    97,    98,    98,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,   100,   101,   101,   102,   102,
     102,   102,   103,   104,   105,   105,   106,   107,   108,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   110,   110
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     4,     3,     0,     2,     1,     4,     1,
       2,     1,     3,     3,     0,     2,     1,     8,     7,     6,
       1,     3,     2,     1,     6,     5,     2,     1,     8,     7,
       3,     0,     2,     1,     3,     7,     3,     1,     1,     2,
       4,     4,     3,     5,     4,     2,     1,     8,     7,     9,
       8,     7,     2,     1,     3,     2,     2,     1,     3,     7,
       3,     4,     2,     1,     3,     1,     2,     3,     1,     1,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     3,     4,     5,     1,     4,
       3,     1,     5,     5,    10,     8,     8,     7,     7,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     1,     2,     3,     4,
       3,     3,     1,     4,     4,     4,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 59 "yacc_file.y"
                                                                    {          
        (yyval.no) = createTree(NULL, NULL, "void", NODE_ROOT, alloc_5((yyvsp[-4].no), (yyvsp[-3].no), (yyvsp[-2].no), (yyvsp[-1].no), (yyvsp[0].no)) , 5, NULL, NULL); 
        printTypeTable();
        printGSymbolTable();
        printTree((yyval.no), NULL, 0);
        compile((yyval.no));
    }
#line 1719 "y.tab.c"
    break;

  case 3:
#line 66 "yacc_file.y"
                                                      {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_ROOT, alloc_5((yyvsp[-3].no), (yyvsp[-2].no), (yyvsp[-1].no), NULL, (yyvsp[0].no)), 5, NULL, NULL); 
        printTypeTable();
        printGSymbolTable();
        printTree((yyval.no), NULL, 0);
        compile((yyval.no));
    }
#line 1731 "y.tab.c"
    break;

  case 4:
#line 76 "yacc_file.y"
                                        { (yyval.no) = (yyvsp[-1].no);  }
#line 1737 "y.tab.c"
    break;

  case 5:
#line 77 "yacc_file.y"
      { (yyval.no) = NULL; }
#line 1743 "y.tab.c"
    break;

  case 6:
#line 79 "yacc_file.y"
                                  {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CONNECTOR,  alloc_2((yyvsp[-1].no), (yyvsp[0].no)) , 2 , NULL, NULL);
    }
#line 1751 "y.tab.c"
    break;

  case 7:
#line 82 "yacc_file.y"
              { (yyval.no) = (yyvsp[0].no); }
#line 1757 "y.tab.c"
    break;

  case 8:
#line 84 "yacc_file.y"
                                          {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_TYPEDEF,  alloc_2((yyvsp[-3].no), (yyvsp[-1].no)) , 2 , NULL, NULL);
        setUserDefType((yyval.no));
    }
#line 1766 "y.tab.c"
    break;

  case 9:
#line 89 "yacc_file.y"
              { TInstall((yyvsp[0].no)->name, 1, NULL, USER_DEF, NULL); (yyval.no) = (yyvsp[0].no); }
#line 1772 "y.tab.c"
    break;

  case 10:
#line 93 "yacc_file.y"
                                        {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CONNECTOR,  alloc_2((yyvsp[-1].no), (yyvsp[0].no)) , 2 , NULL, NULL);
    }
#line 1780 "y.tab.c"
    break;

  case 11:
#line 96 "yacc_file.y"
                { (yyval.no) = (yyvsp[0].no); }
#line 1786 "y.tab.c"
    break;

  case 12:
#line 98 "yacc_file.y"
                        {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_PARAM,  alloc_2((yyvsp[-2].no), (yyvsp[-1].no)) , 2 , NULL, NULL);
    }
#line 1794 "y.tab.c"
    break;

  case 13:
#line 104 "yacc_file.y"
                                            { 
        (yyval.no) = (yyvsp[-1].no); 
        printClassTable();
    }
#line 1803 "y.tab.c"
    break;

  case 14:
#line 108 "yacc_file.y"
      { (yyval.no) = NULL; }
#line 1809 "y.tab.c"
    break;

  case 15:
#line 110 "yacc_file.y"
                                     {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CONNECTOR,  alloc_2((yyvsp[-1].no), (yyvsp[0].no)) , 2 , NULL, NULL); 
    }
#line 1817 "y.tab.c"
    break;

  case 16:
#line 113 "yacc_file.y"
               { (yyval.no) = (yyvsp[0].no); }
#line 1823 "y.tab.c"
    break;

  case 17:
#line 115 "yacc_file.y"
                                                                           {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CLASS_DEF,  alloc_4((yyvsp[-7].no), (yyvsp[-4].no), (yyvsp[-3].no), (yyvsp[-1].no)) , 4 , NULL, NULL);
    }
#line 1831 "y.tab.c"
    break;

  case 18:
#line 118 "yacc_file.y"
                                                         {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CLASS_DEF,  alloc_4((yyvsp[-6].no), NULL, (yyvsp[-3].no), (yyvsp[-1].no)) , 4 , NULL, NULL);
    }
#line 1839 "y.tab.c"
    break;

  case 19:
#line 121 "yacc_file.y"
                                                {
        printf("Missing Function Declaration.\n");
        printf("If no functions are declared, recommended to use User Defined types\n");
        exit(1);
    }
#line 1849 "y.tab.c"
    break;

  case 20:
#line 127 "yacc_file.y"
           { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CNAME,  alloc_2((yyvsp[0].no), NULL) , 2, NULL, NULL); 
    }
#line 1857 "y.tab.c"
    break;

  case 21:
#line 130 "yacc_file.y"
                    { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CNAME,  alloc_2((yyvsp[-2].no), (yyvsp[0].no)) , 2, NULL, NULL);
    }
#line 1865 "y.tab.c"
    break;

  case 22:
#line 135 "yacc_file.y"
                              { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CONNECTOR,  alloc_2((yyvsp[-1].no), (yyvsp[0].no)) , 2 , NULL, NULL); 
    }
#line 1873 "y.tab.c"
    break;

  case 23:
#line 138 "yacc_file.y"
            { (yyval.no) = (yyvsp[0].no); }
#line 1879 "y.tab.c"
    break;

  case 24:
#line 140 "yacc_file.y"
                                      { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_MDECL,  alloc_3((yyvsp[-5].no), (yyvsp[-4].no), (yyvsp[-2].no)) , 3 , NULL, NULL); 
    }
#line 1887 "y.tab.c"
    break;

  case 25:
#line 143 "yacc_file.y"
                          { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_MDECL,  alloc_3((yyvsp[-4].no), (yyvsp[-3].no), NULL) , 3 , NULL, NULL); 
    }
#line 1895 "y.tab.c"
    break;

  case 26:
#line 147 "yacc_file.y"
                               { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CONNECTOR,  alloc_2((yyvsp[-1].no), (yyvsp[0].no)) , 2 , NULL, NULL); 
    }
#line 1903 "y.tab.c"
    break;

  case 27:
#line 150 "yacc_file.y"
           { (yyval.no) = (yyvsp[0].no); }
#line 1909 "y.tab.c"
    break;

  case 28:
#line 152 "yacc_file.y"
                                                  { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_MDEF, alloc_4((yyvsp[-7].no), (yyvsp[-6].no), (yyvsp[-4].no), (yyvsp[-1].no)), 4, NULL, NULL);
    }
#line 1917 "y.tab.c"
    break;

  case 29:
#line 155 "yacc_file.y"
                                       { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_MDEF, alloc_4((yyvsp[-6].no), (yyvsp[-5].no), NULL, (yyvsp[-1].no)), 4, NULL, NULL);
    }
#line 1925 "y.tab.c"
    break;

  case 30:
#line 161 "yacc_file.y"
                                    { (yyval.no) = (yyvsp[-1].no); }
#line 1931 "y.tab.c"
    break;

  case 31:
#line 162 "yacc_file.y"
      { (yyval.no) = NULL; }
#line 1937 "y.tab.c"
    break;

  case 32:
#line 164 "yacc_file.y"
                            {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CONNECTOR,  alloc_2((yyvsp[-1].no), (yyvsp[0].no)) , 2 , NULL, NULL); 
    }
#line 1945 "y.tab.c"
    break;

  case 33:
#line 167 "yacc_file.y"
            { (yyval.no) = (yyvsp[0].no); }
#line 1951 "y.tab.c"
    break;

  case 34:
#line 169 "yacc_file.y"
                         {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_GDECL,  alloc_2((yyvsp[-2].no), (yyvsp[-1].no)) , 2 , NULL, NULL); 
        setGTypes( (yyvsp[-1].no), (yyvsp[-2].no)->type, NULL );
    }
#line 1960 "y.tab.c"
    break;

  case 35:
#line 173 "yacc_file.y"
                                           {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_TUPLE, alloc_3((yyvsp[-5].no), (yyvsp[-3].no), (yyvsp[-1].no)), 3, NULL, NULL);
        initTuple((yyval.no)->children, NULL);
    }
#line 1969 "y.tab.c"
    break;

  case 36:
#line 178 "yacc_file.y"
                          {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CONNECTOR,  alloc_2((yyvsp[-2].no), (yyvsp[0].no)) , 2 , NULL, NULL); 
    }
#line 1977 "y.tab.c"
    break;

  case 37:
#line 181 "yacc_file.y"
          { (yyval.no) = (yyvsp[0].no); }
#line 1983 "y.tab.c"
    break;

  case 38:
#line 183 "yacc_file.y"
         { (yyval.no) = (yyvsp[0].no); }
#line 1989 "y.tab.c"
    break;

  case 39:
#line 184 "yacc_file.y"
             { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_PTR,  alloc_1((yyvsp[0].no)) , 1 , NULL, NULL); 
    }
#line 1997 "y.tab.c"
    break;

  case 40:
#line 187 "yacc_file.y"
                      { 
        (yyval.no) = createTree(NULL, NULL, "void", NODE_ARR_TYPE,  alloc_2((yyvsp[-3].no), (yyvsp[-1].no)) , 2 , NULL, NULL); 
    }
#line 2005 "y.tab.c"
    break;

  case 41:
#line 190 "yacc_file.y"
                           {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_GDECL_FUNC,  alloc_2((yyvsp[-3].no), (yyvsp[-1].no)) , 2 , NULL, NULL); 
    }
#line 2013 "y.tab.c"
    break;

  case 42:
#line 193 "yacc_file.y"
                 {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_GDECL_FUNC,  alloc_2((yyvsp[-2].no), NULL) , 2 , NULL, NULL); 
    }
#line 2021 "y.tab.c"
    break;

  case 43:
#line 196 "yacc_file.y"
                               {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_GDECL_PTRFUNC,  alloc_2((yyvsp[-3].no), (yyvsp[-1].no)) , 2 , NULL, NULL); 
    }
#line 2029 "y.tab.c"
    break;

  case 44:
#line 199 "yacc_file.y"
                     {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_GDECL_PTRFUNC,  alloc_2((yyvsp[-2].no), NULL) , 2 , NULL, NULL); 
    }
#line 2037 "y.tab.c"
    break;

  case 45:
#line 205 "yacc_file.y"
                           {
        (yyval.no) = createTree(NULL, NULL, "void", NODE_CONNECTOR, alloc_2((yyvsp[-1].no), (yyvsp