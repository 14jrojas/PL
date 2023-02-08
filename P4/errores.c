principal() {
	ini_varlocal
		ent ve ;
		real vf,rt,rs, re, r1 r6
		car vc, hm, rt rf ;
		b·ool vl ;
		ent pe[2], pe2[ve,ve] ;
		real pf[2], pf2[ve,ve] ;
		car pc[2], pc2[ve,ve] ;
		bool pl[2] ;
	fin_varlocal

	procedimientoA (ent a1, real a2, car a3) {
		ini_varlocal
			ent x1, x2 ;
		fin_varlocal

		procedimientoB (car bool, bool b2) {
			ini_varlocal
				real xf, x2 ;
			fin_varlocal

			procedimientoC (bool c1, ent c2) {
				ini_varlocal
					real x1 ;
				fin_varlocal

				x1= 1.3 ;
				si (c2>10) {
					c2= c2-1 ;
				} si_no {
					x1= 3.1 ;
				}
				mostrar x1 ;
			}

			procedimientoC (verdadero, 10)
			procedimientoC x1 (falso,1)
			xf rt = xf-x2/10.0;
			mientras (x2-xf<10.0) {
				x2= x2*xf ;
			}
        

		    procedimientoD (real d1) {
			    ini_varlocal
				    car dato ;
				    ent valor ;
			    fin_varlocal

                d = ((mat**mat)/2*3)<=(-(-5<=*3-5*6)+1) && || ((a+/1-15)%98) > 1;
                mat = *mat2**%mat2;

			    procedimientoE (car e1, car e2) {
                    mostrar "introduzca dos caracteres: " ;
				    leer e1, e2 ;
				    si (e1=='a') mostrar e1 ;
				    si_no 
					    si (e1=='b') mostrar e2 ;
					    si_no mostrar ' ' ;
			    }
            mostrar "introduzca un valor entero: " ;
		    leer valor ;
		    si (d1>0.0)	{
			    ini_varlocal
				    ent dato ;
			    fin_varlocal
			    dato= 2 ;
			    dato= valor*20/dato ;
		    } si_no {
			    valor= valor * 100 ;
			    d1= d1/1000.0 ;
		    }

		    mostrar d1 ;
            }
        }
	}

	pe[0] = 10 ;
	pf[0] = 10.0 ;
	pc[1] = '#' ;
	pl[0] = verdadero ;

	pe = pe ** pe ; 
	pf2 = pf2 ** pf2;

	elegir(ve){
		caso 2:
			mostrar "caso 2" ;
			salir;
		caso 3:
			mostrar "caso 3" ;
			salir;
		base:
			mostrar "predeterminado" ;
            salir ;
	}

	mostrar !verdadero ;

}


