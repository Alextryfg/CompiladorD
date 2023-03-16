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
 * Funcion que forma el lexema y busca su codigo en la tabla de simbolos en caso de ser una palabra reservada
 */
void _formarLexema(tipoelem *comp,int retroceso){

    if(retroceso){
        retroceder_puntero();
    }

    getLexema(comp);

    //Caso especial para keyWords
    if(comp->codigo==ID){
        //Buscar el lexema
        findCodigo(comp);
    }


}

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
                /*
                 * Estado Inicial
                 */

                //Siguiente caracter
                c = siguiente_caracter();

                //Separador?
                if(c == ' ' || c == '\t' || c == '\r' || c == '\n' || c== '.' || c == ';' || c == '}' || c == '{' || c == '(' || c == ')' || c == '['
                || c == ']' || c == ',' || c == '*' || c == '<' || c == '>' || c == '%'){
                    if( c == '*' || c == '<' || c == '>' ){
                        comp->codigo=OPERADORSIMPLE;
                        _formarLexema(comp,0);
                        accept = 1;
                    }else{
                        comp->codigo = c;
                        _formarLexema(comp,0);
                        accept = 1;
                    }

                //Identificador?
                }else if (isalpha(c) || c == '_'){ //IDENTIFICADORES Y ALFANUMERICOS
                    state = 1;
                //Numeros?
                }else if (isdigit(c)){ //NUMEROS
                    state = 2;
                //Comentario?
                }else if(c == '/'){
                    state = 3;
                //Doble Operador
                }else if(c == '+' || c == '='){
                    state = 4;
                }else if(c == '"'){
                    state = 5;
                }else if(c == EOF){
                    printf("EOOOOOOF");
                    accept = 1;
                }

                break;
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

                //Asocio el codigo
                comp->codigo= ID;

                _formarLexema(comp,1);

                //Informamos de que se acepta el lexema
                accept=1;


                break;


            case 2:
                /*
                 * Integers can be specified in decimal, binary, or hexadecimal.
                 * Decimal integers are a sequence of decimal digits.
                 * Binary integers are a sequence of binary digits preceded by a ‘0b’ or ‘0B’.
                 */

                if(c == 0){
                    c = siguiente_caracter();
                    //Si hay una B o b despues del 0, binario
                    if ( c == 'b' || c == 'B'){
                        c = siguiente_caracter();
                        //Despues pueden venir los 0's, 1's o _ que sean
                        while(c == '0' || c == '1' || c == '_'){
                            c = siguiente_caracter();
                        }
                        //Palabra reservada INT
                        comp->codigo=INT;
                    }else if(c == '.') {
                        //Estado de los float
                        state = 6;
                    }
                }else{
                    c = siguiente_caracter();
                    //Los integers pueden tener las _ por el medio que sean
                    while( c == '_' || isdigit(c)){
                        c = siguiente_caracter();
                        //. , e o E significa FLOAR
                        if(c == '.' || c == 'e' || c == 'E') {
                            //Estado de los float
                            state = 6;
                        }else{
                            //De otra forma se reconoce como INT
                            comp->codigo= INT;
                        }
                    }
                }

                //Formo el lexema
                _formarLexema(comp,1);

                //Acepto el lexema
                accept=1;

                break;

            case 3:
                /*
                 * Block comments can span multiple lines, but do not nest.
                 * Line comments terminate at the end of the line.
                 * Nesting block comments can span multiple lines and can nest.
                 */

                c = siguiente_caracter();

                //Comentario de una sola linea
                if( c == '/'){

                    while(c != '\n'){
                        c = siguiente_caracter();
                    }
                //Comentario de varias lineas
                }else if( c == '*'){

                    c = siguiente_caracter();
                    //Variable para comprobar si se encuentra '*/' juntos
                    int juntos = 0;

                    while(juntos == 0){

                        if(c == '*'){
                            c = siguiente_caracter();
                            if(c == '/'){
                                juntos = 1;
                            }
                        }
                        c = siguiente_caracter();
                    }
                //Comentario anidado
                }else if(c == '+'){

                    //Numero de comentarios anidades, al principio es minimo uno debido al /+
                    int num = 1;

                    // Mientras sea mayor que 0 y no sea un EOF
                    while (num > 0 && c != EOF) {
                        c = siguiente_caracter();
                        //Si es un '+/' quiere decir que cierra comentario, por lo que se resta al contador
                        if (c == '+') {
                            c = siguiente_caracter();
                            if (c == '/') {
                                num--;
                            }
                        }
                        //Si es '/+' quiere decir que abre y por lo tanto se suma
                        else if (c == '/') {
                            c = siguiente_caracter();
                            if (c == '+') {
                                num++;
                            }
                        }
                    }
                //Caso de que no es ningun tipo de comentario, por lo que se ira al automata de los operadores
                }else{

                    comp->codigo = OPERADORSIMPLE;

                    _formarLexema(comp,0);

                    accept = 1;

                }

                //Igualamos los punteros para ignorar los comentarios
                igualarPunteros();

                //Volvemos al estado inicial
                state = 0;
                break;

            case 4:
                //(==)
                if(c == '='){
                    c = siguiente_caracter();
                    if(c == '='){
                        comp->codigo = OPERADORSUMASUMA;
                    }else{ //En otro caso operador normal
                        comp->codigo = OPERADORSIMPLE;
                    }
                //(+=) o (++), en otro caso operador normal
                }else if(c == '+'){
                    c = siguiente_caracter();
                    if(c == '='){
                        comp->codigo = OPERADORSUMAIGUAL;
                    }else if(c == '+'){
                        comp->codigo = OPERADORSUMASUMA;
                    }
                    else{
                        comp->codigo = OPERADORSIMPLE;
                    }
                }

                if(comp->codigo == OPERADORSIMPLE){
                    _formarLexema(comp,1);
                }else{
                    _formarLexema(comp,0);
                }


                //Informamos de que se acepta el lexema
                accept=1;

                break;

            case 5:

                //Mientras el codigo no sea String
                while ( comp->codigo != STRING){
                    c = siguiente_caracter();
                    //Si es otra barra se acaba el string
                    if( c == '"'){
                        comp->codigo = STRING;
                        //Si es una '\' se puede poner '\"' sin que acabe
                    }else if(c == 92){
                        c = siguiente_caracter();
                        //Aqui esta el '\"'
                        if( c == '"'){
                            c = siguiente_caracter();
                        }
                    }
                }


                //Fomramos el lexema
                _formarLexema(comp,0);

                //Aceptamos el lexema
                accept = 1;

                break;

            case 6:
                //FLOATS
                if(c == 'E' || c == 'e'){
                    c = siguiente_caracter();
                    if(isdigit(c) || c == '-' || c == '+'){
                        c = siguiente_caracter();
                    }else{
                        perror("MAL FORMADO EL EXPONENTE");
                    }
                    while(isdigit(c)){
                        c = siguiente_caracter();
                    }
                }else if (c == '.'){
                    c = siguiente_caracter();
                    while(isdigit(c)){
                        c = siguiente_caracter();
                    }
                }

                comp->codigo = FLOAT;


            default:
                //return -1;






        }

    }




    return comp->codigo;
}
