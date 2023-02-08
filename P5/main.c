/*********************************************************
**
** Fichero: MAIN.C
** Función: Pruebas de FLEX para practicas Proc.Leng
**
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
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
