principal() {
	ini_varlocal
		ent ve;
        ent ve2;
		real vf;
		car vc;
		bool vl ;
		ent pe[3], pe2[3,3],pe23[2,3],pe32[3,2] ;
		real pf[2], pf2[3,3] ;
		car pc[2], pc2[5,5] ;
		bool pl[2] ;
	fin_varlocal

	procedimiento procedimientoA(ent a1, real a2, car a3) {
		ini_varlocal
			ent x1, x2 ,a2;
		fin_varlocal

		procedimiento procedimientoB (car b1, bool b2) {
			ini_varlocal
				real xf, x2;
			fin_varlocal

			procedimiento procedimientoC (bool c1, ent c2) {
				ini_varlocal
					real x1 ;
				fin_varlocal
                
				x1= 1.3 +ve2;
                procedimientoA(2+3,3.5,'a');
                procedimientoC(1,2,3,4,5,6);
				si (()c2>10)+2) {
					c2= c2-1 ;
				} si_no {
					x1= 3.1 ;
				}
				mostrar x1 ;
			}

			procedimientoC(verdadero, 10);
			procedimientoC(falso, 1);
			x2 = x2-xf/10.0;
			mientras (xf-x2<10.0) {
				x2= x2*xf ;
			}
		}

		procedimiento procedimientoD (real d1) {
			ini_varlocal
				car dato ;
				ent valor ;
			fin_varlocal
			procedimiento procedimientoE (car e1, car e2) {
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
        mostrar "Fin Procedimiento A" ;
	}
	
    
	pe[0] = 10+ve;
	pf[0] = 10.0 +ve;
	pc[1] = '#' ;
	pl[0] = verdadero ;
    pe2=pe32**pe23;
    pe2=pe23**pe32;
    pe = pe**pe2;
    pe = pe - pe;    

}


