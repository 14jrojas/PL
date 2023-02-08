principal()
{
    ini_varlocal
        ent n, d, curr ;
    fin_varlocal
    mostrar "introduce numero : ";
    leer n;
    mostrar n, " == ";
    curr = 2 ;
    mientras( curr <= n )
    {
        d = n/curr ;
        si ( d*curr == n ) 
        {
            mostrar "* ",curr;
            n = n/curr ;
        } si_no
            curr = curr+1 ;
    }
    mostrar "\n";
}