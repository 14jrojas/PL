#include <stdio.h>

/** escribe la descomposición de un numero entero en sus factores primos,
*** usa exclusivamente: multiplicacion, division y suma de enteros
**/
principal()
{
    ent n, curr = 2, ultim = 0, cuenta = 0, primero = 1 ;
    mostrar "introduce numero : ";
    leer n;
    mostrar n, " == ";
    curr = 2 ;
    mientras( curr <= n )
    {
        ent d = n/curr ;
        si ( d*curr == n ) /* curr divide a n */
        {
            si ( curr != ultim )
            {
                ultim = curr ;
                cuenta = 1 ;
            }
            si_no
                cuenta = cuenta + 1 ;

            n = n/curr ;
        }
        si_no /* curr no divide a ’n’ */
        {
            si ( cuenta > 0 )
            {
                si ( primero == 0 ) mostrar " *";
                primero = 0 ;
                mostrar curr;
                si ( cuenta > 1 ) mostrar cuenta;
            }
            curr = curr+1 ;
            cuenta = 0 ;
        }
    }
    si ( cuenta > 0 )
    {
        si ( primero == 0 ) mostrar " *";
        primero = 0 ;
        mostrar curr ;
        si ( cuenta > 1 ) mostrar "^", cuenta ;
    }
    mostrar "\n";
}