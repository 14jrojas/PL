%{
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

// AÑADIDO GENERACION DE CODIGO INTERMEDIO
// REVISAR FUNCION PORQUE NOSOTROS NO TENEMOS OPERADORES @ ? #
TipoDato tipoOp(TipoDato td, char* op) {
  if (!strcmp(op, "+") || !strcmp(op, "-") || !strcmp(op, "*") || !strcmp(op, "/") ||
        !strcmp(op, "**") || !strcmp(op, "%"))
    return td;

  if (!strcmp(op, "!") || !strcmp(op, "&&") || !strcmp(op, "||") || !strcmp(op, ">") || !strcmp(op, "<") ||
        !strcmp(op, ">=") || !strcmp(op, "<=") || !strcmp(op, "!=") || !strcmp(op, "=="))
    return booleano;

  if (!strcmp(op, "#"))
    return entero;

  if (!strcmp(op, "?") || !strcmp(op, "@"))
    return tipoLista(td);
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
}

/*

______________GENERACION DE CODIGO INTERMEDIO___________________

*/

int hayError = 0;
int deep = 0;
FILE * fMain;
FILE * fProc;

#define addTab() {for (int i = 0; i < deep - (yyout != fMain); ++i) fprintf(yyout,"\t") }
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
  if (esArray(td))
    return "Array";
  else if (td == booleano)
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
  if (esArray(td2) && (!strcmp("+",op) || !strcmp("*",op))) {
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
  } else if (!strcmp("?",op)) {
    gen("%s = *(%s*)getActual(%s);\n", etiqueta, tipoIntermedio(tipoArray(td1)), exp1);
  } else if (!strcmp("@", op)) {
    gen("%s = *(%s*)get(%s, %s);\n", etiqueta, tipoIntermedio(tipoArray(td1)), exp1, exp2);
  } else if (!strcmp("--", op)) {
    gen("%s = borrarEn(%s, %s);\n", etiqueta, exp1, exp2);
  } else if (!strcmp("%", op)) {
    gen("%s = borrarAPartirDe(%s, %s);\n", etiqueta, exp1, exp2);
  } else if (!strcmp("**", op)) {
    gen("%s = concatenar(%s, %s);\n", etiqueta, exp1, exp2);
  } else if (esArray(tdPrimario)) {
    if (!strcmp("+", op)) {
      gen("%s = sumarArray(%s, %s);\n", etiqueta, expPrimaria, expSecundaria)
    } else if (!strcmp("-", op)) {
        gen("%s = restarArray(%s, %s);\n", etiqueta, expPrimaria, expSecundaria);
    } else if (!strcmp("*", op)) {
      gen("%s = multiplicarArray(%s, %s);\n", etiqueta, expPrimaria, expSecundaria);
    } else if (!strcmp("/", op)) {
      gen("%s = dividirArray(%s, %s);\n", etiqueta, expPrimaria, expSecundaria);
    }
  } else if (!strcmp("", exp2)) {
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
  else if (esArray(tipo) || tipo == booleano || tipo == cadena)
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

%}


// A continuación declaramos los nombres simbólicos de los tokens.
// byacc se encarga de asociar a cada uno un código.

%token PRINCIPAL
%token PROCEDIMIENTO
%token INICIOBLOQUE
%token FINBLOQUE
%token PYC
%token COMA
%token PAR_IZQUIERDO
%token PAR_DERECHO
%token INI_VARLOCAL
%token FIN_VARLOCAL
%token TIPO_BASICOS
%token ASIGNACION
%token CORCHETE_IZQ
%token CORCHETE_DCHO
%token SI
%token SI_NO
%token MIENTRAS
%token ENTRADA
%token SALIDA
%token ELEGIR
%token CASO
%token SALIR
%token VAL_LOGICOS
%token ENTERO
%token REAL
%token CARACTER
%token CADENA
%token IDENT
%token BASE
%token DOSPUNTOS

%left OPOR
%left OPAND
%left OPSOR
%left OPIGUAL
%left OPREL
%left MAS_MENOS
%left OPMAT
%left OPMULDIV
%right OPUNARIO

// _________HAY QUE AÑADIR LAS COSAS DE GENERACION DE CODIGO INTERMEDIO__________

%%
programa : PRINCIPAL PAR_IZQUIERDO PAR_DERECHO bloque ;

bloque : INICIOBLOQUE { insertarMarca(); }
    Declar_de_variables_locales
    Declar_de_subprogs
    Sentencias
    FINBLOQUE { vaciarEntradas(); } ;

Declar_de_subprogs : Declar_de_subprogs Declar_subprog 
| ;

Declar_subprog : PROCEDIMIENTO IDENT { insertarProc($2.lexema); } PAR_IZQUIERDO lista_expresiones PAR_DERECHO { subProg = 1; } bloque ;


Declar_de_variables_locales : INI_VARLOCAL variables_locales FIN_VARLOCAL | ;


variables_locales : variables_locales cuerpo_declar_variables
                | cuerpo_declar_variables ;

cuerpo_declar_variables : TIPO_BASICOS { tipoTmp = $1.dtipo; } lista_identificadores PYC | error ;

lista_identificadores : lista_identificadores COMA identificador { insertarVariable($3.lexema,$3.dimensiones,$3.TamDimen1,$3.TamDimen2); } 
            | identificador { insertarVariable($1.lexema,$1.dimensiones,$1.TamDimen1,$1.TamDimen2); } ;

Tipo_vector : IDENT CORCHETE_IZQ ENTERO CORCHETE_DCHO { {$$.lexema = $1.lexema; 
                                                              $$.dimensiones=1;
                                                              $$.TamDimen1=atoi($3.lexema);
                                                              $$.TamDimen2=0;}} ;

Tipo_matriz : IDENT CORCHETE_IZQ ENTERO COMA ENTERO CORCHETE_DCHO {$$.lexema = $1.lexema;
                                                                          $$.dimensiones=2;
                                                                          $$.TamDimen1=atoi($3.lexema);
                                                                          $$.TamDimen2=atoi($5.lexema);};

Sentencias : Sentencias Sentencia
                | Sentencia;

Sentencia : bloque
                | sentencia_asignacion
                | sentencia_if
                | sentencia_while
                | sentencia_entrada
                | sentencia_salida
                | sentencia_proc 
                | sentencia_case_switch ;


sentencia_asignacion : identificador ASIGNACION expresion PYC {comprobarAsignacion($1.lexema, $3); } | error;

sentencia_if : SI PAR_IZQUIERDO expresion PAR_DERECHO Sentencia { expresionBooleana($3.dtipo); }
             |  SI PAR_IZQUIERDO expresion PAR_DERECHO Sentencia SI_NO Sentencia { expresionBooleana($3.dtipo); } ;


sentencia_while : MIENTRAS PAR_IZQUIERDO expresion PAR_DERECHO Sentencia { expresionBooleana($3.dtipo); } ;

sentencia_entrada : ENTRADA lista_entrada PYC ;

sentencia_salida : SALIDA lista_expresiones_o_cadena PYC ;

lista_entrada : lista_entrada COMA identificador
              | identificador ;

lista_expresiones_o_cadena : lista_expresiones_o_cadena COMA expresion
                | lista_expresiones_o_cadena COMA CADENA
                | expresion
                | CADENA ;
sentencia_case_switch : ELEGIR expresion INICIOBLOQUE Casos FINBLOQUE ;
    
Casos : Casos Bloque_de_caso | Bloque_de_caso;

Bloque_de_caso : CASO constante DOSPUNTOS Sentencias SALIR PYC 
            | CASO constante DOSPUNTOS Sentencias 
            | BASE DOSPUNTOS Sentencias SALIR PYC
            | BASE Sentencias ;

sentencia_proc : IDENT  PAR_IZQUIERDO argumentos PAR_DERECHO PYC { comprobarProcedimiento($1.lexema);} ;

argumentos : lista_argumentos
           | ;

lista_argumentos : lista_argumentos COMA expresion {
                  argumentos_tipo_datos[n_argumentos] = $3.dtipo;
                  n_argumentos++;
                    
                  }
                 | expresion {
                   argumentos_tipo_datos[n_argumentos] = $1.dtipo;
                    n_argumentos++;
                   
                  } ;


lista_expresiones : lista_expresiones COMA expresion 
              | expresion 
| ;

// AQUI OPUNARIO = EXCL
expresion : PAR_IZQUIERDO expresion PAR_DERECHO { $$.dtipo = $2.dtipo; }
                | OPUNARIO expresion { $$.dtipo = excl($2.dtipo); }
                | MAS_MENOS expresion %prec OPUNARIO { $$.dtipo = masMenos($1.atributo, $2.dtipo); }
                | expresion OPMULDIV expresion { $$.dtipo = porDiv($1, $2.atributo, $3); }
                | expresion OPIGUAL expresion { $$.dtipo = eqn($1.dtipo, $2.atributo, $3.dtipo); }
                | expresion OPREL expresion { $$.dtipo = rel($1.dtipo, $2.atributo, $3.dtipo); }
                | expresion OPAND expresion { $$.dtipo = andLog($1.dtipo, $3.dtipo); }
                | expresion OPOR expresion { $$.dtipo = orLog($1.dtipo, $3.dtipo); }
                | expresion OPSOR expresion  { $$.dtipo = xorLog($1.dtipo, $3.dtipo); }
                | expresion OPMAT expresion { $$.dtipo = porPor($1, $3); 
                                              $$.TamDimen1 = $1.TamDimen1;
                                              $$.TamDimen2 = $3.TamDimen2;    }
                | expresion MAS_MENOS expresion { $$.dtipo = addSub($1, $2.atributo, $3); }
                | identificador { $$ = entradaAAtributos(buscarID($1.lexema)); }
                | constante  { $$.dtipo = $1.dtipo; }
                | TIPO_BASICOS identificador { insertarParametro($1.dtipo, $2.lexema); } 
    		| error; 



constante : ENTERO { $$.dtipo = $1.dtipo; }
        | REAL { $$.dtipo = $1.dtipo; }
        | VAL_LOGICOS { $$.dtipo = $1.dtipo; }
        | CARACTER { $$.dtipo = $1.dtipo; }
// NO SE SI VECTORES Y MATRIZ SE HACEN ASI
        | vector { $$ = $1; }
        | matriz  { $$ = $1; } ;

vector : CORCHETE_IZQ lista_enteros CORCHETE_DCHO { $$.dtipo = $2.dtipo;
                                                    if ($2.dimensiones==1){$$.TamDimen2 = 0; $$.TamDimen1 = $2.TamDimen1; $$.dimensiones = 1;}}
        | CORCHETE_IZQ lista_caracteres CORCHETE_DCHO { $$.dtipo = $2.dtipo;
                                                    if ($2.dimensiones==1){$$.TamDimen2 = 0; $$.TamDimen1 = $2.TamDimen1; $$.dimensiones = 1; }}
        | CORCHETE_IZQ lista_booleanos CORCHETE_DCHO {$$.dtipo = $2.dtipo;
                                                    if ($2.dimensiones==1){$$.TamDimen2 = 0; $$.TamDimen1 = $2.TamDimen1; $$.dimensiones = 1; }}
        | CORCHETE_IZQ lista_reales CORCHETE_DCHO { $$.dtipo = $2.dtipo;
                                                    if ($2.dimensiones==1){$$.TamDimen2 = 0; $$.TamDimen1 = $2.TamDimen1; $$.dimensiones = 1; }} ;

lista_enteros : lista_enteros COMA ENTERO {$$.dtipo = $3.dtipo;
                                           $$.dimensiones = 1;
                                           $$.TamDimen1 = $1.TamDimen1+1;}
            | ENTERO {$$.dtipo = $1.dtipo;
                      $$.dimensiones = 1;
                      $$.TamDimen1 = 1;} ;

lista_caracteres : lista_caracteres COMA CARACTER {$$.dtipo = $3.dtipo;
                                           $$.dimensiones = 1;
                                           $$.TamDimen1 = $1.TamDimen1+1;}
            | CARACTER {$$.dtipo = $1.dtipo;
                      $$.dimensiones = 1;
                      $$.TamDimen1 = 1;};

lista_booleanos : lista_booleanos COMA VAL_LOGICOS {$$.dtipo = $3.dtipo;
                                           $$.dimensiones = 1;
                                           $$.TamDimen1 = $1.TamDimen1+1;}
            | VAL_LOGICOS {$$.dtipo = $1.dtipo;
                      $$.dimensiones = 1;
                      $$.TamDimen1 = 1;} ;

lista_reales : lista_reales COMA REAL {$$.dtipo = $3.dtipo;
                                           $$.dimensiones = 1;
                                           $$.TamDimen1 = $1.TamDimen1+1;}
            | REAL {$$.dtipo = $1.dtipo;
                      $$.dimensiones = 1;
                      $$.TamDimen1 = 1;};

matriz : INICIOBLOQUE matriz_enteros FINBLOQUE { $$.dtipo = $2.dtipo; 
                                                 $$.dimensiones = $2.dimensiones;
                                                 $$.TamDimen1 = $2.TamDimen1;
                                                 $$.TamDimen2 = $2.TamDimen2;}
            | INICIOBLOQUE matriz_caracteres FINBLOQUE { $$.dtipo = $2.dtipo; 
                                                 $$.dimensiones = $2.dimensiones;
                                                 $$.TamDimen1 = $2.TamDimen1;
                                                 $$.TamDimen2 = $2.TamDimen2;}
            | INICIOBLOQUE matriz_booleanos FINBLOQUE { $$.dtipo = $2.dtipo; 
                                                 $$.dimensiones = $2.dimensiones;
                                                 $$.TamDimen1 = $2.TamDimen1;
                                                 $$.TamDimen2 = $2.TamDimen2;}
            | INICIOBLOQUE matriz_reales FINBLOQUE { $$.dtipo = $2.dtipo; 
                                                 $$.dimensiones = $2.dimensiones;
                                                 $$.TamDimen1 = $2.TamDimen1;
                                                 $$.TamDimen2 = $2.TamDimen2;} ;

/* ESTA VERSION NO COMPRUEBA SI LAS LISTAS DENTRO DE LA MISMA MATRIZ TIENEN LA MISMA DIMENSION (CREO), SOLO MIRA O LA DE LA ULTIMA O LA PRIMERA*/
matriz_enteros : matriz_enteros COMA CORCHETE_IZQ lista_enteros CORCHETE_DCHO {$$.TamDimen2 = $4.TamDimen1; 
                                                                               $$.TamDimen1 = $1.TamDimen1+1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2;}    
                  | CORCHETE_IZQ lista_enteros CORCHETE_DCHO {$$.TamDimen2 = $3.TamDimen1; 
                                                                               $$.TamDimen1 = 1;
                                                                               $$.dtipo = $3.dtipo;
                                                                               $$.dimensiones = 2;};

matriz_caracteres : matriz_caracteres COMA  CORCHETE_IZQ lista_caracteres CORCHETE_DCHO {$$.TamDimen2 = $4.TamDimen1; 
                                                                               $$.TamDimen1 = $1.TamDimen1+1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2;}   
                  | CORCHETE_IZQ lista_caracteres CORCHETE_DCHO {$$.TamDimen2 = $3.TamDimen1; 
                                                                               $$.TamDimen1 = 1;
                                                                               $$.dtipo = $3.dtipo;
                                                                               $$.dimensiones = 2;};

matriz_booleanos : matriz_booleanos COMA  CORCHETE_IZQ lista_booleanos CORCHETE_DCHO {$$.TamDimen2 = $4.TamDimen1; 
                                                                               $$.TamDimen1 = $1.TamDimen1+1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2;}
                  | CORCHETE_IZQ lista_booleanos CORCHETE_DCHO {$$.TamDimen2 = $3.TamDimen1; 
                                                                               $$.TamDimen1 = 1;
                                                                               $$.dtipo = $3.dtipo;
                                                                               $$.dimensiones = 2;};

matriz_reales : matriz_reales COMA  CORCHETE_IZQ lista_reales CORCHETE_DCHO {$$.TamDimen2 = $4.TamDimen1; 
                                                                               $$.TamDimen1 = $1.TamDimen1+1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2;}   
                  | CORCHETE_IZQ lista_reales CORCHETE_DCHO {$$.TamDimen2 = $3.TamDimen1; 
                                                                               $$.TamDimen1 = 1;
                                                                               $$.dtipo = $3.dtipo;
                                                                               $$.dimensiones = 2;};


/* ESTA VERSION CREO QUE SI LO COMPRUEBA. SI NO LO HACE, TENEMOS QUE MIRAR EN COMO SE GUARDA LA DIMENSION DE LA MATRIZ Y LAS COMPROBACIONES CON LOS VECTORES QUE METEMOS

matriz_enteros : matriz_enteros COMA CORCHETE_IZQ lista_enteros CORCHETE_DCHO {if($1.TamDimen2 == $4.TamDimen1){$$.TamDimen2 == $4.TamDimen1}else{
                                                                                                                                                sprintf(msgError, "ERROR SEMÁNTICO: las filas de la matriz tienen distinta dimensión: %i %i\n",
                                                                                                                                                    $1.TamDimen1, $4.TamDimen1);
                                                                                                                                                yyerror(msgError);
                                                                                                                                              }; 
                                                                               $$.TamDimen1 = $1.TamDimen1+1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2}    
                  | CORCHETE_IZQ lista_enteros CORCHETE_DCHO {$$.TamDimen2 = $4.TamDimen1; 
                                                                               $$.TamDimen1 = 1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2};

matriz_caracteres : matriz_caracteres COMA  CORCHETE_IZQ lista_caracteres CORCHETE_DCHO {if($1.TamDimen2 == $4.TamDimen1){$$.TamDimen2 == $4.TamDimen1}else{
                                                                                                                                                sprintf(msgError, "ERROR SEMÁNTICO: las filas de la matriz tienen distinta dimensión: %i %i\n",
                                                                                                                                                    $1.TamDimen1, $4.TamDimen1);
                                                                                                                                                yyerror(msgError);
                                                                                                                                              }; 
                                                                               $$.TamDimen1 = $1.TamDimen1+1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2}   
                  | CORCHETE_IZQ lista_caracteres CORCHETE_DCHO {$$.TamDimen2 = $4.TamDimen1; 
                                                                               $$.TamDimen1 = 1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2};

matriz_booleanos : matriz_booleanos COMA  CORCHETE_IZQ lista_booleanos CORCHETE_DCHO {if($1.TamDimen2 == $4.TamDimen1){$$.TamDimen2 == $4.TamDimen1}else{
                                                                                                                                                sprintf(msgError, "ERROR SEMÁNTICO: las filas de la matriz tienen distinta dimensión: %i %i\n",
                                                                                                                                                    $1.TamDimen1, $4.TamDimen1);
                                                                                                                                                yyerror(msgError);
                                                                                                                                              }; 
                                                                               $$.TamDimen1 = $1.TamDimen1+1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2}
                  | CORCHETE_IZQ lista_booleanos CORCHETE_DCHO {$$.TamDimen2 = $4.TamDimen1; 
                                                                               $$.TamDimen1 = 1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2};

matriz_reales : matriz_reales COMA  CORCHETE_IZQ lista_reales CORCHETE_DCHO {if($1.TamDimen2 == $4.TamDimen1){$$.TamDimen2 == $4.TamDimen1}else{
                                                                                                                                                sprintf(msgError, "ERROR SEMÁNTICO: las filas de la matriz tienen distinta dimensión: %i %i\n",
                                                                                                                                                    $1.TamDimen1, $4.TamDimen1);
                                                                                                                                                yyerror(msgError);
                                                                                                                                              }; 
                                                                               $$.TamDimen1 = $1.TamDimen1+1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2}   
                  | CORCHETE_IZQ lista_reales CORCHETE_DCHO {$$.TamDimen2 = $4.TamDimen1; 
                                                                               $$.TamDimen1 = 1;
                                                                               $$.dtipo = $4.dtipo;
                                                                               $$.dimensiones = 2};

*/

identificador : IDENT {$$.lexema = $1.lexema;
                        $$.dimensiones=0; $$.TamDimen1=0; $$.TamDimen2=0;
                      }
            | Tipo_vector { $$.lexema = $1.lexema; 
                            $$.dimensiones=-1;
                            }
            | Tipo_matriz { $$.lexema = $1.lexema; 
                            $$.dimensiones=-2;};


%%

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

int main(){
  fMain = fopen("prog.c","w");
  fFunc = fopen("dec_fun.h","w");

  yyout = fMain;
  yyparse();

  fclose(fMain);
  fclose(fFunc);

  // CREO QUE POR ESTO SI PODEMOS HACER EL EXIT FAILURE
  if (hayError) {
    remove("prog.c");
    remove("dec_func.h");
  }

  return 0;
}
