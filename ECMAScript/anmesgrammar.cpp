
/*  A Bison parser, made from anmesgrammar.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ASSIGN_SYMBOL	258
#define	BITWISE_AND	259
#define	BITWISE_AND_EQUALS	260
#define	BITWISE_EXCLUSIVE_OR	261
#define	BITWISE_EXCLUSIVE_OR_EQUALS	262
#define	BITWISE_OR	263
#define	BITWISE_OR_EQUALS	264
#define	BITWISE_SHIFT_LEFT	265
#define	BITWISE_SHIFT_LEFT_EQUALS	266
#define	BITWISE_SHIFT_RIGHT	267
#define	BITWISE_SHIFT_RIGHT_EQUALS	268
#define	BITWISE_SHIFT_RIGHT_ZERO_FILL	269
#define	BITWISE_SHIFT_RIGHT_ZERO_FILL_EQUALS	270
#define	BREAK	271
#define	CASE	272
#define	CLOSE_PARENTHESIS	273
#define	CLOSE_SQ_BRACKETS	274
#define	COLON	275
#define	COMMA	276
#define	CONTINUE	277
#define	DECREMENT	278
#define	DEFAULT	279
#define	DELETE_TOKEN	280
#define	DIV	281
#define	DIV_EQUALS	282
#define	DOT	283
#define	ELSE	284
#define	END_BLOCK	285
#define	EQUALS	286
#define	FALSE_TOKEN	287
#define	FOR	288
#define	FUNCTION	289
#define	GREATER_THAN	290
#define	GT_EQUAL	291
#define	IF	292
#define	INCREMENT	293
#define	INFINITY	294
#define	IN_TOKEN	295
#define	LESS_THAN	296
#define	LINE_TERMINATOR	297
#define	LOGICAL_AND	298
#define	LOGICAL_OR	299
#define	LOGICAL_NOT	300
#define	LS_EQUAL	301
#define	MINUS	302
#define	MINUS_EQUALS	303
#define	MOD	304
#define	MOD_EQUALS	305
#define	MULTIPLY	306
#define	MULTIPLY_EQUALS	307
#define	NEW	308
#define	NOT_EQUAL	309
#define	NULL_TOKEN	310
#define	ONES_COMPLIMENT	311
#define	OPEN_PARENTHESIS	312
#define	OPEN_SQ_BRACKETS	313
#define	PLUS	314
#define	PLUS_EQUALS	315
#define	QUERY	316
#define	RETURN	317
#define	SWITCH	318
#define	SEMICOLON	319
#define	START_BLOCK	320
#define	THIS_TOKEN	321
#define	TRUE_TOKEN	322
#define	TYPEOF	323
#define	UNDEFINED_TOKEN	324
#define	VAR	325
#define	VOID_SYMBOL	326
#define	WHILE	327
#define	WITH	328
#define	IDENTIFIER	329
#define	NUMERIC	330
#define	STRING	331
#define	HIGHER_THAN_FALSE	332
#define	LOWER_THAN_CLOSE_PARENTHESIS	333

 
#include "stdafx.h"

#include "anmescompiler.h"
#include "anmessymbol.h"
#include "anmesexpression.h"
#include "anmesstatement.h"

#define yyerror(message) Yyerror(message)
#define yylex() Yylex()
#define yyparse() CAnmECMAScriptCompiler::Yyparse()
#define yylval m_Yylval
#define yyval m_Yyval
#define pcyytoken m_Pcyytoken
#define pcyyerrfl m_Pcyyerrfl
#define pcyyerrct m_Pcyyerrct
#define yyv m_Yyv

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		268
#define	YYFLAG		-32768
#define	YYNTBASE	79

#define YYTRANSLATE(x) ((unsigned)(x) <= 333 ? yytranslate[x] : 138)

static const char yytranslate[] = {     0,
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
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     7,     9,    11,    13,    15,    17,    19,
    21,    23,    25,    27,    29,    31,    33,    34,    35,    44,
    45,    47,    49,    53,    55,    58,    59,    64,    67,    71,
    73,    76,    83,    84,    87,    93,    99,   107,   117,   128,
   136,   146,   149,   152,   156,   159,   165,   166,   171,   174,
   176,   179,   180,   186,   187,   192,   198,   200,   204,   206,
   209,   212,   214,   215,   217,   219,   221,   223,   225,   227,
   229,   233,   235,   240,   244,   245,   250,   252,   253,   257,
   260,   265,   269,   272,   276,   278,   282,   284,   286,   288,
   291,   294,   296,   299,   302,   305,   308,   311,   314,   317,
   320,   323,   325,   329,   333,   337,   339,   343,   347,   349,
   353,   357,   361,   363,   367,   371,   375,   379,   381,   385,
   389,   391,   395,   397,   401,   403,   407,   409,   413,   415,
   419,   421,   427,   429,   433,   435,   437,   439,   441,   443,
   445,   447,   449,   451,   453,   455,   457,   459,   463,   465
};

static const short yyrhs[] = {    80,
     0,    81,     0,    80,    81,     0,    82,     0,    83,     0,
    89,     0,    91,     0,    92,     0,    93,     0,    94,     0,
    96,     0,    97,     0,    98,     0,    99,     0,   100,     0,
   107,     0,     0,     0,    34,    74,    84,    57,    86,    85,
    18,    89,     0,     0,    87,     0,    74,     0,    87,    21,
    74,     0,    82,     0,    88,    82,     0,     0,    65,    90,
    88,    30,     0,    65,    30,     0,    70,   108,    64,     0,
    64,     0,   136,    64,     0,    37,    57,   136,    18,    82,
    95,     0,     0,    29,    82,     0,    72,    57,   136,    18,
    82,     0,    72,    57,    32,    18,    82,     0,    72,    57,
   120,   135,   134,    18,    82,     0,    33,    57,   137,    64,
   137,    64,   137,    18,    82,     0,    33,    57,    70,   108,
    64,   137,    64,   137,    18,    82,     0,    33,    57,   120,
    40,   136,    18,    82,     0,    33,    57,    70,    74,   111,
    40,   136,    18,    82,     0,    22,    64,     0,    16,    64,
     0,    62,   136,    64,     0,    62,    64,     0,    63,    57,
   136,    18,   101,     0,     0,    65,   102,   103,    30,     0,
    65,    30,     0,   104,     0,   103,   104,     0,     0,    17,
   136,    20,   105,    88,     0,     0,    24,    20,   106,    88,
     0,    73,    57,   136,    18,    82,     0,   109,     0,   108,
    21,   109,     0,    74,     0,    74,   110,     0,     3,   134,
     0,   110,     0,     0,    66,     0,    74,     0,    75,     0,
    76,     0,    55,     0,    67,     0,    32,     0,    57,   136,
    18,     0,   112,     0,   113,    58,   136,    19,     0,   113,
    28,    74,     0,     0,    53,    74,   114,   118,     0,   113,
     0,     0,   113,   117,   118,     0,   116,   118,     0,   116,
    58,   136,    19,     0,   116,    28,    74,     0,    57,    18,
     0,    57,   119,    18,     0,   134,     0,   119,    21,   134,
     0,   115,     0,   116,     0,   120,     0,   120,    38,     0,
   120,    23,     0,   121,     0,    25,   122,     0,    71,   122,
     0,    68,   122,     0,    38,   122,     0,    23,   122,     0,
    59,   122,     0,    47,   122,     0,    56,   122,     0,    45,
   122,     0,   122,     0,   123,    51,   122,     0,   123,    26,
   122,     0,   123,    49,   122,     0,   123,     0,   124,    59,
   123,     0,   124,    47,   123,     0,   124,     0,   125,    10,
   124,     0,   125,    12,   124,     0,   125,    14,   124,     0,
   125,     0,   126,    41,   125,     0,   126,    35,   125,     0,
   126,    46,   125,     0,   126,    36,   125,     0,   126,     0,
   127,    31,   126,     0,   127,    54,   126,     0,   127,     0,
   128,     4,   127,     0,   128,     0,   129,     6,   128,     0,
   129,     0,   130,     8,   129,     0,   130,     0,   131,    43,
   130,     0,   131,     0,   132,    44,   131,     0,   132,     0,
   132,    61,   134,    20,   134,     0,   133,     0,   120,   135,
   134,     0,     3,     0,    52,     0,    27,     0,    50,     0,
    60,     0,    48,     0,    11,     0,    13,     0,    15,     0,
     5,     0,     7,     0,     9,     0,   134,     0,   136,    21,
   134,     0,   136,     0,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   161,   166,   168,   175,   180,   185,   187,   189,   191,   193,
   195,   197,   199,   201,   203,   205,   211,   215,   218,   225,
   227,   233,   238,   248,   254,   264,   271,   277,   282,   287,
   292,   301,   344,   346,   353,   358,   364,   374,   379,   384,
   389,   397,   405,   415,   420,   429,   437,   444,   450,   457,
   463,   470,   475,   478,   484,   489,   497,   499,   508,   514,
   523,   529,   531,   538,   543,   550,   555,   560,   565,   570,
   575,   584,   589,   594,   599,   604,   612,   626,   631,   635,
   637,   639,   644,   646,   653,   657,   666,   671,   680,   685,
   691,   701,   706,   710,   714,   718,   723,   728,   733,   738,
   743,   753,   758,   764,   769,   780,   785,   790,   800,   805,
   810,   816,   826,   831,   836,   841,   846,   856,   861,   866,
   877,   882,   893,   898,   908,   913,   923,   928,   938,   943,
   953,   958,   965,   970,   980,   985,   990,   995,  1000,  1005,
  1010,  1015,  1020,  1025,  1030,  1035,  1045,  1050,  1059,  1064
};

static const char * const yytname[] = {   "$","error","$undefined.","ASSIGN_SYMBOL",
"BITWISE_AND","BITWISE_AND_EQUALS","BITWISE_EXCLUSIVE_OR","BITWISE_EXCLUSIVE_OR_EQUALS",
"BITWISE_OR","BITWISE_OR_EQUALS","BITWISE_SHIFT_LEFT","BITWISE_SHIFT_LEFT_EQUALS",
"BITWISE_SHIFT_RIGHT","BITWISE_SHIFT_RIGHT_EQUALS","BITWISE_SHIFT_RIGHT_ZERO_FILL",
"BITWISE_SHIFT_RIGHT_ZERO_FILL_EQUALS","BREAK","CASE","CLOSE_PARENTHESIS","CLOSE_SQ_BRACKETS",
"COLON","COMMA","CONTINUE","DECREMENT","DEFAULT","DELETE_TOKEN","DIV","DIV_EQUALS",
"DOT","ELSE","END_BLOCK","EQUALS","FALSE_TOKEN","FOR","FUNCTION","GREATER_THAN",
"GT_EQUAL","IF","INCREMENT","INFINITY","IN_TOKEN","LESS_THAN","LINE_TERMINATOR",
"LOGICAL_AND","LOGICAL_OR","LOGICAL_NOT","LS_EQUAL","MINUS","MINUS_EQUALS","MOD",
"MOD_EQUALS","MULTIPLY","MULTIPLY_EQUALS","NEW","NOT_EQUAL","NULL_TOKEN","ONES_COMPLIMENT",
"OPEN_PARENTHESIS","OPEN_SQ_BRACKETS","PLUS","PLUS_EQUALS","QUERY","RETURN",
"SWITCH","SEMICOLON","START_BLOCK","THIS_TOKEN","TRUE_TOKEN","TYPEOF","UNDEFINED_TOKEN",
"VAR","VOID_SYMBOL","WHILE","WITH","IDENTIFIER","NUMERIC","STRING","HIGHER_THAN_FALSE",
"LOWER_THAN_CLOSE_PARENTHESIS","Program","SourceElements","SourceElement","Statement",
"FunctionDeclaration","@1","@2","OptionalParameterList","FormalParameterList",
"StatementList","Block","@3","VariableStatement","EmptyStatement","ExpressionStatement",
"IfStatement","OptionalElse","IterationExpression","ContinueStatement","BreakStatement",
"ReturnStatement","SwitchStatement","SwitchBlock","@4","CaseStatementList","CaseStatement",
"@5","@6","WithStatement","VariableDeclarationList","VariableDeclaration","Initializer",
"OptionalInitializer","PrimaryExpression","MemberExpression","@7","NewExpression",
"CallExpression","@8","Arguments","ArgumentList","LeftHandSideExpression","PostfixExpression",
"UnaryExpression","MultiplicativeExpression","AdditiveExpression","ShiftExpression",
"RelationalExpression","EqualityExpression","BitwiseANDExpression","BitwiseXORExpression",
"BitwiseORExpression","LogicalANDExpression","LogicalORExpression","ConditionalExpression",
"AssignmentExpression","AssignmentOperator","Expression","OptionalExpression",
""
};
#endif

static const short yyr1[] = {     0,
    79,    80,    80,    81,    81,    82,    82,    82,    82,    82,
    82,    82,    82,    82,    82,    82,    84,    85,    83,    86,
    86,    87,    87,    88,    88,    90,    89,    89,    91,    92,
    93,    94,    95,    95,    96,    96,    96,    96,    96,    96,
    96,    97,    98,    99,    99,   100,   102,   101,   101,   103,
   103,   105,   104,   106,   104,   107,   108,   108,   109,   109,
   110,   111,   111,   112,   112,   112,   112,   112,   112,   112,
   112,   113,   113,   113,   114,   113,   115,   117,   116,   116,
   116,   116,   118,   118,   119,   119,   120,   120,   121,   121,
   121,   122,   122,   122,   122,   122,   122,   122,   122,   122,
   122,   123,   123,   123,   123,   124,   124,   124,   125,   125,
   125,   125,   126,   126,   126,   126,   126,   127,   127,   127,
   128,   128,   129,   129,   130,   130,   131,   131,   132,   132,
   133,   133,   134,   134,   135,   135,   135,   135,   135,   135,
   135,   135,   135,   135,   135,   135,   136,   136,   137,   137
};

static const short yyr2[] = {     0,
     1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     0,     0,     8,     0,
     1,     1,     3,     1,     2,     0,     4,     2,     3,     1,
     2,     6,     0,     2,     5,     5,     7,     9,    10,     7,
     9,     2,     2,     3,     2,     5,     0,     4,     2,     1,
     2,     0,     5,     0,     4,     5,     1,     3,     1,     2,
     2,     1,     0,     1,     1,     1,     1,     1,     1,     1,
     3,     1,     4,     3,     0,     4,     1,     0,     3,     2,
     4,     3,     2,     3,     1,     3,     1,     1,     1,     2,
     2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     1,     3,     3,     3,     1,     3,     3,     1,     3,
     3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
     1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
     1,     5,     1,     3,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     3,     1,     0
};

static const short yydefact[] = {     0,
     0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
     0,     0,    68,     0,     0,     0,     0,     0,    30,    26,
    64,    69,     0,     0,     0,     0,     0,    65,    66,    67,
     1,     2,     4,     5,     6,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    72,    77,    87,    88,    89,
    92,   102,   106,   109,   113,   118,   121,   123,   125,   127,
   129,   131,   133,   147,     0,    43,    42,    89,    97,    93,
   150,    17,     0,    96,   101,    99,    75,   100,     0,    98,
    45,     0,     0,    28,     0,    95,    59,     0,    57,    94,
     0,     0,     3,     0,     0,     0,     0,     0,     0,    80,
   135,   144,   145,   146,   141,   142,   143,    91,   137,    90,
   140,   138,   136,   139,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    31,     0,    89,   149,
     0,     0,     0,     0,    71,    44,     0,    24,     0,     0,
    60,     0,    29,    70,    89,     0,     0,    74,     0,    79,
    82,    83,     0,    85,     0,   134,   104,   105,   103,   108,
   107,   110,   111,   112,   115,   117,   114,   116,   119,   120,
   122,   124,   126,   128,   130,     0,   148,    59,     0,     0,
   150,    20,     0,    76,     0,    27,    25,    61,    58,     0,
     0,     0,     0,    73,    84,     0,    81,     0,    60,     0,
   150,     0,     0,    22,    18,    21,    33,    47,    46,    36,
   134,    35,    56,    86,   132,     0,     0,     0,   150,     0,
     0,     0,    32,    49,     0,     0,     0,   150,    40,     0,
     0,    23,    34,     0,     0,     0,    50,    37,     0,     0,
     0,    19,     0,    54,    48,    51,    41,     0,    38,    52,
     0,    39,     0,    55,    53,     0,     0,     0
};

static const short yydefgoto[] = {   266,
    31,    32,   148,    34,   142,   230,   215,   216,   149,    35,
    85,    36,    37,    38,    39,   233,    40,    41,    42,    43,
    44,   219,   235,   246,   247,   263,   261,    45,    88,    89,
   151,   210,    46,    47,   144,    48,    49,    96,   100,   163,
    50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
    60,    61,    62,    63,    64,   115,    65,   141
};

static const short yypact[] = {   222,
   -50,    -4,   523,   523,-32768,     8,     5,    33,   523,   523,
   523,    10,-32768,   523,   523,   523,   443,    49,-32768,   114,
-32768,-32768,   523,    37,   523,    91,    93,-32768,-32768,-32768,
   222,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,     9,-32768,    13,   157,
-32768,-32768,   -19,    17,    77,    47,    -9,   149,   148,   147,
   113,   -15,-32768,-32768,   -13,-32768,-32768,    30,-32768,-32768,
   483,-32768,   523,-32768,-32768,-32768,-32768,-32768,    21,-32768,
-32768,   -12,   523,-32768,   344,-32768,   154,    -8,-32768,-32768,
   563,   523,-32768,    84,   523,   116,   100,   403,   523,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   523,   523,   523,   523,   523,   523,
   523,   523,   523,   523,   523,   523,   523,   523,   523,   523,
   523,   523,   523,   523,   523,   523,-32768,   101,   156,   160,
   121,   125,    80,   116,-32768,-32768,    81,-32768,   283,   523,
-32768,    37,-32768,   168,   157,    82,    86,-32768,   117,-32768,
-32768,-32768,    87,-32768,   118,-32768,-32768,-32768,-32768,   -19,
   -19,    17,    17,    17,    77,    77,    77,    77,    47,    47,
    -9,   149,   148,   147,   113,   158,-32768,    22,    -3,   523,
   523,   115,   344,-32768,   122,-32768,-32768,-32768,-32768,   344,
   523,   344,   344,-32768,-32768,   523,-32768,   523,   150,   151,
   523,    92,   124,-32768,-32768,   171,   169,   180,-32768,-32768,
   181,-32768,-32768,-32768,-32768,   523,   137,   344,   523,   193,
   139,   344,-32768,-32768,    31,   344,    94,   523,-32768,   196,
   153,-32768,-32768,   523,   199,    56,-32768,-32768,   344,   202,
   344,-32768,    54,-32768,-32768,-32768,-32768,   344,-32768,-32768,
   344,-32768,   344,   344,   344,   221,   223,-32768
};

static const short yypgoto[] = {-32768,
-32768,   191,     0,-32768,-32768,-32768,-32768,-32768,  -118,   -16,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   -20,-32768,-32768,-32768,    89,    79,
    41,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -94,-32768,
     1,-32768,    24,    27,  -102,   -30,    23,   103,   104,   102,
   106,   107,-32768,-32768,   -92,    85,   -14,  -175
};


#define	YYLAST		639


static const short yytable[] = {    33,
    79,   160,    82,    68,    68,   164,   116,   136,   136,    68,
    68,    68,   152,    66,    68,   213,    68,   152,   172,   173,
   174,   128,   166,    68,   150,    68,    69,    70,   134,   117,
    33,   118,    74,    75,    76,   227,    94,    78,   145,    80,
    97,   136,   186,   187,   129,   135,    86,   244,    90,   194,
   137,   146,   108,   240,   245,   153,   140,   198,   143,    67,
   211,   -63,   250,   119,    71,   -78,    95,   110,   147,    98,
    99,   139,   244,   260,   136,   120,   156,   157,    72,   245,
   159,   124,   125,    77,   165,   255,   121,   126,   122,    73,
   123,   155,   127,   175,   176,   177,   178,   193,   195,   202,
   136,   136,   136,   203,   205,    83,   136,   206,   221,   228,
    87,   249,   136,   224,   136,   225,    68,    68,    68,    68,
    68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
    68,    68,    68,    68,    68,   204,   207,   136,   136,   167,
   168,   169,   264,    84,   265,   170,   171,    91,   197,    92,
   179,   180,   130,   131,   132,   133,   150,   158,   101,   101,
   102,   102,   103,   103,   104,   104,   105,   105,   106,   106,
   107,   107,    98,   161,   188,   212,   140,   208,   108,   108,
   136,   192,   109,   109,   191,   200,   218,   229,   214,   -62,
   226,   231,   217,   110,   110,   190,   140,   232,   236,   220,
   238,   222,   223,   111,   111,   112,   112,   113,   113,   234,
   241,   237,   242,   251,   140,   114,   114,    20,   254,   258,
   267,    93,   268,   140,   252,   256,   189,   239,   209,   253,
   199,   243,   181,   183,   182,   248,     0,     1,   184,   201,
   185,     0,     0,     2,     3,     0,     4,     0,   257,     0,
   259,     0,     0,     5,     6,     7,     0,   262,     8,     9,
     0,     0,     0,   197,   197,     0,    10,     0,    11,     0,
     0,     0,     0,     0,    12,     0,    13,    14,    15,     0,
    16,     0,     0,    17,    18,    19,    20,    21,    22,    23,
     0,    24,    25,    26,    27,    28,    29,    30,     1,     0,
     0,     0,     0,     0,     2,     3,     0,     4,     0,     0,
     0,     0,   196,     0,     5,     6,     0,     0,     0,     8,
     9,     0,     0,     0,     0,     0,     0,    10,     0,    11,
     0,     0,     0,     0,     0,    12,     0,    13,    14,    15,
     0,    16,     0,     0,    17,    18,    19,    20,    21,    22,
    23,     0,    24,    25,    26,    27,    28,    29,    30,     1,
     0,     0,     0,     0,     0,     2,     3,     0,     4,     0,
     0,     0,     0,     0,     0,     5,     6,     0,     0,     0,
     8,     9,     0,     0,     0,     0,     0,     0,    10,     0,
    11,     0,     0,     0,     0,     0,    12,     0,    13,    14,
    15,     0,    16,     0,     0,    17,    18,    19,    20,    21,
    22,    23,     0,    24,    25,    26,    27,    28,    29,    30,
   162,     0,     0,     0,     0,     3,     0,     4,     0,     0,
     0,     0,     0,     0,     5,     0,     0,     0,     0,     0,
     9,     0,     0,     0,     0,     0,     0,    10,     0,    11,
     0,     0,     0,     0,     0,    12,     0,    13,    14,    15,
     0,    16,     0,     0,     0,     3,     0,     4,    21,    22,
    23,     0,     0,    25,     5,     0,    28,    29,    30,     0,
     9,     0,     0,     0,     0,     0,     0,    10,     0,    11,
     0,     0,     0,     0,     0,    12,     0,    13,    14,    15,
     0,    16,     0,     0,     0,     3,    81,     4,    21,    22,
    23,     0,     0,    25,     5,     0,    28,    29,    30,     0,
     9,     0,     0,     0,     0,     0,     0,    10,     0,    11,
     0,     0,     0,     0,     0,    12,     0,    13,    14,    15,
     0,    16,     0,     0,     0,     3,     0,     4,    21,    22,
    23,     0,   138,    25,     5,     0,    28,    29,    30,     0,
     9,     0,     0,     0,     0,     0,     0,    10,     0,    11,
     0,     0,     0,     0,     0,    12,     0,    13,    14,    15,
     0,    16,     0,     0,     0,     3,     0,     4,    21,    22,
    23,     0,     0,    25,   154,     0,    28,    29,    30,     0,
     9,     0,     0,     0,     0,     0,     0,    10,     0,    11,
     0,     0,     0,     0,     0,    12,     0,    13,    14,    15,
     0,    16,     0,     0,     0,     0,     0,     0,    21,    22,
    23,     0,     0,    25,     0,     0,    28,    29,    30
};

static const short yycheck[] = {     0,
    15,    96,    17,     3,     4,    98,    26,    21,    21,     9,
    10,    11,    21,    64,    14,   191,    16,    21,   121,   122,
   123,    31,   115,    23,     3,    25,     3,     4,    44,    49,
    31,    51,     9,    10,    11,   211,    28,    14,    18,    16,
    28,    21,   135,   136,    54,    61,    23,    17,    25,   144,
    64,    64,    23,   229,    24,    64,    71,   150,    73,    64,
    64,    40,   238,    47,    57,    57,    58,    38,    83,    57,
    58,    71,    17,    20,    21,    59,    91,    92,    74,    24,
    95,    35,    36,    74,    99,    30,    10,    41,    12,    57,
    14,    91,    46,   124,   125,   126,   127,    18,    18,    18,
    21,    21,    21,    18,    18,    57,    21,    21,   201,    18,
    74,    18,    21,   206,    21,   208,   116,   117,   118,   119,
   120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
   130,   131,   132,   133,   134,    19,    19,    21,    21,   116,
   117,   118,   261,    30,   263,   119,   120,    57,   149,    57,
   128,   129,     4,     6,     8,    43,     3,    74,     3,     3,
     5,     5,     7,     7,     9,     9,    11,    11,    13,    13,
    15,    15,    57,    74,    74,   190,   191,    20,    23,    23,
    21,    57,    27,    27,    64,    18,    65,    64,    74,    40,
    40,    21,   193,    38,    38,    40,   211,    29,    18,   200,
    64,   202,   203,    48,    48,    50,    50,    52,    52,    30,
    18,   226,    74,    18,   229,    60,    60,    65,    20,    18,
     0,    31,     0,   238,   241,   246,   138,   228,   188,   244,
   152,   232,   130,   132,   131,   236,    -1,    16,   133,   155,
   134,    -1,    -1,    22,    23,    -1,    25,    -1,   249,    -1,
   251,    -1,    -1,    32,    33,    34,    -1,   258,    37,    38,
    -1,    -1,    -1,   264,   265,    -1,    45,    -1,    47,    -1,
    -1,    -1,    -1,    -1,    53,    -1,    55,    56,    57,    -1,
    59,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
    -1,    70,    71,    72,    73,    74,    75,    76,    16,    -1,
    -1,    -1,    -1,    -1,    22,    23,    -1,    25,    -1,    -1,
    -1,    -1,    30,    -1,    32,    33,    -1,    -1,    -1,    37,
    38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,
    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,    57,
    -1,    59,    -1,    -1,    62,    63,    64,    65,    66,    67,
    68,    -1,    70,    71,    72,    73,    74,    75,    76,    16,
    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,    25,    -1,
    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
    37,    38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
    47,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,
    57,    -1,    59,    -1,    -1,    62,    63,    64,    65,    66,
    67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
    18,    -1,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,
    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
    38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,
    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,    57,
    -1,    59,    -1,    -1,    -1,    23,    -1,    25,    66,    67,
    68,    -1,    -1,    71,    32,    -1,    74,    75,    76,    -1,
    38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,
    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,    57,
    -1,    59,    -1,    -1,    -1,    23,    64,    25,    66,    67,
    68,    -1,    -1,    71,    32,    -1,    74,    75,    76,    -1,
    38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,
    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,    57,
    -1,    59,    -1,    -1,    -1,    23,    -1,    25,    66,    67,
    68,    -1,    70,    71,    32,    -1,    74,    75,    76,    -1,
    38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,
    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,    57,
    -1,    59,    -1,    -1,    -1,    23,    -1,    25,    66,    67,
    68,    -1,    -1,    71,    32,    -1,    74,    75,    76,    -1,
    38,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,
    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,    57,
    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
    68,    -1,    -1,    71,    -1,    -1,    74,    75,    76
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */


/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 4:
{
		GenStatement(yyvsp[0].pStatement);
	;
    break;}
case 6:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 7:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 8:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 9:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 10:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 11:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 12:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 13:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 14:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 15:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 16:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 17:
{
		BeginDeclareFunction(yyvsp[0].idVal);
	;
    break;}
case 18:
{
	;
    break;}
case 19:
{
		EndDeclareFunction(yyvsp[0].pStatement);
	;
    break;}
case 22:
{
		DeclareParam(yyvsp[0].idVal);
	;
    break;}
case 23:
{
		DeclareParam(yyvsp[0].idVal);
	;
    break;}
case 24:
{
		AddStatement(yyvsp[0].pStatement);
	;
    break;}
case 25:
{
		AddStatement(yyvsp[0].pStatement);
	;
    break;}
case 26:
{
		BeginBlock();
	;
    break;}
case 27:
{
		yyval.pStatement = EndBlock();
	;
    break;}
case 28:
{ yyval.pStatement = EmptyStatement(); ;
    break;}
case 29:
{ yyval.pStatement = yyvsp[-1].pStatement; ;
    break;}
case 30:
{ yyval.pStatement = EmptyStatement(); ;
    break;}
case 31:
{
		yyval.pStatement = new CAnmESTopLevelStatement(yyvsp[-1].pExpr);
	;
    break;}
case 32:
{
		yyval.pStatement = new CAnmESIfStatement(yyvsp[-3].pExpr, yyvsp[-1].pStatement, yyvsp[0].pStatement);
	;
    break;}
case 33:
{ yyval.pStatement = EmptyStatement(); ;
    break;}
case 34:
{ yyval.pStatement = yyvsp[0].pStatement; ;
    break;}
case 35:
{
		yyval.pStatement = new CAnmESWhileStatement(yyvsp[-2].pExpr, yyvsp[0].pStatement);
	;
    break;}
case 36:
{
         Yywarning("Unreachable code portion");
    ;
    break;}
case 37:
{

        Yywarning("Possible error in WHILE Expression (e.g. a=b instead of a==b)");

		yyval.pStatement = new CAnmESWhileStatement(AssignmentExpression(yyvsp[-4].pExpr, yyvsp[-3].op, yyvsp[-2].pExpr), yyvsp[0].pStatement);

    ;
    break;}
case 38:
{
		yyval.pStatement = new CAnmESForStatement(yyvsp[-6].pExpr, yyvsp[-4].pExpr, yyvsp[-2].pExpr, yyvsp[0].pStatement);
	;
    break;}
case 39:
{
		yyval.pStatement = NIYStatement("for");
	;
    break;}
case 40:
{
		yyval.pStatement = NIYStatement("for");
	;
    break;}
case 41:
{
		yyval.pStatement = NIYStatement("for");
	;
    break;}
case 42:
{
		yyval.pStatement = ContinueStatement();
	;
    break;}
case 43:
{
		yyval.pStatement = BreakStatement();
	;
    break;}
case 44:
{
		yyval.pStatement = new CAnmESReturnStatement(yyvsp[-1].pExpr);
	;
    break;}
case 45:
{
		yyval.pStatement = new CAnmESReturnStatement();
	;
    break;}
case 46:
{
		yyval.pStatement = new CAnmESSwitchStatement(yyvsp[-2].pExpr, yyvsp[0].pStatement);
	;
    break;}
case 47:
{
		BeginBlock();
	;
    break;}
case 48:
{
		yyval.pStatement = EndBlock();
	;
    break;}
case 49:
{ yyval.pStatement = EmptyStatement(); ;
    break;}
case 50:
{
		AddStatement(yyvsp[0].pStatement);
	;
    break;}
case 51:
{
		AddStatement(yyvsp[0].pStatement);
	;
    break;}
case 52:
{
		BeginCaseBlock(yyvsp[-1].pExpr);
	;
    break;}
case 53:
{
		yyval.pStatement = EndCaseBlock();
	;
    break;}
case 54:
{
		BeginCaseBlock(NULL, TRUE);
	;
    break;}
case 55:
{
		yyval.pStatement = EndCaseBlock();
	;
    break;}
case 56:
{
		yyval.pStatement = NIYStatement("with");
	;
    break;}
case 57:
{ yyval.pStatement = new CAnmESVarStatementList(yyvsp[0].pStatement); ;
    break;}
case 58:
{
		BeginVariableDeclarationList();
		AddVariableDeclaration(yyvsp[0].pStatement);
		yyval.pStatement = EndVariableDeclarationList();
	 ;
    break;}
case 59:
{
		yyval.pStatement = new CAnmESVarStatement(IdentifierExpression(yyvsp[0].idVal));
     ;
    break;}
case 60:
{
		yyval.pStatement = new CAnmESVarStatement(IdentifierExpression(yyvsp[-1].idVal), AssignmentExpression(IdentifierExpression(yyvsp[-1].idVal), eOpASSIGNMENT, yyvsp[0].pExpr));
     ;
    break;}
case 61:
{ yyval.pExpr = yyvsp[0].pExpr; ;
    break;}
case 62:
{ yyval.pExpr = yyvsp[0].pExpr; ;
    break;}
case 63:
{ yyval.pExpr = NULL; ;
    break;}
case 64:
{
		yyval.pExpr = ThisExpression();
	 ;
    break;}
case 65:
{
			   yyval.pExpr = IdentifierExpression(yyvsp[0].idVal);

     ;
    break;}
case 66:
{
			   yyval.pExpr = new CAnmESConstExpression(yyvsp[0].doubleVal);
	 ;
    break;}
case 67:
{
			   yyval.pExpr = new CAnmESConstExpression(yyvsp[0].strVal);
	 ;
    break;}
case 68:
{
			   yyval.pExpr = new CAnmESConstExpression((CAnmESObject *) NULL);
	 ;
    break;}
case 69:
{
			   yyval.pExpr = new CAnmESConstExpression(AnmESBool(TRUE));
	 ;
    break;}
case 70:
{
			   yyval.pExpr = new CAnmESConstExpression(AnmESBool(FALSE));
	 ;
    break;}
case 71:
{
		yyval.pExpr = yyvsp[-1].pExpr;
	 ;
    break;}
case 72:
{
		yyval.pExpr = yyvsp[0].pExpr;
	 ;
    break;}
case 73:
{
		yyval.pExpr = new CAnmESLValueExpression(yyvsp[-3].pExpr, yyvsp[-1].pExpr);
	 ;
    break;}
case 74:
{
		yyval.pExpr = new CAnmESLValueExpression(yyvsp[-2].pExpr, yyvsp[0].idVal);
	 ;
    break;}
case 75:
{
		BeginNew(yyvsp[0].idVal)
	 ;
    break;}
case 76:
{
		yyval.pExpr = EndNew();
	 ;
    break;}
case 77:
{
		 yyval.pExpr = yyvsp[0].pExpr;
	 ;
    break;}
case 78:
{
		BeginFunctionCall(yyvsp[0].pExpr);
	 ;
    break;}
case 79:
{
		yyval.pExpr = EndFunctionCall();
	 ;
    break;}
case 85:
{
		AddArgument(yyvsp[0].pExpr);
	 ;
    break;}
case 86:
{
		AddArgument(yyvsp[0].pExpr);
	 ;
    break;}
case 87:
{
		yyval.pExpr = yyvsp[0].pExpr;
	 ;
    break;}
case 88:
{
	 ;
    break;}
case 89:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 90:
{
		yyval.pExpr = UnaryExpression(eOpPOSTINCREMENT, yyvsp[-1].pExpr);
	 ;
    break;}
case 91:
{
		yyval.pExpr = UnaryExpression(eOpPOSTDECREMENT, yyvsp[-1].pExpr);
	 ;
    break;}
case 92:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 93:
{
	 ;
    break;}
case 94:
{
	 ;
    break;}
case 95:
{
	 ;
    break;}
case 96:
{
		yyval.pExpr = UnaryExpression(eOpPREINCREMENT, yyvsp[0].pExpr);
	 ;
    break;}
case 97:
{
		yyval.pExpr = UnaryExpression(eOpPREDECREMENT, yyvsp[0].pExpr);
	 ;
    break;}
case 98:
{
		yyval.pExpr = UnaryExpression(eOpUNARYPLUS, yyvsp[0].pExpr);
	 ;
    break;}
case 99:
{
		yyval.pExpr = UnaryExpression(eOpUNARYMINUS, yyvsp[0].pExpr);
	 ;
    break;}
case 100:
{
		yyval.pExpr = UnaryExpression(eOpONES_COMPLIMENT, yyvsp[0].pExpr);
	 ;
    break;}
case 101:
{
		yyval.pExpr = UnaryExpression(eOpLOGICAL_NOT, yyvsp[0].pExpr);
	 ;
    break;}
case 102:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 103:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpMULTIPLY, yyvsp[0].pExpr);
	 ;
    break;}
case 104:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpDIV, yyvsp[0].pExpr);
	 ;
    break;}
case 105:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpMOD, yyvsp[0].pExpr);
	 ;
    break;}
case 106:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 107:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpPLUS, yyvsp[0].pExpr);
	 ;
    break;}
case 108:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpMINUS, yyvsp[0].pExpr);
	 ;
    break;}
case 109:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 110:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpBITWISE_SHIFT_LEFT, yyvsp[0].pExpr);
	 ;
    break;}
case 111:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpBITWISE_SHIFT_RIGHT, yyvsp[0].pExpr);
	 ;
    break;}
case 112:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpBITWISE_SHIFT_RIGHT_ZERO_FILL, yyvsp[0].pExpr);
	 ;
    break;}
case 113:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 114:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpLESS_THAN, yyvsp[0].pExpr);
	 ;
    break;}
case 115:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpGREATER_THAN, yyvsp[0].pExpr);
	 ;
    break;}
case 116:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpLS_EQUAL, yyvsp[0].pExpr);
	 ;
    break;}
case 117:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpGT_EQUAL, yyvsp[0].pExpr);
	 ;
    break;}
case 118:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 119:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpEQUALS, yyvsp[0].pExpr);
	 ;
    break;}
case 120:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpNOT_EQUAL, yyvsp[0].pExpr);
	 ;
    break;}
case 121:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 122:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpBITWISE_AND, yyvsp[0].pExpr);
	 ;
    break;}
case 123:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 124:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpBITWISE_EXCLUSIVE_OR, yyvsp[0].pExpr);
	 ;
    break;}
case 125:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 126:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpBITWISE_OR, yyvsp[0].pExpr);
	 ;
    break;}
case 127:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 128:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpLOGICAL_AND, yyvsp[0].pExpr);
	 ;
    break;}
case 129:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 130:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpLOGICAL_OR, yyvsp[0].pExpr);
	 ;
    break;}
case 131:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 133:
{
		yyval.pExpr = yyvsp[0].pExpr;
	;
    break;}
case 134:
{
		yyval.pExpr = AssignmentExpression(yyvsp[-2].pExpr, yyvsp[-1].op, yyvsp[0].pExpr);
	;
    break;}
case 135:
{
		yyval.op = eOpASSIGNMENT;
	 ;
    break;}
case 136:
{
		yyval.op = eOpMULTIPLY_EQUALS;
	 ;
    break;}
case 137:
{
		yyval.op = eOpDIV_EQUALS;
	 ;
    break;}
case 138:
{
		yyval.op = eOpMOD_EQUALS;
	 ;
    break;}
case 139:
{
		yyval.op = eOpPLUS_EQUALS;
	 ;
    break;}
case 140:
{
		yyval.op = eOpMINUS_EQUALS;
	 ;
    break;}
case 141:
{
		yyval.op = eOpBITWISE_SHIFT_LEFT_EQUALS;
	 ;
    break;}
case 142:
{
		yyval.op = eOpBITWISE_SHIFT_RIGHT_EQUALS;
	 ;
    break;}
case 143:
{
		yyval.op = eOpBITWISE_SHIFT_RIGHT_ZERO_FILL_EQUALS;
	 ;
    break;}
case 144:
{
		yyval.op = eOpBITWISE_AND_EQUALS;
	 ;
    break;}
case 145:
{
		yyval.op = eOpBITWISE_EXCLUSIVE_OR_EQUALS;
	 ;
    break;}
case 146:
{
		yyval.op = eOpBITWISE_OR_EQUALS;
	 ;
    break;}
case 147:
{
		yyval.pExpr = yyvsp[0].pExpr;
	 ;
    break;}
case 148:
{
		yyval.pExpr = BinaryExpression(yyvsp[-2].pExpr, eOpCOMMA, yyvsp[0].pExpr);
	 ;
    break;}
case 149:
{
		yyval.pExpr = yyvsp[0].pExpr;
	 ;
    break;}
case 150:
{
		yyval.pExpr = NULL;
	 ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}

 



 
