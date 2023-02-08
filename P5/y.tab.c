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
FILE* yyout;
extern int yylineno;

char msgError[256];
void mostrar_tabla();
// Esto elimina un Warning, no debería cambiar nada más.
int yylex();

/************************/
/* ESTRUCTURA DE LA TS */
/***********************/

// Para bucles if
typedef struct {
  char* etiquetaEntrada;
  char* etiquetaSalida;
  char* etiquetaElse;
} DescriptorDeInstrControl;


// Tipo de entrada
// AÑADIDO P5. DESCRIPTOR
typedef enum {
  marca,
  procedimiento,
  variable,
  parametroFormal,
  descriptor
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
  // AÑADIDO P5. DESCRIPTOR DE CONTROL
  DescriptorDeInstrControl* descriptor;
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

// AÑADIDO GENERACION DE CODIGO INTERMEDIO
// REVISAR FUNCION PORQUE NOSOTROS NO TENEMOS OPERADORES @ ? #
TipoDato tipoOp(TipoDato td, char* op) {
  if (!strcmp(op, "+") || !strcmp(op, "-") || !strcmp(op, "*") || !strcmp(op, "/") ||
        !strcmp(op, "**") || !strcmp(op, "%"))
    return td;

  if (!strcmp(op, "!") || !strcmp(op, "&&") || !strcmp(op, "||") || !strcmp(op, ">") || !strcmp(op, "<") ||
        !strcmp(op, ">=") || !strcmp(op, "<=") || !strcmp(op, "!=") || !strcmp(op, "=="))
    return booleano;

  //if (!strcmp(op, "#"))
  //return entero;

  //if (!strcmp(op, "?") || !strcmp(op, "@"))
  //return tipoLista(td);
}

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

void insertarEntrada(TipoEntrada te, char* nombre, TipoDato tipo_dato, int nParam, unsigned int dimensiones, int dim1, int dim2, DescriptorDeInstrControl* descp) {
  // Hacemos la entrada
  
  EntradaTS entrada = {
    te,
    strdup(nombre),
    tipo_dato,
    nParam,
    dimensiones,
    dim1,
    dim2,
    // AÑADIDO P5. DESC
    descp
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

// AÑADIDO P5. DESC NULL
void insertarMarca() {
  insertarEntrada(marca, "error", -1, -1,0,0,0,NULL);
  // Si es subprograma añadimos las variables al bloque
  if (subProg) {
    for (int i = tope - 1; ts[i].tipoEntrada != procedimiento; --i) {
      insertarEntrada(variable, ts[i].nombre, ts[i].tipoDato, -1,0,0,0,NULL);
    }
    subProg = 0;
  }
}

void vaciarEntradas() {
  while (ts[tope].tipoEntrada != marca)
    --tope;
  --tope;
}

// AÑADIDO P5. DESC NULL
void insertarVariable(char* id,unsigned int dimensiones, int dim1,int dim2) {
  // Comprobamos que no esté repetida la id
  idRepetida(id);
  insertarEntrada(variable, id, tipoTmp, -1,dimensiones,dim1,dim2,NULL);
}

// AÑADIDO P5. DESC NULL
void insertarProc( char* id) {
  // Comprobamos que el id no esté usado ya
  idRepetida(id);
  insertarEntrada(procedimiento, id, entero, 0,0,0,0,NULL);
}

// AÑADIDO P5. REVISAR
void insertarDescriptor(char* etqEntrada, char* etqSalida, char* etqElse) {
  DescriptorDeInstrControl* descp = (DescriptorDeInstrControl*) malloc(sizeof(DescriptorDeInstrControl));
  descp -> etiquetaEntrada = strdup(etqEntrada);
  descp -> etiquetaSalida = strdup(etqSalida);
  descp -> etiquetaElse = strdup(etqElse);
  // AQUI MODIFICAN INSERTAR ENTRADA, AÑADEN DESCRIPTOR
  insertarEntrada(descriptor, "", -1, -1,0,0,0, descp);
}

// AÑADIDO P5. DESC NULL
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
  insertarEntrada(parametroFormal, id, tipoDato, -1,0,0,0,NULL);
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
}

/*

______________GENERACION DE CODIGO INTERMEDIO___________________

*/

int hayError = 0;
int deep = 0;
FILE * fMain;
FILE * fProc;

#define addTab() {for (int i = 0; i < deep - (yyout != fMain); ++i) fprintf(yyout,"\t");; }
#define gen(f_, ...) { if (!hayError) {addTab(); fprintf(yyout, f_, ##__VA_ARGS__); fflush(yyout);} }

char* temporal() {
  static int indice = 1;
  char* temp = malloc(sizeof(char)*10);
  sprintf(temp,"temp%i",indice++);
  return temp;
}

char* etiqueta() {
  static int indice = 1;
  char* etiqueta = malloc(sizeof(char)*14);
  sprintf(etiqueta,"etiqueta%i",indice++);
  return etiqueta;
}

char* tipoIntermedio(TipoDato td) {
  /*struct Atributos uptd1;
  uptd1.dtipo=td;
  if (esArray(uptd))
    return "Array";
  else*/ if (td == booleano)
    return "int";
  else
    return tipoAString(td);
}

char* leerOp(TipoDato td1, char* exp1, char* op, char* exp2, TipoDato td2) {
  char* etiqueta = temporal();
  TipoDato tdPrimario = td1;
  char* expPrimaria = exp1;
  char* expSecundaria = exp2;

  // ESTO NO SE PARA QUE SIRVE
  if (/*esArray(td2) && */(!strcmp("+",op) || !strcmp("*",op))) {
    tdPrimario = td2;
    expPrimaria = exp2;
    expSecundaria = exp1;
  }

  // tipoOp AÑADIDO ARRIBA, ENCIMA DE ESARRAY
  gen("%s %s;\n", tipoIntermedio(tipoOp(tdPrimario, op)), etiqueta);

  // REVISAR OPERADORES QUE NO TENEMOS Y VER SI AL FINAL HACEMOS LAS COSAS DE ARRAY
  // ESCRIBIR FUNCIONES DE COSAS CON ARRAYS SI ES QUE LO HACEMOS
  if (!strcmp("#",op)) {
    gen("%s = getTam(%s);\n",etiqueta,exp1);
  } /*else if (!strcmp("?",op)) {
    gen("%s = *(%s*)getActual(%s);\n", etiqueta, tipoIntermedio(tipoArray(td1)), exp1);
  } else if (!strcmp("@", op)) {
    gen("%s = *(%s*)get(%s, %s);\n", etiqueta, tipoIntermedio(tipoArray(td1)), exp1, exp2);
  }*/ else if (!strcmp("--", op)) {
    gen("%s = borrarEn(%s, %s);\n", etiqueta, exp1, exp2);
  } else if (!strcmp("%", op)) {
    gen("%s = borrarAPartirDe(%s, %s);\n", etiqueta, exp1, exp2);
  } else if (!strcmp("**", op)) {
    gen("%s = concatenar(%s, %s);\n", etiqueta, exp1, exp2);
  }/* else if (esArray(tdPrimario)) {
    if (!strcmp("+", op)) {
      gen("%s = sumarArray(%s, %s);\n", etiqueta, expPrimaria, expSecundaria)
    } else if (!strcmp("-", op)) {
        gen("%s = restarArray(%s, %s);\n", etiqueta, expPrimaria, expSecundaria);
    } else if (!strcmp("*", op)) {
      gen("%s = multiplicarArray(%s, %s);\n", etiqueta, expPrimaria, expSecundaria);
    } else if (!strcmp("/", op)) {
      gen("%s = dividirArray(%s, %s);\n", etiqueta, expPrimaria, expSecundaria);
    }
  }*/ else if (!strcmp("", exp2)) {
    gen("%s = %s %s;\n", etiqueta, op, exp1);
  } else {
    gen("%s = %s %s %s;\n", etiqueta, exp1, op, exp2);
  }
  return etiqueta;
  
}

char * leerCte(char* cte, TipoDato td) {
  if (td == booleano) {
    if (!strcmp("verdadero",cte))
      return "1";
    else
      return "0";
  }
  return cte;
}

char* insertarDato(char* id, TipoDato td) {
  char* buffer = malloc(sizeof(char)*100);
  switch(td) {
    // NO SE POR QUE ENTERO ESTA VACIO
    case entero:
    // NO SE SI TENEMOS QUE DECLARAR pInt EN ALGUN LADO
    case booleano:
      sprintf(buffer, "pInt(%s)",id);
      return buffer;
    case real:
      sprintf(buffer, "pFloat(%s)",id);
      return buffer;
    case caracter:
      sprintf(buffer, "pChar(%s)",id);
      return buffer;
    default:
      if (!hayError) {
        sprintf(msgError, "ERROR INTERMEDIO: tipo no básico en insertarDato()\n");
        yyerror(msgError);
        // CON ESTO CREO QUE EN LA P4 NO NOS MOSTRABA LOS DEMAS ERRORES, Y SI LO QUITAMOS
        // A LO MEJOR DA CORE, COMPROBAR.
        exit(EXIT_FAILURE);
      }
  }
}

char* tipoImprimir(TipoDato tipo) {
  if (tipo == entero)
    return "%d";
  else if (tipo == real)
    return "%f";
  else if (/*esArray(tipo) ||*/ tipo == booleano)
    return "%s";
  else if (tipo == caracter)
    return "%c";
  else{
    if (!hayError) {
      sprintf(msgError, "ERROR INTERMEDIO: tipoImprimir() tipo no válido\n");
      yyerror(msgError);
      // IDEM insertarDato
      exit(EXIT_FAILURE);
    }
  }
}

char* inicializaTipoArray(TipoDato tipo) {
  if (tipo == entero)
    return "tInt";
  else if (tipo == real)
    return "tFloat";
  else if (tipo == caracter)
    return "tChar";
  else if (tipo == booleano)
    return "tBool";
  else {
    if (!hayError) {
      sprintf(msgError, "ERROR INTERMEDIO: tipo no válido en inicializaTipoArray()\n");
      yyerror(msgError);
      // IDEM insertarDato
      exit(EXIT_FAILURE);
    }
  }
}


/*

_____________________FIN GENERACION DE CODIGO INTERMEDIO________________________

*/

#define YYSTYPE Atributos


#line 1003 "y.tab.c"

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
#define YYLAST   301

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  123
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  222

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
       0,   984,   984,   984,   991,   992,   993,   991,   997,   998,
    1006,  1006,  1006,  1009,  1010,  1013,  1014,  1016,  1016,  1017,
    1021,  1022,  1024,  1029,  1034,  1034,  1035,  1035,  1039,  1040,
    1041,  1042,  1043,  1044,  1045,  1046,  1049,  1050,  1055,  1056,
    1057,  1058,  1055,  1061,  1061,  1061,  1064,  1065,  1064,  1069,
    1071,  1074,  1075,  1078,  1078,  1089,  1089,  1090,  1090,  1101,
    1101,  1104,  1106,  1107,  1109,  1110,  1111,  1112,  1114,  1120,
    1121,  1123,  1129,  1136,  1141,  1148,  1152,  1156,  1160,  1164,
    1168,  1172,  1176,  1180,  1185,  1186,  1192,  1197,  1198,  1199,
    1200,  1204,  1205,  1206,  1207,  1209,  1210,  1213,  1215,  1217,
    1219,  1222,  1225,  1229,  1232,  1236,  1239,  1243,  1246,  1250,
    1254,  1258,  1262,  1268,  1272,  1277,  1281,  1286,  1290,  1295,
    1299,  1362,  1365,  1368
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
  "OPUNARIO", "$accept", "programa", "$@1", "bloque", "$@2", "$@3", "$@4",
  "Declar_de_subprogs", "Declar_subprog", "$@5", "$@6",
  "Declar_de_variables_locales", "variables_locales",
  "cuerpo_declar_variables", "$@7", "lista_identificadores", "Tipo_vector",
  "Tipo_matriz", "Sentencias", "$@8", "$@9", "Sentencia",
  "sentencia_asignacion", "sentencia_if", "$@10", "$@11", "$@12", "$@13",
  "bloque_else", "$@14", "sentencia_while", "$@15", "$@16",
  "sentencia_entrada", "sentencia_salida", "lista_entrada",
  "lista_expresiones_o_cadena", "@17", "@18", "@19", "@20",
  "sentencia_case_switch", "Casos", "Bloque_de_caso", "sentencia_proc",
  "argumentos", "lista_argumentos", "lista_expresiones", "expresion",
  "constante", "vector", "lista_enteros", "lista_caracteres",
  "lista_booleanos", "lista_reales", "matriz", "matriz_enteros",
  "matriz_caracteres", "matriz_booleanos", "matriz_reales",
  "identificador", YY_NULLPTR
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

#define YYPACT_NINF (-161)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-76)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       3,     6,    13,    62,  -161,  -161,    96,  -161,  -161,    76,
      11,  -161,  -161,  -161,     7,  -161,  -161,    95,  -161,  -161,
     106,   112,    98,  -161,  -161,  -161,   114,  -161,  -161,   124,
    -161,    95,  -161,   159,   228,    36,  -161,   160,  -161,   228,
    -161,   167,   169,    95,    32,   200,    56,  -161,  -161,  -161,
    -161,  -161,  -161,  -161,  -161,  -161,   173,   168,  -161,    94,
    -161,  -161,  -161,   135,  -161,  -161,   195,   200,    95,   209,
    -161,  -161,  -161,  -161,  -161,   200,   200,   199,   226,  -161,
    -161,  -161,  -161,   184,   136,   200,   198,     8,   226,   200,
     200,  -161,    95,   209,    26,    80,   178,   196,   119,  -161,
    -161,  -161,  -161,  -161,    47,    48,    58,    81,  -161,  -161,
    -161,  -161,   170,   200,   200,   200,   200,   200,   200,   200,
     200,  -161,     4,   202,   208,   226,    78,  -161,   200,  -161,
     226,   226,  -161,    82,    92,   115,   118,  -161,   216,  -161,
     224,  -161,   225,  -161,   227,  -161,   206,  -161,   215,  -161,
     219,  -161,   255,  -161,  -161,   226,   217,   232,   237,   241,
      29,   133,   243,  -161,   165,   214,    15,  -161,   277,   200,
    -161,   226,    96,   275,   276,  -161,  -161,  -161,  -161,   261,
     260,   264,   263,  -161,  -161,  -161,  -161,  -161,  -161,   259,
    -161,    19,  -161,  -161,  -161,   226,  -161,  -161,   228,   120,
     122,   131,   132,  -161,   268,   228,  -161,  -161,  -161,  -161,
    -161,     5,   286,  -161,   287,  -161,   278,  -161,  -161,  -161,
     228,  -161
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     2,     0,     4,     3,    14,
       0,     5,    19,    17,     0,    16,     9,     0,    13,    15,
       6,   121,     0,   122,   123,    21,     0,    26,     8,     0,
      18,     0,    10,    24,     0,     0,    20,     0,     7,     0,
      37,     0,     0,     0,     0,     0,   121,    28,    27,    29,
      30,    31,    32,    33,    35,    34,     0,     0,    22,     0,
      25,    38,    46,     0,    52,    90,     0,     0,     0,     0,
      93,    91,    92,    94,    59,     0,     0,     0,    57,    88,
      95,    96,    87,     0,     0,     0,     0,     0,    74,     0,
       0,    49,     0,     0,     0,     0,     0,     0,     0,    89,
     106,   102,   108,   104,     0,     0,     0,     0,    60,    78,
      77,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,    69,    72,     0,    23,     0,    11,
      39,    47,    51,     0,     0,     0,     0,   109,     0,   110,
       0,   111,     0,   112,     0,    76,     0,    97,     0,    98,
       0,    99,     0,   100,    55,    53,    83,    82,    84,    80,
      81,    86,    85,    79,     0,    26,     0,    63,     0,     0,
      36,    73,     0,     0,     0,   114,   116,   118,   120,     0,
       0,     0,     0,   101,   103,   105,   107,    56,    54,     0,
      26,    24,    61,    62,    68,    71,    12,    40,     0,     0,
       0,     0,     0,    26,    24,     0,    48,   113,   115,   117,
     119,    24,     0,    41,     0,    66,    45,    64,    43,    42,
       0,    44
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -161,  -161,  -161,    -4,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,  -161,  -161,   281,  -161,  -161,  -161,  -161,  -160,  -161,
    -161,   -38,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,  -161,  -161,   134,  -161,  -161,  -161,  -161,   -36,   137,
    -161,   -83,   -86,   -90,   -89,  -161,  -161,  -161,  -161,  -161,
     -17
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     6,    47,     9,    16,    27,    20,    28,    37,
     172,    11,    14,    15,    17,    22,    23,    24,    33,    39,
      34,    48,    49,    50,    89,   173,   205,   216,   219,   220,
      51,    90,   174,    52,    53,    63,    77,   188,   187,   121,
     108,    54,   166,   167,    55,   123,   124,    87,    78,    79,
      80,   104,   105,   106,   107,    81,    94,    95,    96,    97,
      82
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,    60,     8,   135,   136,   191,     1,   134,    12,    83,
     133,   -65,    12,     4,    36,     3,   128,    56,   129,    18,
      13,   192,    56,    88,    13,   -67,    64,   164,   -65,   214,
     204,    98,   137,    65,   138,   165,   -65,    66,   164,   109,
     110,    67,   -67,   211,    57,    68,   165,    69,   125,   126,
     -67,    99,    58,   130,   131,   146,   148,    70,    71,    72,
      73,    74,    21,   147,   149,    84,   150,   118,   119,   120,
      75,    29,     5,    76,   151,   132,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   170,   139,    10,   140,   152,
     146,   201,   171,   202,   200,    65,   199,   153,   175,    66,
     148,     7,   -75,    67,   -75,    30,    31,    68,   176,    69,
      26,   113,   114,   115,   116,   117,   118,   119,   120,    70,
      71,    72,    73,   150,    21,    21,   152,    29,   146,   145,
     148,   177,    75,   195,   178,    76,   207,    65,   208,   150,
     152,    66,    91,    92,    32,    67,   -70,   209,   210,    68,
      35,    69,   113,   114,   115,   116,   117,   118,   119,   120,
     206,    70,    71,    72,    73,    38,    21,   213,   196,    59,
      66,    65,   119,   120,    75,    66,    61,    76,    62,    67,
      69,    56,   221,    68,   141,    69,   142,    85,    56,   122,
      70,    71,    72,    73,    86,    70,    71,    72,    73,   154,
      21,    65,   143,    56,   144,    66,   111,   112,    75,    67,
      93,    76,   168,    68,   127,    69,   169,   113,   114,   115,
     116,   117,   118,   119,   120,    70,    71,    72,    73,    40,
      21,   179,   183,     7,   100,   101,   102,   103,    75,   180,
     181,    76,   182,   184,   185,    41,   190,    42,    43,    44,
      45,   114,   115,   116,   117,   118,   119,   120,    46,   113,
     114,   115,   116,   117,   118,   119,   120,   115,   116,   117,
     118,   119,   120,   116,   117,   118,   119,   120,   117,   118,
     119,   120,   186,   120,   194,   197,   198,   101,   103,   100,
     102,   203,   212,   215,   217,    19,   218,     0,     0,     0,
     193,   189
};

static const yytype_int16 yycheck[] =
{
      17,    39,     6,    93,    93,   165,     3,    93,     1,    45,
      93,     6,     1,     0,    31,     9,     8,    34,    10,    12,
      13,     6,    39,    59,    13,     6,    43,    23,    23,    24,
     190,    67,     6,     1,     8,    31,    31,     5,    23,    75,
      76,     9,    23,   203,     8,    13,    31,    15,    84,    85,
      31,    68,    16,    89,    90,     8,     8,    25,    26,    27,
      28,    29,    30,    16,    16,     9,     8,    38,    39,    40,
      38,    15,    10,    41,    16,    92,   112,   113,   114,   115,
     116,   117,   118,   119,   120,     7,     6,    11,     8,     8,
       8,   181,   128,   182,   180,     1,   179,    16,    16,     5,
       8,     5,     8,     9,    10,     7,     8,    13,    16,    15,
       4,    33,    34,    35,    36,    37,    38,    39,    40,    25,
      26,    27,    28,     8,    30,    30,     8,    15,     8,    10,
       8,    16,    38,   169,    16,    41,    16,     1,    16,     8,
       8,     5,     7,     8,    30,     9,    10,    16,    16,    13,
      26,    15,    33,    34,    35,    36,    37,    38,    39,    40,
     198,    25,    26,    27,    28,     6,    30,   205,   172,     9,
       5,     1,    39,    40,    38,     5,     9,    41,     9,     9,
      15,   198,   220,    13,     6,    15,     8,    14,   205,     5,
      25,    26,    27,    28,    26,    25,    26,    27,    28,    29,
      30,     1,     6,   220,     8,     5,     7,     8,    38,     9,
      15,    41,    10,    13,    16,    15,     8,    33,    34,    35,
      36,    37,    38,    39,    40,    25,    26,    27,    28,     1,
      30,    15,    26,     5,    25,    26,    27,    28,    38,    15,
      15,    41,    15,    28,    25,    17,    32,    19,    20,    21,
      22,    34,    35,    36,    37,    38,    39,    40,    30,    33,
      34,    35,    36,    37,    38,    39,    40,    35,    36,    37,
      38,    39,    40,    36,    37,    38,    39,    40,    37,    38,
      39,    40,    27,    40,     7,    10,    10,    26,    28,    25,
      27,    32,    24,     7,     7,    14,    18,    -1,    -1,    -1,
     166,   164
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    43,     9,     0,    10,    44,     5,    45,    46,
      11,    53,     1,    13,    54,    55,    47,    56,    12,    55,
      49,    30,    57,    58,    59,   102,     4,    48,    50,    15,
       7,     8,    30,    60,    62,    26,   102,    51,     6,    61,
       1,    17,    19,    20,    21,    22,    30,    45,    63,    64,
      65,    72,    75,    76,    83,    86,   102,     8,    16,     9,
      63,     9,     9,    77,   102,     1,     5,     9,    13,    15,
      25,    26,    27,    28,    29,    38,    41,    78,    90,    91,
      92,    97,   102,    90,     9,    14,    26,    89,    90,    66,
      73,     7,     8,    15,    98,    99,   100,   101,    90,   102,
      25,    26,    27,    28,    93,    94,    95,    96,    82,    90,
      90,     7,     8,    33,    34,    35,    36,    37,    38,    39,
      40,    81,     5,    87,    88,    90,    90,    16,     8,    10,
      90,    90,   102,    93,    94,    95,    96,     6,     8,     6,
       8,     6,     8,     6,     8,    10,     8,    16,     8,    16,
       8,    16,     8,    16,    29,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    23,    31,    84,    85,    10,     8,
       7,    90,    52,    67,    74,    16,    16,    16,    16,    15,
      15,    15,    15,    26,    28,    25,    27,    80,    79,    91,
      32,    60,     6,    85,     7,    90,    45,    10,    10,    93,
      94,    95,    96,    32,    60,    68,    63,    16,    16,    16,
      16,    60,    24,    63,    24,     7,    69,     7,    18,    70,
      71,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    44,    43,    46,    47,    48,    45,    49,    49,
      51,    52,    50,    53,    53,    54,    54,    56,    55,    55,
      57,    57,    58,    59,    61,    60,    62,    60,    63,    63,
      63,    63,    63,    63,    63,    63,    64,    64,    66,    67,
      68,    69,    65,    71,    70,    70,    73,    74,    72,    75,
      76,    77,    77,    79,    78,    80,    78,    81,    78,    82,
      78,    83,    84,    84,    85,    85,    85,    85,    86,    87,
      87,    88,    88,    89,    89,    89,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    91,    91,    91,    91,    91,    91,    92,    92,    92,
      92,    93,    93,    94,    94,    95,    95,    96,    96,    97,
      97,    97,    97,    98,    98,    99,    99,   100,   100,   101,
     101,   102,   102,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     5,     0,     0,     0,     8,     2,     0,
       0,     0,     8,     3,     0,     2,     1,     0,     4,     1,
       3,     1,     4,     6,     0,     3,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     0,     0,
       0,     0,    10,     0,     3,     0,     0,     0,     7,     3,
       3,     3,     1,     0,     4,     0,     4,     0,     2,     0,
       2,     5,     2,     1,     6,     4,     5,     2,     5,     1,
       0,     3,     1,     3,     1,     0,     3,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       3,     3,     3,     5,     3,     5,     3,     5,     3,     5,
       3,     1,     1,     1
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
#line 984 "ejemplosyntax.y"
                                               {
                          gen("#include <stdlib.h>\n");
                          gen("#include <stdio.h>\n");
                          gen("#include \"dec_dat.h\"\n\n");
                          gen("#include \"dec_func.h\"\n");
                      }
#line 2461 "y.tab.c"
    break;

  case 4:
#line 991 "ejemplosyntax.y"
                      { insertarMarca(); if (yyout != fMain || (yyout == fMain && deep > 0)) {gen("{\n"); ++deep;} }
#line 2467 "y.tab.c"
    break;

  case 5:
#line 992 "ejemplosyntax.y"
                                { if (deep == 0) yyout = fProc; }
#line 2473 "y.tab.c"
    break;

  case 6:
#line 993 "ejemplosyntax.y"
                       { if (deep == 0) {yyout = fMain; gen("int main()\n{\n"); ++deep;} }
#line 2479 "y.tab.c"
    break;

  case 7:
#line 995 "ejemplosyntax.y"
              { vaciarEntradas(); --deep; gen("}\n\n"); }
#line 2485 "y.tab.c"
    break;

  case 10:
#line 1006 "ejemplosyntax.y"
                                     { insertarProc(yyvsp[0].lexema); }
#line 2491 "y.tab.c"
    break;

  case 11:
#line 1006 "ejemplosyntax.y"
                                                                                                              { subProg = 1; gen("void %s(%s)\n", yyvsp[-4].lexema, yyvsp[-1].lexema); }
#line 2497 "y.tab.c"
    break;

  case 13:
#line 1009 "ejemplosyntax.y"
                                                                          { gen("\n"); }
#line 2503 "y.tab.c"
    break;

  case 17:
#line 1016 "ejemplosyntax.y"
                                       { tipoTmp = yyvsp[0].dtipo; }
#line 2509 "y.tab.c"
    break;

  case 18:
#line 1016 "ejemplosyntax.y"
                                                                                         { gen("%s %s;\n", tipoIntermedio(yyvsp[-3].dtipo),yyvsp[-1].lexema); }
#line 2515 "y.tab.c"
    break;

  case 20:
#line 1021 "ejemplosyntax.y"
                                                                 { insertarVariable(yyvsp[0].lexema,yyvsp[0].dimensiones,yyvsp[0].TamDimen1,yyvsp[0].TamDimen2); }
#line 2521 "y.tab.c"
    break;

  case 21:
#line 1022 "ejemplosyntax.y"
                            { insertarVariable(yyvsp[0].lexema,yyvsp[0].dimensiones,yyvsp[0].TamDimen1,yyvsp[0].TamDimen2); }
#line 2527 "y.tab.c"
    break;

  case 22:
#line 1024 "ejemplosyntax.y"
                                                      { {yyval.lexema = yyvsp[-3].lexema; 
                                                              yyval.dimensiones=1;
                                                              yyval.TamDimen1=atoi(yyvsp[-1].lexema);
                                                              yyval.TamDimen2=0;}}
#line 2536 "y.tab.c"
    break;

  case 23:
#line 1029 "ejemplosyntax.y"
                                                                  {yyval.lexema = yyvsp[-5].lexema;
                                                                          yyval.dimensiones=2;
                                                                          yyval.TamDimen1=atoi(yyvsp[-3].lexema);
                                                                          yyval.TamDimen2=atoi(yyvsp[-1].lexema);}
#line 2545 "y.tab.c"
    break;

  case 24:
#line 1034 "ejemplosyntax.y"
                        { gen("{\n"); ++deep; }
#line 2551 "y.tab.c"
    break;

  case 25:
#line 1034 "ejemplosyntax.y"
                                                          { --deep; gen("\n"); }
#line 2557 "y.tab.c"
    break;

  case 26:
#line 1035 "ejemplosyntax.y"
                  { gen("\n"); ++deep; }
#line 2563 "y.tab.c"
    break;

  case 27:
#line 1035 "ejemplosyntax.y"
                                                   { --deep; gen("\n"); }
#line 2569 "y.tab.c"
    break;

  case 36:
#line 1049 "ejemplosyntax.y"
                                                              {comprobarAsignacion(yyvsp[-3].lexema, yyvsp[-1]); gen("%s = %s;\n", yyvsp[-3].lexema, yyvsp[-1].lexema); }
#line 2575 "y.tab.c"
    break;

  case 38:
#line 1055 "ejemplosyntax.y"
                                 { insertarDescriptor("",etiqueta(),etiqueta()); }
#line 2581 "y.tab.c"
    break;

  case 39:
#line 1056 "ejemplosyntax.y"
                                { expresionBooleana(yyvsp[0].dtipo); gen("\n"); gen("if (!%s) goto %s;\n",yyvsp[0].lexema, ts[tope].descriptor->etiquetaElse); }
#line 2587 "y.tab.c"
    break;

  case 40:
#line 1057 "ejemplosyntax.y"
                                  {gen("\n"); ++deep; }
#line 2593 "y.tab.c"
    break;

  case 41:
#line 1058 "ejemplosyntax.y"
                                { { --deep; gen("}\n"); } DescriptorDeInstrControl* ds = ts[tope].descriptor; gen("goto %s;\n\n", ds->etiquetaSalida); gen("%s: {} \n", ts[tope].descriptor->etiquetaElse); }
#line 2599 "y.tab.c"
    break;

  case 42:
#line 1059 "ejemplosyntax.y"
                                  { gen("\n"); gen("%s: {} \n", ts[tope].descriptor->etiquetaSalida); --tope;}
#line 2605 "y.tab.c"
    break;

  case 43:
#line 1061 "ejemplosyntax.y"
                   { gen("\n"); gen("{\n"); ++deep; }
#line 2611 "y.tab.c"
    break;

  case 44:
#line 1061 "ejemplosyntax.y"
                                                                { --deep; gen("}\n")}
#line 2617 "y.tab.c"
    break;

  case 45:
#line 1061 "ejemplosyntax.y"
                                                                                              { int aux = deep; deep = 0; gen(" {}\n"); deep = aux; }
#line 2623 "y.tab.c"
    break;

  case 46:
#line 1064 "ejemplosyntax.y"
                                         { insertarDescriptor(etiqueta(),etiqueta(),""); gen("%s:\n", ts[tope].descriptor->etiquetaEntrada); gen("{\n"); ++deep; gen("{\n"); ++deep; }
#line 2629 "y.tab.c"
    break;

  case 47:
#line 1065 "ejemplosyntax.y"
                                      { expresionBooleana(yyvsp[0].dtipo); gen("\n"); gen("if (!%s) goto %s;\n", yyvsp[0].lexema, ts[tope].descriptor->etiquetaSalida); --deep; gen("}\n\n"); }
#line 2635 "y.tab.c"
    break;

  case 48:
#line 1066 "ejemplosyntax.y"
                                                  { gen("goto %s;\n\n", ts[tope].descriptor->etiquetaEntrada); --deep; gen("}\n"); gen("%s: {}\n", ts[tope].descriptor->etiquetaSalida); --tope; }
#line 2641 "y.tab.c"
    break;

  case 50:
#line 1071 "ejemplosyntax.y"
                                                         { gen("printf(\"\\n\");\n"); }
#line 2647 "y.tab.c"
    break;

  case 53:
#line 1078 "ejemplosyntax.y"
                                                                       { if (esArray(yyvsp[0])) {
                                                                            yyval.lexema = malloc(sizeof(char)*(15+strlen(yyvsp[0].lexema)));
                                                                            sprintf(yyval.lexema, "arrayAString(%s)",yyvsp[0].lexema);
                                                                          } else if (yyvsp[0].dtipo == booleano) {
                                                                            yyval.lexema = malloc(sizeof(char)*(8+strlen(yyvsp[0].lexema)));
                                                                            sprintf(yyval.lexema, "aBool(%s)", yyvsp[0].lexema);
                                                                          } else {
                                                                            yyval.lexema = yyvsp[0].lexema;
                                                                          }
                                                                          yyval.dtipo = yyvsp[0].dtipo;
                                                                        }
#line 2663 "y.tab.c"
    break;

  case 54:
#line 1088 "ejemplosyntax.y"
                                                                          { gen("printf(\"%s\",%s);\n", tipoImprimir(yyvsp[-1].dtipo),yyvsp[-1].lexema); gen("fflush(stdout);\n"); }
#line 2669 "y.tab.c"
    break;

  case 55:
#line 1089 "ejemplosyntax.y"
                                                         { yyval.lexema = yyvsp[0].lexema; yyval.dtipo = yyvsp[0].dtipo; }
#line 2675 "y.tab.c"
    break;

  case 56:
#line 1089 "ejemplosyntax.y"
                                                                                                         { gen("printf(\"%s\",%s);\n", tipoImprimir(yyvsp[-1].dtipo),yyvsp[-1].lexema); gen("fflush(stdout);\n"); }
#line 2681 "y.tab.c"
    break;

  case 57:
#line 1090 "ejemplosyntax.y"
                            { if (esArray(yyvsp[0])) {
                                yyval.lexema = malloc(sizeof(char)*(15+strlen(yyvsp[0].lexema)));
                                sprintf(yyval.lexema, "arrayAString(%s)",yyvsp[0].lexema);
                              } else if (yyvsp[0].dtipo == booleano) {
                                yyval.lexema = malloc(sizeof(char)*(8+strlen(yyvsp[0].lexema)));
                                sprintf(yyval.lexema, "aBool(%s)", yyvsp[0].lexema);
                              } else {
                                yyval.lexema = yyvsp[0].lexema;
                              }
                              yyval.dtipo = yyvsp[0].dtipo;
                            }
#line 2697 "y.tab.c"
    break;

  case 58:
#line 1100 "ejemplosyntax.y"
                              { gen("printf(\"%s\",%s);\n", tipoImprimir(yyvsp[-1].dtipo),yyvsp[-1].lexema); gen("fflush(stdout);\n"); }
#line 2703 "y.tab.c"
    break;

  case 59:
#line 1101 "ejemplosyntax.y"
                         { yyval.lexema = yyvsp[0].lexema; yyval.dtipo = yyvsp[0].dtipo; }
#line 2709 "y.tab.c"
    break;

  case 60:
#line 1101 "ejemplosyntax.y"
                                                                         { gen("printf(\"%s\",%s);\n", tipoImprimir(yyvsp[-1].dtipo),yyvsp[-1].lexema); gen("fflush(stdout);\n"); }
#line 2715 "y.tab.c"
    break;

  case 68:
#line 1114 "ejemplosyntax.y"
                                                                { 
                                comprobarProcedimiento(yyvsp[-4].lexema);
                                yyval.lexema = malloc(sizeof(char)*(strlen(yyvsp[-4].lexema)+strlen(yyvsp[-2].lexema)+3));
                                sprintf(yyval.lexema, "%s(%s)", yyvsp[-4].lexema, yyvsp[-2].lexema);
                                }
#line 2725 "y.tab.c"
    break;

  case 69:
#line 1120 "ejemplosyntax.y"
                              { yyval.lexema = yyvsp[0].lexema; }
#line 2731 "y.tab.c"
    break;

  case 70:
#line 1121 "ejemplosyntax.y"
                   { yyval.lexema = ""; }
#line 2737 "y.tab.c"
    break;

  case 71:
#line 1123 "ejemplosyntax.y"
                                                   {
                    yyval.lexema = malloc(sizeof(char)*(strlen(yyvsp[-2].lexema)+strlen(yyvsp[0].lexema)+3));
                    sprintf(yyval.lexema, "%s, %s", yyvsp[-2].lexema, yyvsp[0].lexema);
                    argumentos_tipo_datos[n_argumentos] = yyvsp[0].dtipo;
                    n_argumentos++;
                  }
#line 2748 "y.tab.c"
    break;

  case 72:
#line 1129 "ejemplosyntax.y"
                      {
                  yyval.lexema = yyvsp[0].lexema;
                  argumentos_tipo_datos[n_argumentos] = yyvsp[0].dtipo;
                  n_argumentos++;
                }
#line 2758 "y.tab.c"
    break;

  case 73:
#line 1136 "ejemplosyntax.y"
                                                     {
                                        yyval.lexema = yyvsp[-2].lexema;
                                        yyval.dtipo = /*mismoTipoArray($1.dtipo,$3.dtipo)*/ yyvsp[-2].dtipo;//He puesto un tipo de los 2
                                        gen("insertar(%s %s);\n", yyval.lexema, insertarDato(yyvsp[0].lexema, yyvsp[0].dtipo));
                                      }
#line 2768 "y.tab.c"
    break;

  case 74:
#line 1141 "ejemplosyntax.y"
                          {
                        yyval.lexema = temporal();
                        yyval.dtipo = yyvsp[0].dtipo;
                        gen("Array %s;\n", yyval.lexema);
                        gen("%s = inicializar(%s);\n", yyval.lexema, inicializaTipoArray(yyval.dtipo));
                        gen("insertar(%s, %s);\n", yyval.lexema, insertarDato(yyvsp[0].lexema,yyvsp[0].dtipo));
                      }
#line 2780 "y.tab.c"
    break;

  case 76:
#line 1152 "ejemplosyntax.y"
                                                { 
                                yyval.lexema = yyvsp[-1].lexema; 
                                yyval.dtipo = yyvsp[-1].dtipo; 
                              }
#line 2789 "y.tab.c"
    break;

  case 77:
#line 1156 "ejemplosyntax.y"
                                     { 
                                yyval.lexema = leerOp(yyvsp[0].dtipo,yyvsp[0].lexema,yyvsp[-1].lexema,"",-1);
                                yyval.dtipo = excl(yyvsp[0].dtipo); 
                              }
#line 2798 "y.tab.c"
    break;

  case 78:
#line 1160 "ejemplosyntax.y"
                                                     { 
                                yyval.lexema = leerOp(yyvsp[0].dtipo, yyvsp[0].lexema, yyvsp[-1].lexema, "", -1); 
                                yyval.dtipo = masMenos(yyvsp[-1].atributo, yyvsp[0].dtipo); 
                              }
#line 2807 "y.tab.c"
    break;

  case 79:
#line 1164 "ejemplosyntax.y"
                                               { 
                                yyval.lexema = leerOp(yyvsp[-2].dtipo,yyvsp[-2].lexema,yyvsp[-1].lexema,yyvsp[0].lexema,yyvsp[0].dtipo);
                                yyval.dtipo = porDiv(yyvsp[-2], yyvsp[-1].atributo, yyvsp[0]); 
                              }
#line 2816 "y.tab.c"
    break;

  case 80:
#line 1168 "ejemplosyntax.y"
                                              { 
                                yyval.lexema = leerOp(yyvsp[-2].dtipo,yyvsp[-2].lexema,yyvsp[-1].lexema,yyvsp[0].lexema,yyvsp[0].dtipo);
                                yyval.dtipo = eqn(yyvsp[-2].dtipo, yyvsp[-1].atributo, yyvsp[0].dtipo); 
                              }
#line 2825 "y.tab.c"
    break;

  case 81:
#line 1172 "ejemplosyntax.y"
                                            { 
                                yyval.lexema = leerOp(yyvsp[-2].dtipo,yyvsp[-2].lexema,yyvsp[-1].lexema,yyvsp[0].lexema,yyvsp[0].dtipo);
                                yyval.dtipo = rel(yyvsp[-2].dtipo, yyvsp[-1].atributo, yyvsp[0].dtipo); 
                              }
#line 2834 "y.tab.c"
    break;

  case 82:
#line 1176 "ejemplosyntax.y"
                                            { 
                                yyval.lexema = leerOp(yyvsp[-2].dtipo,yyvsp[-2].lexema,yyvsp[-1].lexema,yyvsp[0].lexema,yyvsp[0].dtipo);  
                                yyval.dtipo = andLog(yyvsp[-2].dtipo, yyvsp[0].dtipo); 
                              }
#line 2843 "y.tab.c"
    break;

  case 83:
#line 1180 "ejemplosyntax.y"
                                           { 
                                yyval.lexema = leerOp(yyvsp[-2].dtipo,yyvsp[-2].lexema,yyvsp[-1].lexema,yyvsp[0].lexema,yyvsp[0].dtipo);
                                yyval.dtipo = orLog(yyvsp[-2].dtipo, yyvsp[0].dtipo); 
                              }
#line 2852 "y.tab.c"
    break;

  case 84:
#line 1185 "ejemplosyntax.y"
                                             { yyval.dtipo = xorLog(yyvsp[-2].dtipo, yyvsp[0].dtipo); }
#line 2858 "y.tab.c"
    break;

  case 85:
#line 1186 "ejemplosyntax.y"
                                            {
                                yyval.lexema = leerOp(yyvsp[-2].dtipo,yyvsp[-2].lexema,yyvsp[-1].lexema,yyvsp[0].lexema,yyvsp[0].dtipo);
                                yyval.dtipo = porPor(yyvsp[-2], yyvsp[0]); 
                                yyval.TamDimen1 = yyvsp[-2].TamDimen1;
                                yyval.TamDimen2 = yyvsp[0].TamDimen2; 
                              }
#line 2869 "y.tab.c"
    break;

  case 86:
#line 1192 "ejemplosyntax.y"
                                                { 
                                yyval.lexema = leerOp(yyvsp[-2].dtipo,yyvsp[-2].lexema,yyvsp[-1].lexema,yyvsp[0].lexema,yyvsp[0].dtipo);  
                                yyval.dtipo = addSub(yyvsp[-2], yyvsp[-1].atributo, yyvsp[0]); 
                              }
#line 2878 "y.tab.c"
    break;

  case 87:
#line 1197 "ejemplosyntax.y"
                                { yyval = entradaAAtributos(buscarID(yyvsp[0].lexema)); }
#line 2884 "y.tab.c"
    break;

  case 88:
#line 1198 "ejemplosyntax.y"
                             { yyval.lexema = yyvsp[0].lexema; yyval.dtipo = yyvsp[0].dtipo; }
#line 2890 "y.tab.c"
    break;

  case 89:
#line 1199 "ejemplosyntax.y"
                                             { insertarParametro(yyvsp[-1].dtipo, yyvsp[0].lexema); }
#line 2896 "y.tab.c"
    break;

  case 91:
#line 1204 "ejemplosyntax.y"
                   { yyval.lexema = leerCte(yyvsp[0].lexema,yyvsp[0].dtipo); yyval.dtipo = yyvsp[0].dtipo; }
#line 2902 "y.tab.c"
    break;

  case 92:
#line 1205 "ejemplosyntax.y"
               { yyval.lexema = leerCte(yyvsp[0].lexema,yyvsp[0].dtipo); yyval.dtipo = yyvsp[0].dtipo; }
#line 2908 "y.tab.c"
    break;

  case 93:
#line 1206 "ejemplosyntax.y"
                      { yyval.lexema = leerCte(yyvsp[0].lexema,yyvsp[0].dtipo); yyval.dtipo = yyvsp[0].dtipo; }
#line 2914 "y.tab.c"
    break;

  case 94:
#line 1207 "ejemplosyntax.y"
                   { yyval.lexema = leerCte(yyvsp[0].lexema,yyvsp[0].dtipo); yyval.dtipo = yyvsp[0].dtipo; }
#line 2920 "y.tab.c"
    break;

  case 95:
#line 1209 "ejemplosyntax.y"
                 { yyval = yyvsp[0]; }
#line 2926 "y.tab.c"
    break;

  case 96:
#line 1210 "ejemplosyntax.y"
                  { yyval = yyvsp[0]; }
#line 2932 "y.tab.c"
    break;

  case 97:
#line 1213 "ejemplosyntax.y"
                                                  { yyval.dtipo = yyvsp[-1].dtipo;
                                                    if (yyvsp[-1].dimensiones==1){yyval.TamDimen2 = 0; yyval.TamDimen1 = yyvsp[-1].TamDimen1; yyval.dimensiones = 1;}}
#line 2939 "y.tab.c"
    break;

  case 98:
#line 1215 "ejemplosyntax.y"
                                                      { yyval.dtipo = yyvsp[-1].dtipo;
                                                    if (yyvsp[-1].dimensiones==1){yyval.TamDimen2 = 0; yyval.TamDimen1 = yyvsp[-1].TamDimen1; yyval.dimensiones = 1; }}
#line 2946 "y.tab.c"
    break;

  case 99:
#line 1217 "ejemplosyntax.y"
                                                     {yyval.dtipo = yyvsp[-1].dtipo;
                                                    if (yyvsp[-1].dimensiones==1){yyval.TamDimen2 = 0; yyval.TamDimen1 = yyvsp[-1].TamDimen1; yyval.dimensiones = 1; }}
#line 2953 "y.tab.c"
    break;

  case 100:
#line 1219 "ejemplosyntax.y"
                                                  { yyval.dtipo = yyvsp[-1].dtipo;
                                                    if (yyvsp[-1].dimensiones==1){yyval.TamDimen2 = 0; yyval.TamDimen1 = yyvsp[-1].TamDimen1; yyval.dimensiones = 1; }}
#line 2960 "y.tab.c"
    break;

  case 101:
#line 1222 "ejemplosyntax.y"
                                          {yyval.dtipo = yyvsp[0].dtipo;
                                           yyval.dimensiones = 1;
                                           yyval.TamDimen1 = yyvsp[-2].TamDimen1+1;}
#line 2968 "y.tab.c"
    break;

  case 102:
#line 1225 "ejemplosyntax.y"
                     {yyval.dtipo = yyvsp[0].dtipo;
                      yyval.dimensiones = 1;
                      yyval.TamDimen1 = 1;}
#line 2976 "y.tab.c"
    break;

  case 103:
#line 1229 "ejemplosyntax.y"
                                                  {yyval.dtipo = yyvsp[0].dtipo;
                                           yyval.dimensiones = 1;
                                           yyval.TamDimen1 = yyvsp[-2].TamDimen1+1;}
#line 2984 "y.tab.c"
    break;

  case 104:
#line 1232 "ejemplosyntax.y"
                       {yyval.dtipo = yyvsp[0].dtipo;
                      yyval.dimensiones = 1;
                      yyval.TamDimen1 = 1;}
#line 2992 "y.tab.c"
    break;

  case 105:
#line 1236 "ejemplosyntax.y"
                                                   {yyval.dtipo = yyvsp[0].dtipo;
                                           yyval.dimensiones = 1;
                                           yyval.TamDimen1 = yyvsp[-2].TamDimen1+1;}
#line 3000 "y.tab.c"
    break;

  case 106:
#line 1239 "ejemplosyntax.y"
                          {yyval.dtipo = yyvsp[0].dtipo;
                      yyval.dimensiones = 1;
                      yyval.TamDimen1 = 1;}
#line 3008 "y.tab.c"
    break;

  case 107:
#line 1243 "ejemplosyntax.y"
                                      {yyval.dtipo = yyvsp[0].dtipo;
                                           yyval.dimensiones = 1;
                                           yyval.TamDimen1 = yyvsp[-2].TamDimen1+1;}
#line 3016 "y.tab.c"
    break;

  case 108:
#line 1246 "ejemplosyntax.y"
                   {yyval.dtipo = yyvsp[0].dtipo;
                      yyval.dimensiones = 1;
                      yyval.TamDimen1 = 1;}
#line 3024 "y.tab.c"
    break;

  case 109:
#line 1250 "ejemplosyntax.y"
                                               { yyval.dtipo = yyvsp[-1].dtipo; 
                                                 yyval.dimensiones = yyvsp[-1].dimensiones;
                                                 yyval.TamDimen1 = yyvsp[-1].TamDimen1;
                                                 yyval.TamDimen2 = yyvsp[-1].TamDimen2;}
#line 3033 "y.tab.c"
    break;

  case 110:
#line 1254 "ejemplosyntax.y"
                                                       { yyval.dtipo = yyvsp[-1].dtipo; 
                                                 yyval.dimensiones = yyvsp[-1].dimensiones;
                                                 yyval.TamDimen1 = yyvsp[-1].TamDimen1;
                                                 yyval.TamDimen2 = yyvsp[-1].TamDimen2;}
#line 3042 "y.tab.c"
    break;

  case 111:
#line 1258 "ejemplosyntax.y"
                                                      { yyval.dtipo = yyvsp[-1].dtipo; 
                                                 yyval.dimensiones = yyvsp[-1].dimensiones;
                                                 yyval.TamDimen1 = yyvsp[-1].TamDimen1;
                                                 yyval.TamDimen2 = yyvsp[-1].TamDimen2;}
#line 3051 "y.tab.c"
    break;

  case 112:
#line 1262 "ejemplosyntax.y"
                                                   { yyval.dtipo = yyvsp[-1].dtipo; 
                                                 yyval.dimensiones = yyvsp[-1].dimensiones;
                                                 yyval.TamDimen1 = yyvsp[-1].TamDimen1;
                                                 yyval.TamDimen2 = yyvsp[-1].TamDimen2;}
#line 3060 "y.tab.c"
    break;

  case 113:
#line 1268 "ejemplosyntax.y"
                                                                              {yyval.TamDimen2 = yyvsp[-1].TamDimen1; 
                                                                               yyval.TamDimen1 = yyvsp[-4].TamDimen1+1;
                                                                               yyval.dtipo = yyvsp[-1].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 3069 "y.tab.c"
    break;

  case 114:
#line 1272 "ejemplosyntax.y"
                                                             {yyval.TamDimen2 = yyvsp[0].TamDimen1; 
                                                                               yyval.TamDimen1 = 1;
                                                                               yyval.dtipo = yyvsp[0].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 3078 "y.tab.c"
    break;

  case 115:
#line 1277 "ejemplosyntax.y"
                                                                                        {yyval.TamDimen2 = yyvsp[-1].TamDimen1; 
                                                                               yyval.TamDimen1 = yyvsp[-4].TamDimen1+1;
                                                                               yyval.dtipo = yyvsp[-1].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 3087 "y.tab.c"
    break;

  case 116:
#line 1281 "ejemplosyntax.y"
                                                                {yyval.TamDimen2 = yyvsp[0].TamDimen1; 
                                                                               yyval.TamDimen1 = 1;
                                                                               yyval.dtipo = yyvsp[0].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 3096 "y.tab.c"
    break;

  case 117:
#line 1286 "ejemplosyntax.y"
                                                                                     {yyval.TamDimen2 = yyvsp[-1].TamDimen1; 
                                                                               yyval.TamDimen1 = yyvsp[-4].TamDimen1+1;
                                                                               yyval.dtipo = yyvsp[-1].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 3105 "y.tab.c"
    break;

  case 118:
#line 1290 "ejemplosyntax.y"
                                                               {yyval.TamDimen2 = yyvsp[0].TamDimen1; 
                                                                               yyval.TamDimen1 = 1;
                                                                               yyval.dtipo = yyvsp[0].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 3114 "y.tab.c"
    break;

  case 119:
#line 1295 "ejemplosyntax.y"
                                                                            {yyval.TamDimen2 = yyvsp[-1].TamDimen1; 
                                                                               yyval.TamDimen1 = yyvsp[-4].TamDimen1+1;
                                                                               yyval.dtipo = yyvsp[-1].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 3123 "y.tab.c"
    break;

  case 120:
#line 1299 "ejemplosyntax.y"
                                                            {yyval.TamDimen2 = yyvsp[0].TamDimen1; 
                                                                               yyval.TamDimen1 = 1;
                                                                               yyval.dtipo = yyvsp[0].dtipo;
                                                                               yyval.dimensiones = 2;}
#line 3132 "y.tab.c"
    break;

  case 121:
#line 1362 "ejemplosyntax.y"
                      {yyval.lexema = yyvsp[0].lexema;
                        yyval.dimensiones=0; yyval.TamDimen1=0; yyval.TamDimen2=0;
                      }
#line 3140 "y.tab.c"
    break;

  case 122:
#line 1365 "ejemplosyntax.y"
                          { yyval.lexema = yyvsp[0].lexema; 
                            yyval.dimensiones=-1;
                            }
#line 3148 "y.tab.c"
    break;

  case 123:
#line 1368 "ejemplosyntax.y"
                          { yyval.lexema = yyvsp[0].lexema; 
                            yyval.dimensiones=-2;}
#line 3155 "y.tab.c"
    break;


#line 3159 "y.tab.c"

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
#line 1372 "ejemplosyntax.y"


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
    fflush(stderr);
    hayError = 1;
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

extern FILE *yyin ;
int yyparse(void) ;
FILE *abrir_entrada( int argc, char *argv[] )
{
    FILE *f= NULL ;
    if ( argc > 1 )
    {
        f= fopen(argv[1],"r");
        if (f==NULL)
        {
            fprintf(stderr,"fichero ’%s’ no encontrado\n",argv[1]);
            exit(1);
        }
        else
            printf("leyendo fichero ’%s’.\n",argv[1]);
    }
    else
        printf("leyendo entrada estándar.\n");
    return f ;
}
/************************************************************/
int main(){
  fMain = fopen("prog.c","w");
  fProc = fopen("dec_fun.h","w");

  yyout = fMain;
  yyparse();

  fclose(fMain);
  fclose(fProc);

  // CREO QUE POR ESTO SI PODEMOS HACER EL EXIT FAILURE
  if (hayError) {
    remove("prog.c");
    remove("dec_func.h");
  }

  return 0;
}
