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

#ifndef YY_YY_COACH_LANG_PARSER_HPP_INCLUDED
# define YY_YY_COACH_LANG_PARSER_HPP_INCLUDED
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
    RCSS_CLANG_INT = 258,
    RCSS_CLANG_REAL = 259,
    RCSS_CLANG_STR = 260,
    RCSS_CLANG_VAR = 261,
    RCSS_CLANG_LP = 262,
    RCSS_CLANG_RP = 263,
    RCSS_CLANG_LB = 264,
    RCSS_CLANG_RB = 265,
    RCSS_CLANG_SAY = 266,
    RCSS_CLANG_HEAR = 267,
    RCSS_CLANG_COACH_SIDE = 268,
    RCSS_CLANG_META = 269,
    RCSS_CLANG_FREEFORM = 270,
    RCSS_CLANG_UNSUPP = 271,
    RCSS_CLANG_INFO = 272,
    RCSS_CLANG_ADVICE = 273,
    RCSS_CLANG_DEFINE = 274,
    RCSS_CLANG_DEFINEC = 275,
    RCSS_CLANG_DEFINED = 276,
    RCSS_CLANG_DEFINER = 277,
    RCSS_CLANG_DEFINEA = 278,
    RCSS_CLANG_VER = 279,
    RCSS_CLANG_NULL_REGION = 280,
    RCSS_CLANG_QUAD = 281,
    RCSS_CLANG_ARC = 282,
    RCSS_CLANG_PT = 283,
    RCSS_CLANG_REG = 284,
    RCSS_CLANG_TRI = 285,
    RCSS_CLANG_REC = 286,
    RCSS_CLANG_BALL = 287,
    RCSS_CLANG_POS = 288,
    RCSS_CLANG_HOME = 289,
    RCSS_CLANG_BALLTO = 290,
    RCSS_CLANG_MARK = 291,
    RCSS_CLANG_MARK_LINE = 292,
    RCSS_CLANG_OFFLINE = 293,
    RCSS_CLANG_HTYPE = 294,
    RCSS_CLANG_BMOVE_TOKEN = 295,
    RCSS_CLANG_DO_DONT = 296,
    RCSS_CLANG_TEAM = 297,
    RCSS_CLANG_TRUE_KW = 298,
    RCSS_CLANG_FALSE_KW = 299,
    RCSS_CLANG_PPOS = 300,
    RCSS_CLANG_BPOS = 301,
    RCSS_CLANG_BOWNER = 302,
    RCSS_CLANG_AND = 303,
    RCSS_CLANG_OR = 304,
    RCSS_CLANG_NOT = 305,
    RCSS_CLANG_PLAY_MODE_KW = 306,
    RCSS_CLANG_PLAY_MODE = 307,
    RCSS_CLANG_CLEAR = 308,
    RCSS_CLANG_L = 309,
    RCSS_CLANG_G = 310,
    RCSS_CLANG_E = 311,
    RCSS_CLANG_NE = 312,
    RCSS_CLANG_LE = 313,
    RCSS_CLANG_GE = 314,
    RCSS_CLANG_PLUS = 315,
    RCSS_CLANG_MINUS = 316,
    RCSS_CLANG_MULT = 317,
    RCSS_CLANG_DIV = 318,
    RCSS_CLANG_TIME = 319,
    RCSS_CLANG_OPP_GOAL = 320,
    RCSS_CLANG_OUR_GOAL = 321,
    RCSS_CLANG_GOAL_DIFF = 322,
    RCSS_CLANG_UNUM = 323,
    RCSS_CLANG_PASS = 324,
    RCSS_CLANG_DRIBBLE = 325,
    RCSS_CLANG_SHOOT = 326,
    RCSS_CLANG_HOLD = 327,
    RCSS_CLANG_INTERCEPT = 328,
    RCSS_CLANG_TACKLE = 329,
    RCSS_CLANG_RULE = 330,
    RCSS_CLANG_ON_OFF = 331,
    RCSS_CLANG_ALL = 332,
    RCSS_CLANG_DEL = 333,
    RCSS_CLANG_DEFRULE = 334,
    RCSS_CLANG_MODEL = 335,
    RCSS_CLANG_DIREC = 336,
    RCSS_CLANG_ERROR = 337
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (rcss::clang::Parser::Param &param);

#endif /* !YY_YY_COACH_LANG_PARSER_HPP_INCLUDED  */
