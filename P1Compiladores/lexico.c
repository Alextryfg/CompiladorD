#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "definiciones.h"
#include "tabladesimbolos.h"
#include "sistemaEntrada.h"
#include "abb.h"

/*
 * Automatas a definir:
 *
 *  integers:
 *  identificadores:
 *  keywords:
 *  Strings:
 *  floats:
 *  Operadores:
 *
 */

void identificadores( *comp){

}


/*
 * Funcion encargada de devolver el componente lexico que será procesado por el Analizador sintáctico
 */
void siguiente_componente_lexico(tipoelem *comp){
    int accept = 0;
    int a = 0;
    int state = 0;
    char c;

    //Mientras el lexema no sea aceptado
    while(!accept){
        switch (state){
            case 0:
                c = siguiente_caracter();
                if(c == ' ' || c == '\t'){

                }else if (isalpha(c) || c == '_'){
                    state = 2;
                    break;

                }else if (isdigit(c)){

                }
            case 1:
                /*
                 * Identifiers start with:
                 * A letter, _, or universal alpha, and are followed by any number of letters, _, digits, or universal alphas
                 */

                //Se obtiene el siguiente caracter
                c = siguiente_caracter();
                //Se comprueba que cumpla las condiciones de D
                while(isalpha(c) || isdigit(c) || c == '_'){
                    c = siguiente_caracter();
                }
                //Salgo del while, y por lo tanto el lexema esta formado

                //TODO: Recuperar el lexema?
                //Una vez tenemos el final del elxema
                devolver()
                //Construimos el lexema
                //getWord() en el Sistema de entrada tb


                break;


            case 2:




        }

    }





}
