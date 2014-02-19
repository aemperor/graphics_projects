
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     HIERARCHY = 258,
     ROOT = 259,
     JOINT = 260,
     OFFSET = 261,
     CHANNELS = 262,
     MOTION = 263,
     FRAMES = 264,
     FRAME = 265,
     TIME = 266,
     END = 267,
     SITE = 268,
     LBRACK = 269,
     RBRACK = 270,
     COLON = 271,
     NEWLINE = 272,
     INT = 273,
     FLOAT = 274,
     ID = 275,
     PARAM = 276
   };
#endif
/* Tokens.  */
#define HIERARCHY 258
#define ROOT 259
#define JOINT 260
#define OFFSET 261
#define CHANNELS 262
#define MOTION 263
#define FRAMES 264
#define FRAME 265
#define TIME 266
#define END 267
#define SITE 268
#define LBRACK 269
#define RBRACK 270
#define COLON 271
#define NEWLINE 272
#define INT 273
#define FLOAT 274
#define ID 275
#define PARAM 276




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 28 "bvh.y"

	int ival;
	float fval;
	char * sval;



/* Line 1676 of yacc.c  */
#line 102 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


