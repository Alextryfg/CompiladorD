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


/*
 * Funcion encargada de devolver el componente lexico que será procesado por el Analizador sintáctico
 */
int siguiente_componente_lexico(tipoelem *comp){
    int accept = 0;
    int state = 0;
    char c = '\0';

    //Mientras el lexema no sea aceptado
    while(!accept){
        switch (state){
            case 0:
                c = siguiente_caracter();
                if(c == ' ' || c == '\t' || c== '.' || c == ';' || c == '}' || c == '{' || c == '(' || c == ')' || c == '\n'){
                    comp->codigo=c;
                    getLexema(comp);
                    accept=1;
                    break;

                }else if (isalpha(c) || c == '_'){ //IDENTIFICADORES
                    state = 1;
                    break;

                }else if (isdigit(c)){ //NUMEROS
                    state = 2;
                    break;

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
                //Una vez tenemos el final del elxema
                retroceder_puntero();

                //Construimos el lexema añadiendo directamente su codigo correspondiente.
                getLexema(comp);

                //Asocio el codigo
                comp->codigo= ID;

                //Informamos de que se acepta el lexema
                accept=1;


                break;


            case 2:

                //Se obtiene el siguiente caracter
                c = siguiente_caracter();

                //CONDICIONES INTEGER
                while(isdigit(c)){
                    c = siguiente_caracter();
                }
                //0b1010101
                //0B10010110

                //Solo depsues de un digito se puede barrabaja
                //0b_ MAL
                //0b1___ Bien
                //1_234 Bien
                //_2121 Mal
                //  Si encuentras un '.' o una 'e' o 'E' es float

                break;


            default:
                return -1;






        }

    }
    return comp->codigo;
}
