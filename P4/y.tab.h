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
    PRINCIPAL = 258,
    PROCEDIMIENTO = 259,
    INICIOBLOQUE = 260,
    FINBLOQUE = 261,
    PYC = 262,
    COMA = 263,
    PAR_IZQUIERDO = 264,
    PAR_DERECHO = 265,
    INI_VARLOCAL = 266,
    FIN_VARLOCAL = 267,
    TIPO_BASICOS = 268,
    ASIGNACION = 269,
    CORCHETE_IZQ = 270,
    CORCHETE_DCHO = 271,
    SI = 272,
    SI_NO = 273,
    MIENTRAS = 274,
    ENTRADA = 275,
    SALIDA = 276,
    ELEGIR = 277,
    CASO = 278,
    SALIR = 279,
    VAL_LOGICOS = 280,
    ENTERO = 281,
    REAL = 282,
    CARACTER = 283,
    CADENA = 284,
    IDENT = 285,
    BASE = 286,
    DOSPUNTOS = 287,
    OPOR = 288,
    OPAND = 289,
    OPSOR = 290,
    OPIGUAL = 291,
    OPREL = 292,
    MAS_MENOS = 293,
    OPMAT = 294,
    OPMULDIV = 295,
    OPUNARIO = 296
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
