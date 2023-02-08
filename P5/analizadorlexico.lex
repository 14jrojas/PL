%option yylineno
%{
    #include "y.tab.h"
%}

%option noyywrap
%option yylineno

char* msgError[256]

%%
[ \n\t] { ; }
"{"                         { return INICIOBLOQUE; }
"}"                         { return FINBLOQUE; }
"["                         { return CORCHETE_IZQ; }
"]"                         { return CORCHETE_DCHO; }
"("                         { return PAR_IZQUIERDO; }
")"                         { return PAR_DERECHO; }
";"                         { return PYC; }
","                         { return COMA; }
"="                         { return ASIGNACION; }
"leer"                      { return ENTRADA; }
"mostrar"                   { return SALIDA; }
"ini_varlocal"              { return INI_VARLOCAL; }
"fin_varlocal"              { return FIN_VARLOCAL; }
"principal"                 { return PRINCIPAL; }
"elegir"                    { return ELEGIR; }
"caso"                      { return CASO; }
"salir"                     { return SALIR; }
"ent"                       { yylval.atributo = 0; yylval.dtipo = entero; return TIPO_BASICOS; }
"bool"                      { yylval.atributo = 1; yylval.dtipo = booleano; return TIPO_BASICOS; }
"real"                      { yylval.atributo = 2; yylval.dtipo = real; return TIPO_BASICOS; }
"car"                       { yylval.atributo = 3; yylval.dtipo = caracter; return TIPO_BASICOS; }
"si"                        { return SI; }
"mientras"                  { return MIENTRAS; }
"si_no"                     { return SI_NO; }
"*"                         { yylval.atributo = 0; yylval.lexema = strdup(yytext); return OPMULDIV; }
"/"                         { yylval.atributo = 1; yylval.lexema = strdup(yytext); return OPMULDIV; }
"%"                         { yylval.atributo = 2; yylval.lexema = strdup(yytext); return OPMULDIV; }
"=="                        { yylval.atributo = 0; yylval.lexema = strdup(yytext); return OPIGUAL; }
"!="                        { yylval.atributo = 1; yylval.lexema = strdup(yytext); return OPIGUAL; }
">"                         { yylval.atributo = 0; yylval.lexema = strdup(yytext); return OPREL; }
">="                        { yylval.atributo = 1; yylval.lexema = strdup(yytext); return OPREL; }
"<"                         { yylval.atributo = 2; yylval.lexema = strdup(yytext); return OPREL; }
"<="                        { yylval.atributo = 3; yylval.lexema = strdup(yytext); return OPREL; }
"&&"                        { yylval.lexema = strdup(yytext); return OPAND; }
"||"                        { yylval.lexema = strdup(yytext); return OPOR; }
"^"                         { yylval.lexema = strdup(yytext); return OPSOR; }
"**"                        { yylval.lexema = strdup(yytext); return OPMAT; }
"+"                         { yylval.atributo = 0; yylval.lexema = strdup(yytext);  return MAS_MENOS; }
"-"                         { yylval.atributo = 1; yylval.lexema = strdup(yytext); return MAS_MENOS; }
"!"                         { yylval.lexema = strdup(yytext); return OPUNARIO; }
\"[^\"]*\"                  { yylval.lexema = strdup(yytext); return CADENA; }
"verdadero"                 { yylval.atributo = 0; yylval.dtipo = booleano; yylval.lexema = strdup(yytext); return VAL_LOGICOS; }
"falso"                     { yylval.atributo = 1; yylval.dtipo = booleano; yylval.lexema = strdup(yytext); return VAL_LOGICOS; }
"base"                      { return BASE; }
"procedimiento"             { return PROCEDIMIENTO; }
([0-9]+)                    { yylval.dtipo = entero; yylval.lexema = strdup(yytext); return ENTERO; }
([0-9]*\.[0-9]*)            { yylval.dtipo = real; yylval.lexema = strdup(yytext); return REAL; }
\'[^\']\'                   { yylval.dtipo = caracter; yylval.lexema = strdup(yytext); return CARACTER; }
[a-z|A-Z][a-z|A-Z|0-9|_]*   { yylval.lexema = strdup(yytext); return IDENT; }
":"                         { return DOSPUNTOS; }
<*>.|\n                     { printf("Error en la línea %d. Lexema %s no reconocible.\n", yylineno, yytext); }
%%

