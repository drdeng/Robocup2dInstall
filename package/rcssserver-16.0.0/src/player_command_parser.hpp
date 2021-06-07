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

#ifndef YY_YY_PLAYER_COMMAND_PARSER_HPP_INCLUDED
# define YY_YY_PLAYER_COMMAND_PARSER_HPP_INCLUDED
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
    RCSS_PCOM_INT = 258,
    RCSS_PCOM_REAL = 259,
    RCSS_PCOM_STR = 260,
    RCSS_PCOM_LP = 261,
    RCSS_PCOM_RP = 262,
    RCSS_PCOM_DASH = 263,
    RCSS_PCOM_TURN = 264,
    RCSS_PCOM_TURN_NECK = 265,
    RCSS_PCOM_KICK = 266,
    RCSS_PCOM_LONG_KICK = 267,
    RCSS_PCOM_CATCH = 268,
    RCSS_PCOM_SAY = 269,
    RCSS_PCOM_UNQ_SAY = 270,
    RCSS_PCOM_SENSE_BODY = 271,
    RCSS_PCOM_SCORE = 272,
    RCSS_PCOM_MOVE = 273,
    RCSS_PCOM_CHANGE_VIEW = 274,
    RCSS_PCOM_COMPRESSION = 275,
    RCSS_PCOM_BYE = 276,
    RCSS_PCOM_DONE = 277,
    RCSS_PCOM_POINTTO = 278,
    RCSS_PCOM_ATTENTIONTO = 279,
    RCSS_PCOM_TACKLE = 280,
    RCSS_PCOM_CLANG = 281,
    RCSS_PCOM_EAR = 282,
    RCSS_PCOM_SYNCH_SEE = 283,
    RCSS_PCOM_VIEW_WIDTH_NARROW = 284,
    RCSS_PCOM_VIEW_WIDTH_NORMAL = 285,
    RCSS_PCOM_VIEW_WIDTH_WIDE = 286,
    RCSS_PCOM_VIEW_QUALITY_LOW = 287,
    RCSS_PCOM_VIEW_QUALITY_HIGH = 288,
    RCSS_PCOM_ON = 289,
    RCSS_PCOM_OFF = 290,
    RCSS_PCOM_TRUE = 291,
    RCSS_PCOM_FALSE = 292,
    RCSS_PCOM_OUR = 293,
    RCSS_PCOM_OPP = 294,
    RCSS_PCOM_LEFT = 295,
    RCSS_PCOM_RIGHT = 296,
    RCSS_PCOM_EAR_PARTIAL = 297,
    RCSS_PCOM_EAR_COMPLETE = 298,
    RCSS_PCOM_CLANG_VERSION = 299,
    RCSS_PCOM_ERROR = 300
  };
#endif

/* Value type.  */



int yyparse (rcss::pcom::Parser::Param& param);

#endif /* !YY_YY_PLAYER_COMMAND_PARSER_HPP_INCLUDED  */
