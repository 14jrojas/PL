%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

// La siguiente declaracion permite que ’yyerror’ se pueda invocar desde el
// fuente de lex (prueba.l)

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

// Se debe implementar la función yyerror. En este caso simplemente escribimos
// el mensaje de error en pantalla
void yyerror( char *msg ){
    fprintf(stderr, "Línea %d: %s\n", yylineno, msg) ;
}

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
  arrayEntero,
  arrayReal,
  arrayBooleano,
  arrayCaracter,
  error // Si da un error con expresiones
} TipoDato;

typedef struct {
  TipoEntrada tipoEntrada;    // Tipo de entrada
  char* nombre;               // Nombre del identificador (no se usa con marca)
  TipoDato tipoDato;          // Tipo de dato
  int parametros;             // Nº de parámetros formales (sólo se usa con función)
} EntradaTS;

// La Tabla de Símbolos
EntradaTS ts[MAX_TAM_TS];

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
    case arrayReal:
      return "array real";
    case arrayEntero:
      return "array ent";
    case arrayCaracter:
      return "array car";
    case arrayBooleano:
      return "array bool";
    case error:
      return "error";
    default:
      fprintf(stderr, "Error en tipoAString(), no se conoce el tipo dato\n");
      exit(EXIT_FAILURE);
  }
}

TipoDato tipoArray(TipoDato tipo_dato) {
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
}

TipoDato aTipoArray(TipoDato td) {
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
}

int esArray(TipoDato tipo_dato){
  return tipo_dato == arrayEntero || tipo_dato == arrayReal || tipo_dato == arrayBooleano || tipo_dato == arrayCaracter;
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
        printf("Procedimiento %s, nº parametros: %i\n", ts[i].nombre,
            tipoAString(ts[i].tipoDato), ts[i].parametros);
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
  }
}

void idRepetida(char* id) {
  // Miramos si id estaba declarado después de la última marca
  int repetida = 0;
  for (int i = tope; !repetida && ts[i].tipoEntrada != marca; --i) {
    if (ts[i].tipoEntrada != parametroFormal && !strcmp(ts[i].nombre, id)) {
      sprintf(msgError, "ERROR SINTÁCTICO: identificador %s ya declarado\n", id);
      yyerror(msgError);
      repetida = 1;
    }
  }
}

void insertarEntrada(TipoEntrada te, char* nombre, TipoDato tipo_dato, int nParam) {
  // Hacemos la entrada
  EntradaTS entrada = {
    te,
    strdup(nombre),
    tipo_dato,
    nParam
  };

  // Si la tabla está llena da error
  if (tope + 1 >= MAX_TAM_TS) {
    sprintf(msgError, "ERROR SINTÁCTICO: La tabla de símbolos está llena\n");
    yyerror(msgError);
  }
  // Aumentamos el tope
  ++tope;
  // Añadimos la nueva entrada
  ts[tope] = entrada;
}

// Busca una entrada en la TS con el id especificado en el ámbito del programa
// actual. Si no lo encuentra, devuelve -1. No gestiona errores!
int buscarEntrada(char* id) {
  int i = tope;
  while(i >= 0 && (ts[i].tipoEntrada == parametroFormal || strcmp(id, ts[i].nombre)))
    --i;

  if (i < 0) {
    sprintf(msgError, "ERROR SINTÁCTICO: identificador %s no declarado\n", id);
    yyerror(msgError);
  }
  return i;
}


void insertarMarca() {
  insertarEntrada(marca, "", -1, -1);
  // Si es subprograma añadimos las variables al bloque
  if (subProg) {
    for (int i = tope - 1; ts[i].tipoEntrada != procedimiento; --i) {
      insertarEntrada(variable, ts[i].nombre, ts[i].tipoDato, -1);
    }
    subProg = 0;
  }
}

void vaciarEntradas() {
  while (ts[tope].tipoEntrada != marca)
    --tope;
  --tope;
}

void insertarVariable(char* id) {
  // Comprobamos que no esté repetida la id
  idRepetida(id);
  insertarEntrada(variable, id, tipoTmp, -1);
}

void insertarProc(TipoDato tipoDato, char* id) {
  // Comprobamos que el id no esté usado ya
  idRepetida(id);
  insertarEntrada(procedimiento, id, tipoDato, 0);
}

void insertarParametro(TipoDato tipoDato, char* id) {
  // Comprobamos que no haya parámetros con nombres repetidos
  // Además guardamos el índice de la función
  int i;
  int parametroRepetido = 0;
  for (i = tope; !parametroRepetido && ts[i].tipoEntrada != procedimiento; --i) {
    if (!strcmp(ts[i].nombre, id)) {
      sprintf(msgError, "ERROR SINTÁCTICO: identificador del parámetro %s ya declarado\n", id);
      yyerror(msgError);
      parametroRepetido = 1;
    }
  }
  // Añadimos la entrada
  insertarEntrada(parametroFormal, id, tipoDato, -1);
  // Actualizamos el nº de parámetros de la función
  ++ts[i].parametros;
}

TipoDato buscarID(char* id) {
  int i = buscarEntrada(id);

  if (i < 0)
    return error;
  return ts[i].tipoDato;
}

void comprobarAsignacion(char* id, TipoDato td) {
  int i = buscarEntrada(id);
  if (i >= 0) {
    if (ts[i].tipoEntrada != variable) {
      sprintf(msgError, "ERROR SINTÁCTICO: se intenta asignar a %s, y no es una variable\n", id);
      yyerror(msgError);
    } else {
      if (td != error && td != ts[i].tipoDato) {
        sprintf(msgError, "ERROR SINTÁCTICO: asignación incorrecta, %s es tipo %s y se obtuvo %s\n",
            id, tipoAString(td), tipoAString(td));
        yyerror(msgError);
      }
    }
  }
}

void expresionBooleana(TipoDato td) {
  if (td != error && td != booleano) {
    sprintf(msgError, "ERROR SINTÁCTICO: condición no es de tipo booleano, se tiene tipo %s",
        tipoAString(td));
    yyerror(msgError);
  }
}

void sentenciaArray(TipoDato td, char* sentencia) {
  if (td != error && !esArray(td)) {
    sprintf(msgError, "ERROR SINTÁCTICO: sentencia %s no aplicable al tipo %s\n",
        sentencia, tipoAString(td));
    yyerror(msgError);
  }
}

TipoDato mismoTipoArray(TipoDato t1, TipoDato t2) {

  if (t1 == error || t2 == error)
    return error;

  if (t1 != t2) {
    sprintf(msgError, "ERROR SINTÁCTICO: array con dos tipos distintos %s y %s\n",
        tipoAString(t1), tipoAString(t2));
    yyerror(msgError);
    return error;
  }
  return t1;
}

TipoDato masMenos(int atr, TipoDato td) {
  if (td == error)
    return error;

  char* operador = atr ? "-" : "+";
  if (!esNumero(td)) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable al tipo %s\n",
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
    sprintf(msgError, "ERROR SINTÁCTICO: operador ! no aplicable al tipo %s\n",
        tipoAString(td));
    yyerror(msgError);
    return error;
  }
  return booleano;
}

TipoDato intHash(int atr, TipoDato td) {
  if (td == error)
    return error;

  char* operador = atr ? "#" : "?";
  if (!esArray(td)) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable al tipo %s\n",
        operador, tipoAString(td));
    yyerror(msgError);
    return error;
  }

  if (atr)
    return tipoArray(td);
  else
    return entero;
}

TipoDato at(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (!esArray(td1) || td2 != entero) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador @ no aplicable a los tipos %s, %s\n",
        tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return tipoArray(td1);
}

TipoDato andLog(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (td1 != booleano || td2 != booleano) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador && no aplicable a los tipos %s, %s\n",
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
    sprintf(msgError, "ERROR SINTÁCTICO: operador || no aplicable a los tipos %s, %s\n",
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
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable a los tipos %s, %s\n",
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
TipoDato op2Binario(TipoDato td1, int atr, TipoDato td2, char* op1, char* op2) {
  if (td1 == error || td2 == error)
    return error;

  char* operador = atr ? op1 : op2;
  int l1 = esArray(td1);
  int l2 = esArray(td2);
  TipoDato tipo1 = l1 ? tipoArray(td1) : td1;
  TipoDato tipo2 = l2 ? tipoArray(td2) : td2;

  int op_error = (l1 && l2) ||
              (tipo1 != tipo2) ||
              !esNumero(tipo1);

  TipoDato resultado = td1;

  if (!op_error && (l1 || l2) ) {
    // Llegado a este punto hay exactamente una lista. Vemos si el tipo de
    // la lista encaja con el tipo del otro atributo:
    if ( (operador == op2) || l1 ) {
      resultado = l1 ? td1 : td2;
    } else {
      op_error = 1;
    }
  }

  if (op_error) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }
  return resultado;
}

TipoDato addSub(TipoDato td1, int atr, TipoDato td2) {
  return op2Binario(td1, atr, td2, "-", "+");
}

TipoDato porDiv(TipoDato td1, int atr, TipoDato td2) {
  return op2Binario(td1, atr, td2, "/", "*");
}

TipoDato porPor(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (td1 != td2 || !esArray(td1) || !esArray(td2)) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador ** no aplicable a los tipos %s, %s\n",
        tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return td1;
}

TipoDato borrArray(TipoDato td1, int atr, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  char* operador = atr ? "%" : "--";
  if (!esArray(td1) || td2 != entero) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return td1;
}

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
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

TipoDato ternario(TipoDato td1, TipoDato td2, TipoDato td3) {
  if (td1 == error || td2 == error || td3 == error)
    return error;

  if (!esArray(td1) || tipoArray(td1) != td2 || td3 != entero) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador ++ @ no aplicable a los tipos %s, %s, %s\n",
        yylineno, tipoAString(td1), tipoAString(td2), tipoAString(td3));
    yyerror(msgError);
    return error;
  }
  return td1;
}

TipoDato comprobarProcedimiento(char* id) {
  int iProc = buscarEntrada(id);

  if (iProc < 0)
    return error;

  if (ts[iProc].tipoEntrada != procedimiento) {
    sprintf(msgError, "ERROR SINTÁCTICO: %s no es un procedimiento y no puede ser llamado\n", id);
    yyerror(msgError);
    return error;
  }

  int n_argumentos_esperados = ts[iProc].parametros;

  if ( n_argumentos != n_argumentos_esperados ) {
    sprintf(msgError, "ERROR SINTÁCTICO: número de argumentos errónea al llamar al procedimiento %s. Esperados: %d, encontrados: %d\n",
        id, n_argumentos_esperados, n_argumentos);
    yyerror(msgError);
  }

  n_argumentos = min(n_argumentos, n_argumentos_esperados);

  for (int i = 0; i < n_argumentos; i++){
    TipoDato tipoEsperado = ts[iProc + i + 1].tipoDato;
    TipoDato tipoObtenido = argumentos_tipo_datos[i];
    if (tipoObtenido != error && tipoEsperado != tipoObtenido){
      sprintf(msgError, "ERROR SINTÁCTICO: argumento número %d de tipo erróneo al llamar al procedimiento %s. Esperado: %s, encontrado: %s\n",
          i, id, tipoAString(tipoEsperado), tipoAString(tipoObtenido));
      yyerror(msgError);
    }
  }

  // De esta forma mostramos el máximo número de errores posibles.

  // Borramos los argumentos recibidos.
  n_argumentos = 0;

  // Devolvemos el tipo de la función.
  return ts[iProc].tipoDato;
}

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


%%
programa : PRINCIPAL PAR_IZQUIERDO PAR_DERECHO bloque ;

bloque : INICIOBLOQUE { insertarMarca(); }
    Declar_de_variables_locales
    Declar_de_subprogs
    Sentencias
    FINBLOQUE { vaciarEntradas(); } ;

Declar_de_subprogs : Declar_de_subprogs Declar_subprog 
| ;

Declar_subprog : PROCEDIMIENTO sentencia_proc { subProg = 1; } bloque ;

Declar_de_variables_locales : INI_VARLOCAL variables_locales FIN_VARLOCAL | ;


variables_locales : variables_locales cuerpo_declar_variables
                | cuerpo_declar_variables ;

cuerpo_declar_variables : TIPO_BASICOS { tipoTmp = $1.dtipo; } lista_identificadores PYC | error ;

lista_identificadores : lista_identificadores COMA identificador { insertarVariable($1.lexema); } 
            | identificador { insertarVariable($1.lexema); } ;

Tipo_vector : IDENT CORCHETE_IZQ expresion CORCHETE_DCHO ;

Tipo_matriz : IDENT CORCHETE_IZQ expresion COMA expresion CORCHETE_DCHO ;

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


sentencia_asignacion : identificador ASIGNACION expresion PYC {comprobarAsignacion($1.lexema, $3.dtipo); } | error;

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

sentencia_proc : IDENT  PAR_IZQUIERDO lista_expresiones PAR_DERECHO { insertarProc($1.dtipo, $2.lexema); } ;

lista_expresiones : lista_expresiones COMA expresion 
              | expresion 
| ;

// AQUI OPUNARIO = EXCL
expresion : PAR_IZQUIERDO expresion PAR_DERECHO { $$.dtipo = $2.dtipo; }
                | OPUNARIO expresion { $$.dtipo = excl($2.dtipo); }
                | MAS_MENOS expresion %prec OPUNARIO { $$.dtipo = masMenos($1.atributo, $2.dtipo); }
                | expresion OPMULDIV expresion { $$.dtipo = porDiv($1.dtipo, $2.atributo, $3.dtipo); }
                | expresion OPIGUAL expresion { $$.dtipo = eqn($1.dtipo, $2.atributo, $3.dtipo); }
                | expresion OPREL expresion { $$.dtipo = rel($1.dtipo, $2.atributo, $3.dtipo); }
                | expresion OPAND expresion { $$.dtipo = andLog($1.dtipo, $3.dtipo); }
                | expresion OPOR expresion { $$.dtipo = orLog($1.dtipo, $3.dtipo); }
// AQUI FALTA OPERADOR XOR QUE NO LO HE ENCONTRADO
                | expresion OPSOR expresion 
                | expresion OPMAT expresion { $$.dtipo = porPor($1.dtipo, $3.dtipo); }
                | expresion MAS_MENOS expresion { $$.dtipo = addSub($1.dtipo, $2.atributo, $3.dtipo); }
                | identificador { $$.dtipo = buscarID($1.lexema); }
                | constante  { $$.dtipo = $1.dtipo; }
                | TIPO_BASICOS identificador { insertarParametro($1.dtipo, $2.lexema); } 
    | error; 


constante : ENTERO { $$.dtipo = $1.dtipo; }
        | REAL { $$.dtipo = $1.dtipo; }
        | VAL_LOGICOS { $$.dtipo = $1.dtipo; }
        | CARACTER { $$.dtipo = $1.dtipo; }
// NO SE SI VECTORES Y MATRIZ SE HACEN ASI
        | vector { $$.dtipo = $1.dtipo; }
        | matriz { $$.dtipo = $1.dtipo; } ;

vector : CORCHETE_IZQ lista_enteros CORCHETE_DCHO { $$.dtipo = aTipoArray($2.dtipo); }
        | CORCHETE_IZQ lista_caracteres CORCHETE_DCHO { $$.dtipo = aTipoArray($2.dtipo); }
        | CORCHETE_IZQ lista_booleanos CORCHETE_DCHO { $$.dtipo = aTipoArray($2.dtipo); }
        | CORCHETE_IZQ lista_reales CORCHETE_DCHO { $$.dtipo = aTipoArray($2.dtipo); } ;

lista_enteros : lista_enteros COMA ENTERO
            | ENTERO ;

lista_caracteres : lista_caracteres COMA CARACTER
            | CARACTER ;

lista_booleanos : lista_booleanos COMA VAL_LOGICOS
            | VAL_LOGICOS ;

lista_reales : lista_reales COMA REAL
            | REAL ;

matriz : INICIOBLOQUE matriz_enteros FINBLOQUE
            | INICIOBLOQUE matriz_caracteres FINBLOQUE
| INICIOBLOQUE matriz_booleanos FINBLOQUE
| INICIOBLOQUE matriz_reales FINBLOQUE ;

matriz_enteros : matriz_enteros COMA  CORCHETE_IZQ lista_enteros CORCHETE_DCHO    | CORCHETE_IZQ lista_enteros CORCHETE_DCHO ;

matriz_caracteres : matriz_caracteres COMA  CORCHETE_IZQ lista_caracteres CORCHETE_DCHO    | CORCHETE_IZQ lista_caracteres CORCHETE_DCHO ;

matriz_booleanos : matriz_booleanos COMA  CORCHETE_IZQ lista_booleanos CORCHETE_DCHO    | CORCHETE_IZQ lista_booleanos CORCHETE_DCHO ;

matriz_reales : matriz_reales COMA  CORCHETE_IZQ lista_reales CORCHETE_DCHO    | CORCHETE_IZQ lista_reales CORCHETE_DCHO ;

identificador : IDENT 
            | Tipo_vector
            | Tipo_matriz ;


%%

// Aqui incluimos el fichero generado por el ’lex’ que implementa la función
// ’yylex’

#ifdef DOSWINDOWS /* Variable de entorno que indica la plataforma */
#include "lexyy.c"
#else
#include "lex.yy.c"
#endif



