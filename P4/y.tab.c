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
#line 1 "ejemplosyntax.y"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "y.tab.h"

// La siguiente declaracion permite que ’yyerror’ se pueda invocar desde el
// fuente de lex (prueba.l)
void yyerror(char * msg);

// La siguiente variable se usará para conocer el numero de la línea
// que se esta leyendo en cada momento. También es posible usar la variable
// ’yylineno’ que también nos muestra la línea actual. Para ello es necesario
// invocar a flex con la opción ’-l’ (compatibilidad con lex).

//############
//#define YYDEBUG 1
//############

#define YYERROR_VERBOSE

#define min(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define max(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

#define MAX_TAM_TS 500

extern int yylineno;

char msgError[256];
void mostrar_tabla();
// Esto elimina un Warning, no debería cambiar nada más.
int yylex();

/************************/
/* ESTRUCTURA DE LA TS */
/***********************/

// Tipo de entrada
typedef enum {
  marca,
  procedimiento,
  variable,
  parametroFormal
} TipoEntrada;

// Si TipoEntrada es función, variable, o parametro-formal; indica el tipo de dato
// No se usa en otro caso
typedef enum {
  entero,
  real,
  booleano,
  caracter,
  error // Si da un error con expresiones
} TipoDato;

typedef struct {
  TipoEntrada tipoEntrada;    // Tipo de entrada
  char* nombre;               // Nombre del identificador (no se usa con marca)
  TipoDato tipoDato;          // Tipo de dato
  int parametros;             // Nº de parámetros formales (sólo se usa con función)
  unsigned int dimensiones;
  int TamDimen1;
  int TamDimen2;
} EntradaTS;

// La Tabla de Símbolos
EntradaTS ts[MAX_TAM_TS];

char* entrada_a_string(entradaTS);

// Última entrada de la TS usada.
long int tope = -1;

// Tipo auxiliar para declaración de variables
TipoDato tipoTmp;

// Si entramos en un bloque de un subprograma
// Si es 0 es un bloque de un subprograma; en caso contrario no
int subProg = 0;

// Variables usadas para pasar argumentos a una función
#define MAX_ARGS 50
TipoDato argumentos_tipo_datos[MAX_ARGS];
int n_argumentos = 0;


typedef struct {
  int atributo;
  char* lexema;
  TipoDato dtipo;
  unsigned int dimensiones;
  int TamDimen1;
  int TamDimen2;
} Atributos;




char* tipoAString(TipoDato tipo_dato) {
  switch (tipo_dato) {
    case real:
      return "real";
    case entero:
      return "ent";
    case booleano:
      return "bool";
    case caracter:
      return "car";
    case error:
      return "error";
    default:
      fprintf(stderr, "Error en tipoAString(), no se conoce el tipo dato\n");
      return "error";
  }
}

/*TipoDato tipoArray(TipoDato tipo_dato) {
  switch (tipo_dato) {
    case arrayEntero:
      return entero;
    case arrayCaracter:
      return caracter;
    case arrayBooleano:
      return booleano;
    case arrayReal:
      return real;
    case error:
      return error;
    default:
      fprintf(stderr, "Error en tipoArray(), tipo no es array.\n");
      exit(EXIT_FAILURE);
  }
}*/

/*TipoDato aTipoArray(TipoDato td) {
  switch (td) {
    case entero:
      return arrayEntero;
    case real:
      return arrayReal;
    case caracter:
      return arrayCaracter;
    case booleano:
      return arrayBooleano;
    case error:
      return error;
    default:
      fprintf(stderr, "Error en aTipoArray(), tipo no es array.\n");
      exit(EXIT_FAILURE);
  }
}*/

int esArray(Atributos dato){
  return dato.dimensiones == 1 || dato.dimensiones == 2;
}

int esNumero(TipoDato tipo_dato){
  return tipo_dato == entero || tipo_dato == real;
}

void imprimir() {
  for (int i = 0; i <= tope; ++i) {
    printf("[%i]: ", i);
    switch(ts[i].tipoEntrada) {
      case variable:
        printf("Variable %s, tipo: %s\n", ts[i].nombre,
            tipoAString(ts[i].tipoDato));
        break;
      case procedimiento:
        printf("Procedimiento %s, nº parametros: %i\n", ts[i].nombre, ts[i].parametros);
        break;
      case marca:
        printf("Marca\n");
        break;
      case parametroFormal:
        printf("Parametro formal %s, tipo: %s\n", ts[i].nombre,
            tipoAString(ts[i].tipoDato));
        break;
      default:
        fprintf(stderr, "Error en imprimir(), no debería salir\n");
        exit(EXIT_FAILURE);
    }
    //printf("Dimensiones de %s son %i",ts[i].nombre,ts[i].dimensiones);
    //printf("Dimensiones de %s son %i",ts[i].nombre,ts[i].TamDimen1);
  }
}

void idRepetida(char* id) {
  // Miramos si id estaba declarado después de la última marca
  int repetida = 0;
  for (int i = tope; !repetida && ts[i].tipoEntrada != marca; --i) {
    if (ts[i].tipoEntrada != parametroFormal && !strcmp(ts[i].nombre, id)) {
      sprintf(msgError, "ERROR SEMÁNTICO: identificador %s ya declarado\n", id);
      yyerror(msgError);
      repetida = 1;
    }
  }
}

Atributos entradaAAtributos(EntradaTS ets){
  Atributos atr; 
  atr.dtipo = ets.tipoDato; 
  atr.dimensiones = ets.dimensiones;
  atr.TamDimen1 = ets.TamDimen1;
  atr.TamDimen2 = ets.TamDimen2;

  return atr;
}

void insertarEntrada(TipoEntrada te, char* nombre, TipoDato tipo_dato, int nParam,unsigned int dimensiones, int dim1, int dim2) {
  // Hacemos la entrada
  
  EntradaTS entrada = {
    te,
    strdup(nombre),
    tipo_dato,
    nParam,
    dimensiones,
    dim1,
    dim2
  };

  // Si la tabla está llena da error
  if (tope + 1 >= MAX_TAM_TS) {
    sprintf(msgError, "ERROR SEMÁNTICO: La tabla de símbolos está llena\n");
    yyerror(msgError);
  }
  // Aumentamos el tope
  ++tope;
  // Añadimos la nueva entrada
  ts[tope] = entrada;

  //mostrar_tabla();
  //imprimir();
}

// Busca una entrada en la TS con el id especificado en el ámbito del programa
// actual. Si no lo encuentra, devuelve -1. No gestiona errores!
int buscarEntrada(char* id) {
  int i = tope;
  while(i >= 0 && (ts[i].tipoEntrada == parametroFormal || strcmp(id, ts[i].nombre)))
    --i;

  if (i < 0) {
    sprintf(msgError, "ERROR SEMÁNTICO: identificador %s no declarado\n", id);
    yyerror(msgError);
  }
  return i;
}


void insertarMarca() {
  insertarEntrada(marca, "error", -1, -1,0,0,0);
  // Si es subprograma añadimos las variables al bloque
  if (subProg) {
    for (int i = tope - 1; ts[i].tipoEntrada != procedimiento; --i) {
      insertarEntrada(variable, ts[i].nombre, ts[i].tipoDato, -1,0,0,0);
    }
    subProg = 0;
  }
}

void vaciarEntradas() {
  while (ts[tope].tipoEntrada != marca)
    --tope;
  --tope;
}

void insertarVariable(char* id,unsigned int dimensiones, int dim1,int dim2) {
  // Comprobamos que no esté repetida la id
  idRepetida(id);
  insertarEntrada(variable, id, tipoTmp, -1,dimensiones,dim1,dim2);
}


void insertarProc( char* id) {
  // Comprobamos que el id no esté usado ya
  idRepetida(id);
  insertarEntrada(procedimiento, id, entero, 0,0,0,0);
}

void insertarParametro(TipoDato tipoDato, char* id) {
  // Comprobamos que no haya parámetros con nombres repetidos
  // Además guardamos el índice de la función
  int i;
  int parametroRepetido = 0;
  for (i = tope; !parametroRepetido && ts[i].tipoEntrada != procedimiento; --i) {
    if (!strcmp(ts[i].nombre, id)) {
      sprintf(msgError, "ERROR SEMÁNTICO: identificador del parámetro %s ya declarado\n", id);
      yyerror(msgError);
      parametroRepetido = 1;
    }
  }
  // Añadimos la entrada
  insertarEntrada(parametroFormal, id, tipoDato, -1,0,0,0);
  // Actualizamos el nº de parámetros de la función
  ++ts[i].parametros;
}

EntradaTS buscarID(char* id) {
  int i = buscarEntrada(id);

  if (i < 0){
    sprintf(msgError, "ERROR SEMANTICO: No se ha encontrado una entrada en la tabla con el id: %s\n", id);
    yyerror(msgError);
  }
    
  return ts[i];
}

void comprobarAsignacion(char* id, Atributos td) {
  int i = buscarEntrada(id);
  //printf("Dimensiones de td: %i,%i,%i",td.dimensiones,td.TamDimen1,td.TamDimen2);
  if (i >= 0) {
    if (ts[i].tipoEntrada != variable) {
      sprintf(msgError, "ERROR SEMÁNTICO: se intenta asignar a %s, y no es una variable\n", id);
      yyerror(msgError);
    } else {
      if (td.dtipo != error && td.dtipo != ts[i].tipoDato) {
        sprintf(msgError, "ERROR SEMÁNTICO: asignación incorrecta, %s es tipo %s y se obtuvo %s\n",
            id, tipoAString(ts[i].tipoDato),tipoAString(td.dtipo));
        yyerror(msgError);
      }
    }
    if(td.dimensiones == -1 && ts[i].dimensiones == 0 || td.dimensiones == 0 && ts[i].dimensiones == -1) {
          if (td.dimensiones == -1){
            td.dimensiones = 1;
          }
          if (ts[i].dimensiones == -1){
            ts[i].dimensiones = 1;
          }
    }else if((td.dimensiones == -2 && ts[i].dimensiones == 0 || td.dimensiones == 0 && ts[i].dimensiones == -2)) {
          if (td.dimensiones == -2){
            td.dimensiones = 2;
          }
          if (ts[i].dimensiones == -2){
            ts[i].dimensiones = 2;
          }
    }else{
        if (td.dimensiones == -1) td.dimensiones = 1;
      if (ts[i].dimensiones == -1) ts[i].dimensiones = 1;
      if (td.dimensiones == -2) td.dimensiones = 2;
      if (ts[i].dimensiones == -2) ts[i].dimensiones = 2;

        if(td.dimensiones != ts[i].dimensiones ){
          sprintf(msgError, "ERROR SEMÁNTICO: asignación incorrecta, dimensiones diferentes: %i,%i \n",
                ts[i].dimensiones,td.dimensiones);
            yyerror(msgError);
        }else if(td.dimensiones == 1){
          if(td.TamDimen1 != ts[i].TamDimen1){
            sprintf(msgError, "ERROR SEMÁNTICO: asignación incorrecta, dimensiones de vector diferentes: %i,%i \n",
                ts[i].TamDimen1,td.TamDimen1);
            yyerror(msgError);
          }
        }else if(td.dimensiones == 2){
          if(td.TamDimen1 != ts[i].TamDimen1 || td.TamDimen2 != ts[i].TamDimen2 ){
            sprintf(msgError, "ERROR SEMANTICO: asignación incorrecta,dimensiones de las matrices diferentes: (%i, %i),(%i,%i)\n",
                 ts[i].TamDimen1,ts[i].TamDimen2,td.TamDimen1,td.TamDimen2);
            yyerror(msgError);
          }

        }

    

    }
  }
}

void expresionBooleana(TipoDato td) {
  if (td != error && td != booleano) {
    sprintf(msgError, "ERROR SEMÁNTICO: condición no es de tipo booleano, se tiene tipo %s",
        tipoAString(td));
    yyerror(msgError);
  }
}

/*void sentenciaArray(TipoDato td, char* sentencia) {
  if (td != error && !esArray(td)) {
    sprintf(msgError, "ERROR SEMÁNTICO: sentencia %s no aplicable al tipo %s\n",
        sentencia, tipoAString(td));
    yyerror(msgError);
  }
}*/

/* TipoDato mismoTipoArray(TipoDato t1, TipoDato t2) {

  if (t1 == error || t2 == error)
    return error;

  if (t1 != t2) {
    sprintf(msgError, "ERROR SEMÁNTICO: array con dos tipos distintos %s y %s\n",
        tipoAString(t1), tipoAString(t2));
    yyerror(msgError);
    return error;
  }
  return t1;
}*/

TipoDato masMenos(int atr, TipoDato td) {
  if (td == error)
    return error;

  char* operador = atr ? "-" : "+";
  if (!esNumero(td)) {
    sprintf(msgError, "ERROR SEMÁNTICO: operador %s no aplicable al tipo %s\n",
        operador, tipoAString(td));
    yyerror(msgError);
    return error;
  }
  return td;
}

TipoDato excl(TipoDato td) {
  if (td == error)
    return error;
  if (td != booleano) {
    sprintf(msgError, "ERROR SEMÁNTICO: operador ! no aplicable al tipo %s\n",
        tipoAString(td));
    yyerror(msgError);
    return error;
  }
  return booleano;
}



TipoDato andLog(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (td1 != booleano || td2 != booleano) {
    sprintf(msgError, "ERROR SEMÁNTICO: operador && no aplicable a los tipos %s, %s\n",
        tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

TipoDato orLog(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (td1 != booleano || td2 != booleano) {
    sprintf(msgError, "ERROR SEMÁNTICO: operador || no aplicable a los tipos %s, %s\n",
        tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

TipoDato xorLog(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (td1 != booleano || td2 != booleano) {
    sprintf(msgError, "ERROR SEMÁNTICO: operador ^ no aplicable a los tipos %s, %s\n",
        tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

TipoDato eqn(TipoDato td1, int atr, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  char* operador = atr ? "!=" : "==";
  if (td1 != td2) {
    sprintf(msgError, "ERROR SEMÁNTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

// Comprueba el tipo de la operación binaria realizada. En caso de error, lo
// gestiona. En caso contrario, devuelve el tipo tras la operación binaria.
// IMPORTANTE: Se asume que op1 esta asociado al valor 1 del atributo (atr)
// mientras que op2 está asociado al valor 0.
// IMPORTANE: Se asume que el op1 es simétrico y que el op2 no es simétrico y
// unicamente funciona de la forma "T op2 T" o bien "list_of T op2 T".
TipoDato op2Binario(Atributos td1, int atr, Atributos td2, char* op1, char* op2) {
  if (td1.dtipo == error || td2.dtipo == error)
    return error;
  
  if(td1.dimensiones == -1 && td2.dimensiones == 0 || td1.dimensiones == 0 && td2.dimensiones == -1) {
          if (td1.dimensiones == -1){
            td1.dimensiones = 1;
          }
          if (td2.dimensiones == -1){
            td1.dimensiones = 1;
          }
    }else if((td1.dimensiones == -2 && td2.dimensiones == 0 || td1.dimensiones == 0 && td2.dimensiones == -2)) {
          if(td2.dimensiones == -2){
            td2.dimensiones = 2;
          }
          if(td1.dimensiones == -2){
            td1.dimensiones = 2;
          }
  } else {
      if (td1.dimensiones == -1) td1.dimensiones = 1;
      if (td2.dimensiones == -1) td2.dimensiones = 1;
      if (td1.dimensiones == -2) td1.dimensiones = 2;
      if (td2.dimensiones == -2) td2.dimensiones = 2;

      if( td1.dimensiones != td2.dimensiones){
        sprintf(msgError, "ERROR SEMANTICO: Dimensiones diferentes: %i, %i\n",
                 td1.dimensiones,td2.dimensiones);
            yyerror(msgError);
        return error;
      }
      else{
        if(td1.dimensiones == 1){
          if(td1.TamDimen1 != td2.TamDimen1){
             sprintf(msgError, "ERROR SEMANTICO: Dimensiones de los vectores diferentes: %i, %i\n",
                 td1.TamDimen1,td2.TamDimen1);
            yyerror(msgError);
            return error;
          }
        }else if(td1.dimensiones == 2){
          if(td1.TamDimen1 != td2.TamDimen1 || td1.TamDimen2 != td2.TamDimen2){
             sprintf(msgError, "ERROR SEMANTICO: Dimensiones de las matrices diferentes: (%i, %i),(%i,%i)\n",
                 td1.TamDimen1,td1.TamDimen2,td2.TamDimen1,td2.TamDimen2);
            yyerror(msgError);
            return error;
          }
        }
    }
  }
  if(td1.dimensiones == 0 && td2.dimensiones == 0){
      char* operador = atr ? op1 : op2;
      /*int l1 = esArray(td1);
      int l2 = esArray(td2);
      TipoDato tipo1 = l1 ? tipoArray(td1.dtipo) : td1.dtipo;
      TipoDato tipo2 = l2 ? tipoArray(td2.dtipo) : td2.dtipo;*/

      /*int op_error = (l1 && l2) ||
                  (td1.dtipo != td2.dtipo) ||
                  !esNumero(td1.dtipo);*/

      int op_error = (td1.dtipo != td2.dtipo) ||
                  !esNumero(td1.dtipo);

      TipoDato resultado = td1.dtipo;

      /*if (!op_error && (l1 || l2) ) {
        // Llegado a este punto hay exactamente un array. Vemos si el tipo de
        // array encaja con el tipo del otro atributo:
        if ( (operador == op2) || l1 ) {
          resultado = l1 ? td1.dtipo : td2.dtipo;
        } else {
          op_error = 1;
        }
      }*/

      if (op_error) {
        sprintf(msgError, "ERROR SEMÁNTICO: operador %s no aplicable a los tipos %s, %s\n",
            operador, tipoAString(td1.dtipo), tipoAString(td2.dtipo));
        yyerror(msgError);
        return error;
      }

      return resultado;
  }else{
    return error;
  }

}

TipoDato addSub(Atributos td1, int atr, Atributos td2) {
  return op2Binario(td1, atr, td2, "-", "+");
}

TipoDato porDiv(Atributos td1, int atr, Atributos td2) {
 
  return op2Binario(td1, atr, td2, "/", "*") || op2Binario(td1, atr, td2, "/", "%") || op2Binario(td1, atr, td2, "%", "*");
}

TipoDato porPor(Atributos td1, Atributos td2) {
  if (td1.dtipo == error || td2.dtipo == error)
    return error;

  if (td1.dtipo != td2.dtipo) {
    sprintf(msgError, "ERROR SEMÁNTICO: operador ** no aplicable a los tipos %s, %s\n",
        tipoAString(td1.dtipo), tipoAString(td2.dtipo));
    yyerror(msgError);
    return error;
  }
  /*printf("Dimensiones de td1 %s %i \n",td1.dimensiones );
  printf("Es Array : %i\n", esArray(td1));*/

  if (td1.dimensiones == -1) td1.dimensiones = 1;
  if (td2.dimensiones == -1) td2.dimensiones = 1;
  if (td1.dimensiones == -2) td1.dimensiones = 2;
  if (td2.dimensiones == -2) td2.dimensiones = 2;

  if(!esArray(td1)){
    sprintf(msgError, "ERROR SEMÁNTICO: operador ** no aplicable por su dimension %i\n",
        td1.dimensiones);
    yyerror(msgError);
    return error;
  }
  
  if( td1.dimensiones == 1){
    if(td1.TamDimen1 != td2.TamDimen1){
      sprintf(msgError, "ERROR SEMÁNTICO: operador ** no aplicable por ser diferentes dimensiones %i, %i\n",
          td1.TamDimen1, td2.TamDimen1);
      yyerror(msgError);
      return error;
    }
  }

  if(td1.dimensiones == 2){
    if(td1.TamDimen2 != td2.TamDimen1){
      sprintf(msgError, "ERROR SEMÁNTICO: operador ** no aplicable por ser diferentes dimensiones %i, %i\n",
          td1.TamDimen2, td2.TamDimen1);
      yyerror(msgError);
      return error;
    }
  }


  return td1.dtipo;
}

/*TipoDato borrArray(Atributos td1, int atr, Atributos td2) {
  if (td1.dtipo == error || td2.dtipo == error)
    return error;

  char* operador = atr ? "%" : "--";
  if (!esArray(td1) || td2.dtipo != entero) {
    sprintf(msgError, "ERROR SEMÁNTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1.dtipo), tipoAString(td2.dtipo));
    yyerror(msgError);
    return error;
  }

  return td1;
}*/

TipoDato rel(TipoDato td1, int atr, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  char* operador;
  switch (atr) {
    case 0:
      operador = "<";
      break;
    case 1:
      operador = ">";
      break;
    case 2:
      operador = "<=";
      break;
    case 3:
      operador = ">=";
      break;
  }

  if (td1 != td2 || !esNumero(td1) || !esNumero(td2)) {
    sprintf(msgError, "ERROR SEMÁNTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

void comprobarProcedimiento(char* id) {
  int iProc = buscarEntrada(id);

  if (iProc < 0){
    sprintf(msgError, "ERROR SEMANTICO: No se ha declarado el procedimiento %s \n", id);
    yyerror(msgError);
     // return error;
  }

  if (ts[iProc].tipoEntrada != procedimiento) {
    sprintf(msgError, "ERROR SEMÁNTICO: %s no es un procedimiento y no puede ser llamado\n", id);
    yyerror(msgError);
    // return error;
  }

  int n_argumentos_esperados = ts[iProc].parametros;

  if ( n_argumentos != n_argumentos_esperados ) {
    sprintf(msgError, "ERROR SEMÁNTICO: número de argumentos errónea al llamar al procedimiento %s. Esperados: %d, encontrados: %d\n",
        id, n_argumentos_esperados, n_argumentos);
    yyerror(msgError);
  }

 

  n_argumentos = min(n_argumentos, n_argumentos_esperados);

  for (int i = 0; i < n_argumentos; i++){
    TipoDato tipoEsperado = ts[iProc + i + 1].tipoDato;
    TipoDato tipoObtenido = argumentos_tipo_datos[i];
    if (tipoObtenido != error && tipoEsperado != tipoObtenido){
      sprintf(msgError, "ERROR SEMÁNTICO: argumento número %d de tipo erróneo al llamar a la función %s. Esperado: %s, encontrado: %s\n",
          i, id, tipoAString(tipoEsperado), tipoAString(tipoObtenido));
      yyerror(msgError);
    }
  }

  // De esta forma mostramos el máximo número de errores posibles.

  // Borramos los argumentos recibidos.
  n_argumentos = 0;

  //return ts[iProc].tipoDato;

}

#define YYSTYPE Atributos


#line 800 "y.tab.c"

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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   424

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  205

#define YYUNDEFTOK  2
#define YYMAXUTOK   296


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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   778,   778,   780,   780,   786,   787,   789,   789,   789,
     792,   792,   795,   796,   798,   798,   798,   800,   801,   803,
     808,   813,   814,   816,   817,   818,   819,   820,   821,   822,
     823,   826,   826,   828,   829,   832,   834,   836,   838,   839,
     841,   842,   843,   844,   845,   847,   847,   849,   850,   851,
     852,   854,   856,   857,   859,   864,   871,   872,   873,   876,
     877,   878,   879,   880,   881,   882,   883,   884,   885,   886,
     887,   888,   889,   890,   894,   895,   896,   897,   899,   900,
     902,   904,   906,   908,   911,   914,   918,   921,   925,   928,
     932,   935,   939,   943,   947,   951,   957,   961,   966,   970,
     975,   979,   984,   988,  1050,  1053,  1056
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PRINCIPAL", "PROCEDIMIENTO",
  "INICIOBLOQUE", "FINBLOQUE", "PYC", "COMA", "PAR_IZQUIERDO",
  "PAR_DERECHO", "INI_VARLOCAL", "FIN_VARLOCAL", "TIPO_BASICOS",
  "ASIGNACION", "CORCHETE_IZQ", "CORCHETE_DCHO", "SI", "SI_NO", "MIENTRAS",
  "ENTRADA", "SALIDA", "ELEGIR", "CASO", "SALIR", "VAL_LOGICOS", "ENTERO",
  "REAL", "CARACTER", "CADENA", "IDENT", "BASE", "DOSPUNTOS", "OPOR",
  "OPAND", "OPSOR", "OPIGUAL", "OPREL", "MAS_MENOS", "OPMAT", "OPMULDIV",
  "OPUNARIO", "$accept", "programa", "bloque", "$@1", "Declar_de_subprogs",
  "Declar_subprog", "$@2", "$@3", "Declar_de_variables_locales",
  "variables_locales", "cuerpo_declar_variables", "$@4",
  "lista_identificadores", "Tipo_vector", "Tipo_matriz", "Sentencias",
  "Sentencia", "sentencia_asignacion", "sentencia_if", "sentencia_while",
  "sentencia_entrada", "sentencia_salida", "lista_entrada",
  "lista_expresiones_o_cadena", "sentencia_case_switch", "Casos",
  "Bloque_de_caso", "sentencia_proc", "argumentos", "lista_argumentos",
  "lista_expresiones", "expresion", "constante", "vector", "lista_enteros",
  "lista_caracteres", "lista_booleanos", "lista_reales", "matriz",
  "matriz_enteros", "matriz_caracteres", "matriz_booleanos",
  "matriz_reales", "identificador", YY_NULLPTR
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
     295,   296
};
# endif

#define YYPACT_NINF (-137)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-59)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      54,    18,    95,   107,  -137,   128,  -137,  -137,   130,     5,
    -137,  -137,  -137,    11,  -137,   345,    98,  -137,  -137,  -137,
     104,   147,   150,    98,   117,   207,    88,  -137,  -137,  -137,
    -137,   351,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
     146,   156,     8,  -137,  -137,   207,   207,    60,  -137,  -137,
     164,   207,    98,    79,  -137,  -137,  -137,  -137,  -137,   207,
     207,    92,   377,  -137,  -137,  -137,  -137,   216,   168,   148,
    -137,  -137,   207,  -137,    98,   171,   265,   296,  -137,    98,
      79,    35,   119,   123,   159,   305,  -137,  -137,  -137,  -137,
    -137,     3,    12,    24,    29,  -137,  -137,  -137,   198,   207,
     207,   207,   207,   207,   207,   207,   207,   -10,   172,   176,
     377,    34,   224,  -137,    21,   379,   379,  -137,    36,    50,
      55,    64,  -137,   174,  -137,   175,  -137,   182,  -137,   185,
    -137,   160,  -137,   163,  -137,   162,  -137,   177,  -137,  -137,
     377,   384,   288,   204,   114,    46,    81,   161,  -137,    49,
     118,     2,  -137,   195,   207,   179,  -137,  -137,   158,   377,
     192,  -137,  -137,  -137,  -137,  -137,   188,   187,   193,   190,
    -137,  -137,  -137,  -137,   197,   379,   291,  -137,  -137,  -137,
     377,   203,   207,  -137,   379,    65,    71,    75,    82,   379,
     373,  -137,   377,   128,  -137,  -137,  -137,  -137,  -137,   264,
     223,  -137,   231,  -137,  -137
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     3,     2,    11,     0,
       6,    16,    14,     0,    13,     0,     0,    10,    12,    32,
       0,     0,     0,     0,     0,     0,   104,    23,     5,   105,
     106,     0,    22,    24,    25,    26,    27,    28,    30,    29,
       0,   104,     0,    18,     7,     0,     0,     0,    39,    73,
       0,     0,     0,     0,    76,    74,    75,    77,    43,     0,
       0,     0,    42,    71,    78,    79,    70,     0,     0,     0,
       4,    21,     0,    15,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,     0,     0,    72,    89,    85,    91,
      87,     0,     0,     0,     0,    61,    60,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      55,     0,     0,    17,     0,     0,     0,    38,     0,     0,
       0,     0,    92,     0,    93,     0,    94,     0,    95,     0,
      59,     0,    80,     0,    81,     0,    82,     0,    83,    41,
      40,    66,    65,    67,    63,    64,    69,    68,    62,     0,
       0,     0,    46,     0,     0,     0,    19,    31,     0,    57,
      33,    35,    97,    99,   101,   103,     0,     0,     0,     0,
      84,    86,    88,    90,     0,     0,     0,    44,    45,    51,
      54,     0,     0,     8,     0,     0,     0,     0,     0,     0,
       0,    20,    56,     0,    34,    96,    98,   100,   102,     0,
       0,     9,     0,    49,    47
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,  -137,    -5,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
     233,  -137,  -137,  -137,  -137,  -136,   -27,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,  -137,    96,  -137,  -137,  -137,
    -137,    10,   122,  -137,   -70,   -73,   -75,   -77,  -137,  -137,
    -137,  -137,  -137,   -14
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    27,     8,    15,    28,    75,   193,    10,    13,
      14,    16,    42,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    47,    61,    38,   151,   152,    39,   108,   109,
     158,    62,    63,    64,    91,    92,    93,    94,    65,    81,
      82,    83,    84,    66
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       7,    40,    43,   121,    71,   120,    11,   119,   177,    48,
     118,   131,    11,   149,   176,    73,    74,    40,    12,   132,
     133,   150,    49,    17,    12,   149,    50,     3,   134,   -58,
      51,   -58,   135,   150,    52,    67,    53,   137,    86,   190,
     136,   122,   155,   123,   131,   138,    54,    55,    56,    57,
     156,    41,   162,   199,    50,    76,    77,     1,   133,    59,
     113,    85,    60,   135,    53,   117,   163,    78,    79,    95,
      96,   164,   137,   131,    54,    55,    56,    57,   110,   133,
     165,   195,   112,   135,   104,   105,   106,   196,   160,   161,
     137,   197,   188,   187,   186,     4,   185,    68,   198,    97,
      98,    40,    40,    69,    87,    88,    89,    90,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     5,    49,    19,
     105,   106,    50,     6,   159,   124,    51,   125,    41,   126,
      52,   127,    53,     6,    44,    21,    40,    22,    23,    24,
      25,     9,    54,    55,    56,    57,    58,    41,    26,    71,
     175,   103,   104,   105,   106,    59,    45,   194,    60,    46,
      72,    40,    40,    71,   180,   128,   182,   129,   183,    49,
      40,    69,    71,    50,   111,    40,    40,    51,   -53,    80,
     114,    52,   153,    53,   154,    40,   170,   172,   201,   166,
     167,   171,   192,    54,    55,    56,    57,   168,    41,    49,
     169,   106,   179,    50,   173,   181,    59,    51,    49,    60,
     184,    52,    50,    53,    88,    90,    51,    89,    87,   191,
      52,   107,    53,    54,    55,    56,    57,   139,    41,   189,
     203,   157,    54,    55,    56,    57,    59,    41,   204,    60,
     102,   103,   104,   105,   106,    59,    18,   178,    60,    99,
     100,   101,   102,   103,   104,   105,   106,    99,   100,   101,
     102,   103,   104,   105,   106,    19,     0,     0,     0,     6,
     -48,   174,     0,     0,     0,   115,     0,     0,     0,     0,
       0,    21,     0,    22,    23,    24,    25,   -48,   202,     0,
       0,     0,    19,     0,    26,   -48,     6,   -50,    99,   100,
     101,   102,   103,   104,   105,   106,   116,     0,    21,     0,
      22,    23,    24,    25,   -50,   130,     0,     0,     0,     0,
       0,    26,   -50,   101,   102,   103,   104,   105,   106,    99,
     100,   101,   102,   103,   104,   105,   106,     0,    99,   100,
     101,   102,   103,   104,   105,   106,    19,     0,     0,    20,
       6,     0,    19,     0,     0,     0,     6,    70,     0,     0,
       0,     0,    21,     0,    22,    23,    24,    25,    21,     0,
      22,    23,    24,    25,    19,    26,     0,     0,     6,     0,
      19,    26,     0,     0,     6,     0,     0,     0,     0,     0,
      21,     0,    22,    23,    24,    25,    21,   200,    22,    23,
      24,    25,     0,    26,     0,     0,     0,     0,     0,    26,
      99,   100,   101,   102,   103,   104,   105,   106,   100,   101,
     102,   103,   104,   105,   106
};

static const yytype_int16 yycheck[] =
{
       5,    15,    16,    80,    31,    80,     1,    80,     6,    23,
      80,     8,     1,    23,   150,     7,     8,    31,    13,    16,
       8,    31,     1,    12,    13,    23,     5,     9,    16,     8,
       9,    10,     8,    31,    13,    25,    15,     8,    52,   175,
      16,     6,     8,     8,     8,    16,    25,    26,    27,    28,
      16,    30,    16,   189,     5,    45,    46,     3,     8,    38,
      74,    51,    41,     8,    15,    79,    16,     7,     8,    59,
      60,    16,     8,     8,    25,    26,    27,    28,    68,     8,
      16,    16,    72,     8,    38,    39,    40,    16,   115,   116,
       8,    16,   169,   168,   167,     0,   166,     9,    16,     7,
       8,   115,   116,    15,    25,    26,    27,    28,    98,    99,
     100,   101,   102,   103,   104,   105,   106,    10,     1,     1,
      39,    40,     5,     5,   114,     6,     9,     8,    30,     6,
      13,     8,    15,     5,    30,    17,   150,    19,    20,    21,
      22,    11,    25,    26,    27,    28,    29,    30,    30,   176,
      32,    37,    38,    39,    40,    38,     9,   184,    41,     9,
      14,   175,   176,   190,   154,     6,     8,     8,    10,     1,
     184,    15,   199,     5,    26,   189,   190,     9,    10,    15,
       9,    13,    10,    15,     8,   199,    26,    25,   193,    15,
      15,    28,   182,    25,    26,    27,    28,    15,    30,     1,
      15,    40,     7,     5,    27,    26,    38,     9,     1,    41,
      18,    13,     5,    15,    26,    28,     9,    27,    25,    16,
      13,     5,    15,    25,    26,    27,    28,    29,    30,    32,
       7,     7,    25,    26,    27,    28,    38,    30,     7,    41,
      36,    37,    38,    39,    40,    38,    13,   151,    41,    33,
      34,    35,    36,    37,    38,    39,    40,    33,    34,    35,
      36,    37,    38,    39,    40,     1,    -1,    -1,    -1,     5,
       6,   149,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,     1,    -1,    30,    31,     5,     6,    33,    34,
      35,    36,    37,    38,    39,    40,    10,    -1,    17,    -1,
      19,    20,    21,    22,    23,    10,    -1,    -1,    -1,    -1,
      -1,    30,    31,    35,    36,    37,    38,    39,    40,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,     1,    -1,    -1,     4,
       5,    -1,     1,    -1,    -1,    -1,     5,     6,    -1,    -1,
      -1,    -1,    17,    -1,    19,    20,    21,    22,    17,    -1,
      19,    20,    21,    22,     1,    30,    -1,    -1,     5,    -1,
       1,    30,    -1,    -1,     5,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    19,    20,    21,    22,    17,    24,    19,    20,
      21,    22,    -1,    30,    -1,    -1,    -1,    -1,    -1,    30,
      33,    34,    35,    36,    37,    38,    39,    40,    34,    35,
      36,    37,    38,    39,    40
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    43,     9,     0,    10,     5,    44,    45,    11,
      50,     1,    13,    51,    52,    46,    53,    12,    52,     1,
       4,    17,    19,    20,    21,    22,    30,    44,    47,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    66,    69,
      85,    30,    54,    85,    30,     9,     9,    64,    85,     1,
       5,     9,    13,    15,    25,    26,    27,    28,    29,    38,
      41,    65,    73,    74,    75,    80,    85,    73,     9,    15,
       6,    58,    14,     7,     8,    48,    73,    73,     7,     8,
      15,    81,    82,    83,    84,    73,    85,    25,    26,    27,
      28,    76,    77,    78,    79,    73,    73,     7,     8,    33,
      34,    35,    36,    37,    38,    39,    40,     5,    70,    71,
      73,    26,    73,    85,     9,    10,    10,    85,    76,    77,
      78,    79,     6,     8,     6,     8,     6,     8,     6,     8,
      10,     8,    16,     8,    16,     8,    16,     8,    16,    29,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    23,
      31,    67,    68,    10,     8,     8,    16,     7,    72,    73,
      58,    58,    16,    16,    16,    16,    15,    15,    15,    15,
      26,    28,    25,    27,    74,    32,    57,     6,    68,     7,
      73,    26,     8,    10,    18,    76,    77,    78,    79,    32,
      57,    16,    73,    49,    58,    16,    16,    16,    16,    57,
      24,    44,    24,     7,     7
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    45,    44,    46,    46,    48,    49,    47,
      50,    50,    51,    51,    53,    52,    52,    54,    54,    55,
      56,    57,    57,    58,    58,    58,    58,    58,    58,    58,
      58,    59,    59,    60,    60,    61,    62,    63,    64,    64,
      65,    65,    65,    65,    66,    67,    67,    68,    68,    68,
      68,    69,    70,    70,    71,    71,    72,    72,    72,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    74,    74,    74,    74,    74,    74,
      75,    75,    75,    75,    76,    76,    77,    77,    78,    78,
      79,    79,    80,    80,    80,    80,    81,    81,    82,    82,
      83,    83,    84,    84,    85,    85,    85
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     0,     6,     2,     0,     0,     0,     8,
       3,     0,     2,     1,     0,     4,     1,     3,     1,     4,
       6,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     5,     7,     5,     3,     3,     3,     1,
       3,     3,     1,     1,     5,     2,     1,     6,     4,     5,
       2,     5,     1,     0,     3,     1,     3,     1,     0,     3,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     3,     3,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     1,     1,     1
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
  case 3:
#line 780 "ejemplosyntax.y"
                      { insertarMarca(); }
#line 2258 "y.tab.c"
    break;

  case 4:
#line 784 "ejemplosyntax.y"
              { vaciarEntradas(); }
#line 2264 "y.tab.c"
    break;

  case 7:
#line 789 "ejemplosyntax.y"
                                     { insertarProc(yyvsp[0].lexema); }
#line 2270 "y.tab.c"
    break;

  case 8:
#line 789 "ejemplosyntax.y"
                                                                                                              { subProg = 1; }
#line 2276 "y.tab.c"
    break;

  case 14:
#line 798 "ejemplosyntax.y"
                                       { tipoTmp = yyvsp[0].dtipo; }
#line 2282 "y.tab.c"
    break;

  case 17:
#line 800 "ejemplosyntax.y"
                                                                 { insertarVariable(yyvsp[0].lexema,yyvsp[0].dimensiones,yyvsp[0].TamDimen1,yyvsp[0].TamDimen2); }
#line 2288 "y.tab.c"
    break;

  case 18:
#line 801 "ejemplosyntax.y"
                            { insertarVariable(yyvsp[0].lexema,yyvsp[0].dimensiones,yyvsp[0].TamDimen1,yyvsp[0].TamDimen2); }
#line 2294 "y.tab.c"
    break;

  case 19:
#line 803 "ejemplosyntax.y"
                                                      { {yyval.lexema = yyvsp[-3].lexema; 
                                                              yyval.dimensiones=1;
                                                              yyval.TamDimen1=atoi(yyvsp[-1].lexema);
                                                              yyval.TamDimen2=0;}}
#line 2303 "y.tab.c"
    break;

  case 20:
#line 808 "ejemplosyntax.y"
                                                                  {yyval.lexema = yyvsp[-5].lexema;
                                                                          yyval.dimensiones=2;
                                                                          yyval.TamDimen1=atoi(yyvsp[-3].lexema);
                                                                          yyval.TamDimen2=atoi(yyvsp[-1].lexema);}
#line 2312 "y.tab.c"
    break;

  case 31:
#line 826 "ejemplosyntax.y"
                                                              {comprobarAsignacion(yyvsp[-3].lexema, yyvsp[-1]); }
#line 2318 "y.tab.c"
    break;

  case 33:
#line 828 "ejemplosyntax.y"
                                                                { expresionBooleana(yyvsp[-2].dtipo); }
#line 2324 "y.tab.c"
    break;

  case 34:
#line 829 "ejemplosyntax.y"
                                                                                 { expresionBooleana(yyvsp[-4].dtipo); }
#line 2330 "y.tab.c"
    break;

  case 35:
#line 832 "ejemplosyntax.y"
                                                                         { expresionBooleana(yyvsp[-2].dtipo); }
#line 2336 "y.tab.c"
    break;

  case 51:
#line 854 "ejemplosyntax.y"
                                                                 { comprobarProcedimiento(yyvsp[-4].lexema);}
#line 2342 "y.tab.c"
    break;

  case 54:
#line 859 "ejemplosyntax.y"
                                                   {
                  argumentos_tipo_datos[n_argumentos] = yyvsp[0].dtipo;
                  n_argumentos++;
                    
                  }
#line 2352 "y.tab.c"
    break;

  case 55:
#line 864 "ejemplosyntax.y"
                             {
                   argumentos_tipo_datos[n_argumentos] = yyvsp[0].dtipo;
                    n_argumentos++;
                   
                  }
#line 2362 "y.tab.c"
    break;

  case 59:
#line 876 "ejemplosyntax.y"
                                                { yyval.dtipo = yyvsp[-1].dtipo; }
#line 2368 "y.tab.c"
    break;

  case 60:
#line 877 "ejemplosyntax.y"
                                     { yyval.dtipo = excl(yyvsp[0].dtipo); }
#line 2374 "y.tab.c"
    break;

  case 61:
#line 878 "ejemplosyntax.y"
                                                     { yyval.dtipo = masMenos(yyvsp[-1].atributo, yyvsp[0].dtipo); }
#line 2380 "y.tab.c"
    break;

  case 62:
#line 879 "ejemplosyntax.y"
                                               { yyval.dtipo = porDiv(yyvsp[-2], yyvsp[-1].atributo, yyvsp[0]); }
#line 2386 "y.tab.c"
    break;

  case 63:
#line 880 "ejemplosyntax.y"
                                              { yyval.dtipo = eqn(yyvsp[-2].dtipo, yyvsp[-1].atributo, yyvsp[0].dtipo); }
#line 2392 "y.tab.c"
    break;

  case 64:
#line 881 "ejemplosyntax.y"
                                            { yyval.dtipo = rel(yyvsp[-2].dtipo, yyvsp[-1].atributo, yyvsp[0].dtipo); }
#line 2398 "y.tab.c"
    break;

  case 65:
#line 882 "ejemplosyntax.y"
                                            { yyval.dtipo = andLog(yyvsp[-2].dtipo, yyvsp[0].dtipo); }
#line 2404 "y.tab.c"
    break;

  case 66:
#line 883 "ejemplosyntax.y"
                                           { yyval.dtipo = orLog(yyvsp[-2].dtipo, yyvsp[0].dtipo); }
#line 2410 "y.tab.c"
    break;

  case 67:
#line 884 "ejemplosyntax.y"
                                             { yyval.dtipo = xorLog(yyvsp[-2].dtipo, yyvsp[0].dtipo); }
#line 2416 "y.tab.c"
    break;

  case 68:
#line 885 "ejemplosyntax.y"
                                            { yyval.dtipo = porPor(yyvsp[-2], yyvsp[0]); }
#line 2422 "y.tab.c"
    break;

  case 69:
#line 886 "ejemplosyntax.y"
                                                { yyval.dtipo = addSub(yyvsp[-2], yyvsp[-1].atributo, yyvsp[0]); }
#line 2428 "y.tab.c"
    break;

  case 70:
#line 887 "ejemplosyntax.y"
                                { yyval = entradaAAtributos(buscarID(yyvsp[0].lexema)); }
#line 2434 "y.tab.c"
    break;

  case 71:
#line 888 "ejemplosyntax.y"
                             { yyval.dtipo = yyvsp[0].dtipo; }
#line 2440 "y.tab.c"
    break;

  case 72:
#line 889 "ejemplosyntax.y"
                                             { insertarParametro(yyvsp[-1].dtipo, yyvsp[0].lexema); }
#line 2446 "y.tab.c"
    break;

  case 74:
#line 894 "ejemplosyntax.y"
                   { yyval.dtipo = yyvsp[0].dtipo; }
#line 2452 "y.tab.c"
    break;

  case 75:
#line 895 "ejemplosyntax.y"
               { yyval.dtipo = yyvsp[0].dtipo; }
#line 2458 "y.tab.c"
    break;

  case 76:
#line 896 "ejemplosyntax.y"
                      { yyval.dtipo = yyvsp[0].dtipo; }
#line 2464 "y.tab.c"
    break;

  case 77:
#line 897 "ejemplosyntax.y"
                   { yyval.dtipo = yyvsp[0].dtipo; }
#line 2470 "y.tab.c"
    break;

  case 78:
#line 899 "ejemplosyntax.y"
                 { yyval = yyvsp[0]; }
#line 2476 "y.tab.c"
    break;

  case 79:
#line 900 "ejemplosyntax.y"
                  { yyval = yyvsp[0]; }
#line 2482 "y.tab.c"
    break;

  case 80:
#line 902 "ejemplosyntax.y"
                                                  { yyval.dtipo = yyvsp[-1].dtipo;
                                                    if (yyvsp[-1].dimensiones==1){yyval.TamDimen2 = 0; yyval.TamDimen1 = yyvsp[-1].TamDimen1; yyval.dimensiones = 1;}}
#line 2489 "y.tab.c"
    break;

  case 81:
#line 904 "ejemplosyntax.y"
                                                      { yyval.dtipo = yyvsp[-1].dtipo;
                                                    if (yyvsp[-1].dimensiones==1){yyval.TamDimen2 = 0; yyval.TamDimen1 = yyvsp[-1].TamDimen1; yyval.dimensiones = 1; }}
#line 2496 "y.tab.c"
    break;

  case 82:
#line 906 "ejemplosyntax.y"
                                                     {yyval.dtipo = yyvsp[-1].dtipo;
                                                    if (yyvsp[-1].dimensiones==1){yyval.TamDimen2 = 0; yyval.TamDimen1 = yyvsp[-1].TamDimen1; yyval.dimensiones = 1; }}
#line 2503 "y.tab.c"
    break;

  case 83:
#line 908 "ejemplosyntax.y"
                                                  { yyval.dtipo = yyvsp[-1].dtipo;
                                                    if (yyvsp[-1].dimensiones==1){yyval.TamDimen2 = 0; yyval.TamDimen1 = yyvsp[-1].TamDimen1; yyval.dimensiones = 1; }}
#line 2510 "y.tab.c"
    break;

  case 84:
#line 911 "ejemplosyntax.y"
                                          {yyval.dtipo = yyvsp[0].dtipo;
                                           yyval.dimensiones = 1;
                                           yyval.TamDimen1 = yyvsp[-2].TamDimen1+1;}
#line 2518 "y.tab.c"
    break;

  case 85:
#line 914 "ejemplosyntax.y"
                     {yyval.dtipo = yyvsp[0].dtipo;
                      yyval.dimensiones = 1;
                      yyval.TamDimen1 = 1;}
#line 2526 "y.tab.c"
    break;

  case 86:
#line 918 "ejemplosyntax.y"
                                                  {yyval.dtipo = yyvsp[0].dtipo;
                                           yyval.dimensiones = 1;
                                           yyval.TamDimen1 = yyvsp[-2].TamDimen1+1;}
#line 2534 "y.tab.c"
    break;

  case 87:
#line 921 "ejemplosyntax.y"
                       {yyval.dtipo = yyvsp[0].dtipo;
                      yyval.dimensiones = 1;
                      yyval.TamDimen1 = 1;}
#line 2542 "y.tab.c"
    break;

  case 88:
#line 925 "ejemplosyntax.y"
                                                   {yyval.dtipo = yyvsp[0].dtipo;
                                           yyval.dimensiones = 1;
                                           yyval.TamDimen1 = yyvsp[-2].TamDimen1+1;}
#line 2550 "y.tab.c"
    break;

  case 89:
#line 928 "ejemplosyntax.y"
                          {yyval.dtipo = yyvsp[0].dtipo;
                      yyval.dimensiones = 1;
                      yyval.TamDimen1 = 1;}
#line 2558 "y.tab.c"
    break;

  case 90:
#line 932 "ejemplosyntax.y"
                                      {yyval.dtipo = yyvsp[0].dtipo;
                                           yyval.dimensiones = 1;
                                           yyval.TamDimen1 = yyvsp[-2].TamDimen1+1;}
#line 2566 "y.tab.c"
    break;

  case 91:
#line 935 "ejemplosyntax.y"
                   {yyval.dtipo = yyvsp[0].dtipo;
                      yyval.dimensiones = 1;
                      yyval.TamDimen1 = 1;}
#line 2574 "y.tab.c"
    break;

  case 92:
#line 939 "ejemplosyntax.y"
                                               { yyval.dtipo = yyvsp[-1].dtipo; 
                                                 yyval.dimensiones = yyvsp[-1].dimensiones;
                                                 yyval.TamDimen1 = yyvsp[-1].TamDimen1;
                                                 yyval.TamDimen2 = yyvsp[-1].TamDimen2;}
#line 2583 "y.tab.c"
    break;

  case 93:
#line 943 "ejemplosyntax.y"
                                                       { yyval.dtipo = yyvsp[-1].dtipo; 
                                                 yyval.dimensiones = yyvsp[-1].dimensiones;
                                                 yyval.TamDimen1 = yyvsp[-1].TamDimen1;
                                                 yyval.TamDimen2 = yyvsp[-1].TamDimen2;}
#line 2592 "y.tab.c"
    break;

  case 94:
#line 947 "ejemplosyntax.y"
                                                      { yyval.dtipo = yyvsp[-1].dtipo; 
                                                 yyval.dimensiones = yyvsp[-1].dimensiones;
                                                 yyval.TamDimen1 = yyvsp[-1].TamDimen1;
                                                 yyval.TamDimen2 = yyvsp[-1].TamDimen2;}
#line 2601 "y.tab.c"
    break;

  case 95:
#line 951 "ejemplosyntax.y"
                                                   { yyval.dtipo = yyvsp[-1].dtipo; 
                                                 yyval.dimensiones = yyvsp[-1].dimensiones;
                                                 yyval.TamDimen1 = yyvsp[-1].TamDimen1;
                                                 yyval.TamDimen2 = yyvsp[-1].TamDimen2;}
#line 2610 "y.tab.c"
    break;

  case 96:
#line 957 "ejemplosyntax.y"
                                                                              {yyval.TamDimen2 = yyvsp[-1].TamDimen1; 
                                                                               yyval.TamDimen1 = yyvsp[-4].TamDimen1+1;
                                                                               yyval.dtipo = yyvsp[-1].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 2619 "y.tab.c"
    break;

  case 97:
#line 961 "ejemplosyntax.y"
                                                             {yyval.TamDimen2 = yyvsp[0].TamDimen1; 
                                                                               yyval.TamDimen1 = 1;
                                                                               yyval.dtipo = yyvsp[0].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 2628 "y.tab.c"
    break;

  case 98:
#line 966 "ejemplosyntax.y"
                                                                                        {yyval.TamDimen2 = yyvsp[-1].TamDimen1; 
                                                                               yyval.TamDimen1 = yyvsp[-4].TamDimen1+1;
                                                                               yyval.dtipo = yyvsp[-1].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 2637 "y.tab.c"
    break;

  case 99:
#line 970 "ejemplosyntax.y"
                                                                {yyval.TamDimen2 = yyvsp[0].TamDimen1; 
                                                                               yyval.TamDimen1 = 1;
                                                                               yyval.dtipo = yyvsp[0].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 2646 "y.tab.c"
    break;

  case 100:
#line 975 "ejemplosyntax.y"
                                                                                     {yyval.TamDimen2 = yyvsp[-1].TamDimen1; 
                                                                               yyval.TamDimen1 = yyvsp[-4].TamDimen1+1;
                                                                               yyval.dtipo = yyvsp[-1].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 2655 "y.tab.c"
    break;

  case 101:
#line 979 "ejemplosyntax.y"
                                                               {yyval.TamDimen2 = yyvsp[0].TamDimen1; 
                                                                               yyval.TamDimen1 = 1;
                                                                               yyval.dtipo = yyvsp[0].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 2664 "y.tab.c"
    break;

  case 102:
#line 984 "ejemplosyntax.y"
                                                                            {yyval.TamDimen2 = yyvsp[-1].TamDimen1; 
                                                                               yyval.TamDimen1 = yyvsp[-4].TamDimen1+1;
                                                                               yyval.dtipo = yyvsp[-1].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 2673 "y.tab.c"
    break;

  case 103:
#line 988 "ejemplosyntax.y"
                                                            {yyval.TamDimen2 = yyvsp[0].TamDimen1; 
                                                                               yyval.TamDimen1 = 1;
                                                                               yyval.dtipo = yyvsp[0].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 2682 "y.tab.c"
    break;

  case 104:
#line 1050 "ejemplosyntax.y"
                      {yyval.lexema = yyvsp[0].lexema;
                        yyval.dimensiones=0; yyval.TamDimen1=0; yyval.TamDimen2=0;
                      }
#line 2690 "y.tab.c"
    break;

  case 105:
#line 1053 "ejemplosyntax.y"
                          { yyval.lexema = yyvsp[0].lexema; 
                            yyval.dimensiones=-1;
                            }
#line 2698 "y.tab.c"
    break;

  case 106:
#line 1056 "ejemplosyntax.y"
                          { yyval.lexema = yyvsp[0].lexema; 
                            yyval.dimensiones=-2;}
#line 2705 "y.tab.c"
    break;


#line 2709 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1060 "ejemplosyntax.y"


// Aqui incluimos el fichero generado por el ’lex’ que implementa la función
// ’yylex’

#ifdef DOSWINDOWS /* Variable de entorno que indica la plataforma */
#include "lexyy.c"
#else
#include "lex.yy.c"
#endif

// Se debe implementar la función yyerror. En este caso simplemente escribimos
// el mensaje de error en pantalla
void yyerror( char *msg ){
    fprintf(stderr, "Línea %d: %s\n", yylineno, msg) ;
}


void mostrar_tabla(){

  unsigned int entrada = 0;

  printf("-------------------TABLA-------------------\n");
  printf("entrada\tnombre\ttipoDato\tparametros\tdimensiones\tTamDimen1\tTamDimen2\n");

  while (entrada < tope){
    printf("%s\n",entrada_a_string(ts[entrada]));
    entrada++;
  }

  printf("\n\n");

}

char* entrada_a_string(EntradaTS es){

  char salida[2000];
  char formato[] = "%s \t%s \t%s \t%d \t%d \t%d \t%d";
  char* c1, *c2, *c3;
  unsigned int c4,c5,c6,c7;

  if (es.tipoEntrada == marca) c1 = "marca";
  else if (es.tipoEntrada == procedimiento) c1 = "procedimiento";
  else if (es.tipoEntrada == variable) c1 = "variable";
  else if (es.tipoEntrada == parametroFormal) c1 = "paramFormal";

  
  if (es.tipoDato == booleano) c3 = "booleano";
  else if (es.tipoDato == entero) c3 = "entero";
  else if (es.tipoDato == real) c3 = "real";
  else if (es.tipoDato == caracter) c3 = "caracter";
  c4 = es.parametros;
  c5 = es.dimensiones;
  c6 = es.TamDimen1;
  c7 = es.TamDimen2;
  sprintf(salida,formato,c1,es.nombre,c3,c4,c5,c6,c7);
  return strdup(salida);  
}
