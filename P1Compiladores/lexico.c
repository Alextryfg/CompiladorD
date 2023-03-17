#include <stdio.h>
#include <ctype.h>
#include "definiciones.h"
#include "tabladesimbolos.h"
#include "sistemaEntrada.h"
#include "abb.h"
#include "errores.h"

/*
 * Automatas a definir:
 *
 *  es simples: Caso 0;
 *  Delimitadores: Caso 0;
 *  identificadores: Caso 1;
 *  keywords: Caso 1;
 *  integers: Caso 2;
 *  es Dobles: Caso 4;
 *  Strings: Caso 5;
 *  floats: Caso 6;
 *
 */

/*
 * Funcion que forma el lexema y busca su codigo en la tabla de simbolos en caso de ser una palabra reservada
 */
void _formarLexema(tipoelem *comp,int retroceso){

    //Dependiendo del lexema será necesario retroceder el puntero delantero
    if(retroceso){
        retroceder_puntero();
    }

    //Cargo el lexema en comp
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

                //Separador o TOKENs
                if(c == ' ' || c == '\t' || c == '\r' || c == '\n' || c== '.' || c == ';' || c == '}' || c == '{' || c == '(' || c == ')' || c == '['
                || c == ']' || c == ','){
                    //En TOKENs simplemente devuelvo el ASCII
                        comp->codigo = c;
                        _formarLexema(comp,0);
                        accept = 1;

                //Identificador?
                }else if (isalpha(c) || c == '_'){ //IDENTIFICADORES
                    state = 1;
                //Numeros?
                }else if (isdigit(c)){ //NUMEROS (Se separa mas tarde en INTs y FLOATs)
                    state = 2;
                //Comentario?
                }else if(c == '/'){ //COMENTARIOS
                    state = 3;
                //Dobles operadores ?
                }else if(c == '+' || c == '=' || c == '-' || c == '*' || c == '|' || c == '>' || c == '<' || c == '&' || c == '%'){  //DOBLE?
                    state = 4;
                //String?
                }else if(c == '"'){ //STRING?
                    state = 5;
                }else if(c == EOF){
                    comp->codigo=-1;
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

                //Formo el lexema
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

                //Empieza por 0, posibilidad de binario
                if(c == '0'){
                    c = siguiente_caracter();
                    //Si hay una B o b despues del 0, binario
                    if ( c == 'b' || c == 'B'){
                        c = siguiente_caracter();
                        //Despues pueden venir los 0's, 1's o _ que sean
                        while(c == '0' || c == '1' || c == '_'){
                            c = siguiente_caracter();
                        }
                        //Palabra reservada INT
                        comp->codigo=INTEGER;
                    }else if(c == '.') {
                        //Estado de los float
                        state = 6;
                    }
                }else{
                    c = siguiente_caracter();
                    //Los integers pueden tener las _ por el medio que sean
                    while( c == '_' || isdigit(c)) {
                        c = siguiente_caracter();
                    }
                        //. , e o E significa FLOAT
                        if(c == '.' || c == 'e' || c == 'E') {
                            //Estado de los float
                            state = 6;
                        }else{
                            //De otra forma se reconoce como INT
                            comp->codigo= INTEGER;
                        }
                    }
                    //En cualquier otro caso se asigna int al codigo
                    comp->codigo=INTEGER;

                    //Si no esta en el estado 6, quiere decir que detecto INT, si no es FLOAT
                    if(comp->codigo == INTEGER && state!= 6){
                        //Formo el lexema
                        _formarLexema(comp,1);

                        //Acepto el lexema
                        accept=1;

                    }


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

                    while(c != '\n' ){
                        c = siguiente_caracter();
                    }
                //Comentario de varias lineas
                }else if( c == '*'){

                    c = siguiente_caracter();
                    //Variable para comprobar si se encuentra '*/' juntos
                    int juntos = 0;

                    while(juntos == 0){
                        //Caso de '*/' que señala el fin del comentario
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
                //Caso de que no es ningun tipo de comentario, por lo que se ira al automata de los es
                }else{

                    //Es decir encontramos '/' a solas
                    comp->codigo = SIMPLE;

                    //Elemento individual por lo que requiere retroceso de puntero delantero
                    _formarLexema(comp,1);

                    //Se acepta el lexema
                    accept = 1;

                }
                //En caso de ser un comentario

                //Igualamos los punteros para ignorarlo
                igualarPunteros();

                //Volvemos al estado inicial
                state = 0;
                break;

            case 4:
                /*
                 * Este caso seran los TOKENs que se reflejan en el manual
                 */

                //(==)
                if(c == '='){
                    c = siguiente_caracter();
                    if(c == '='){
                        comp->codigo = DOSIGUAL;
                    }else{ //En otro caso  normal
                        comp->codigo = SIMPLE;
                    }
                //(+=) o (++), en otro caso  normal
                }else if(c == '+'){
                    c = siguiente_caracter();
                    if(c == '='){
                        comp->codigo = SUMAIGUAL;
                    }else if(c == '+'){
                        comp->codigo = SUMASUMA;
                    }
                    else{
                        comp->codigo = SIMPLE;
                    }
                //(--) 0 (-=)
                }else if( c == '-'){
                    c = siguiente_caracter();
                    if( c == '-'){
                        comp->codigo = MENOSMENOS;
                    }else if(c == '='){
                        comp->codigo = MENOSIGUAL;
                    }else{
                        comp->codigo = SIMPLE;
                    }
                // (*=)
                }else if(c == '*'){
                    c = siguiente_caracter();
                    if( c == '=') {
                        comp->codigo = PORIGUAL;
                    }else{
                        comp->codigo = SIMPLE;
                    }
                // (|, |= , ||)
                }else if(c == '|'){
                    c = siguiente_caracter();
                    if( c == '=') {
                        comp->codigo = BARRAIGUAL;
                    }else if( c == '|') {
                        comp->codigo = DOSBARRA;
                    }else{
                            comp->codigo = SIMPLE;
                    }

                // ( &, &=, &&)
                }else if( c == '&'){
                    c = siguiente_caracter();
                    if( c == '=') {
                        comp->codigo = ANDIGUAL;
                    }else if( c == '&') {
                        comp->codigo = DOSAND;
                    }else{
                        comp->codigo = SIMPLE;
                    }
                // ( %=, %)
                }else if( c == '%'){
                    c = siguiente_caracter();
                    if( c == '='){
                        comp->codigo = PORCIENTOIGUAL;
                    }else{
                        comp->codigo = SIMPLE;
                    }
                //(<=, <, <<)
                }else if( c == '<'){
                    c = siguiente_caracter();
                    if( c == '=') {
                        comp->codigo = MENORIGUAL;
                    }else if( c == '<') {
                        comp->codigo = MENORMENOR;
                    }else{
                        comp->codigo = SIMPLE;
                    }
                //(>=, >, >>)
                }else if( c == '>') {
                    c = siguiente_caracter();
                    if (c == '=') {
                        comp->codigo = MAYORIGUAL;
                    } else if (c == '>') {
                        comp->codigo = MAYORMAYOR;
                    } else {
                        comp->codigo = SIMPLE;
                    }
                }

                // Si es un  simple, se necesita el retroceso del puntero
                if(comp->codigo == SIMPLE){
                    _formarLexema(comp,1);
                }else{
                    _formarLexema(comp,0);
                }


                //Informamos de que se acepta el lexema
                accept=1;

                break;

            case 5:
                /*
                 * A string literal is either a double quoted string, a wysiwyg quoted string, a delimited string, or a token string.
                 * In all string literal forms, an EndOfLine is regarded as a single \n character.
                 * String literals are read only.
                 * Undefined Behavior: writing to a string literal. This is not allowed in @safe code.
                 */

                /* Solo añadiremos los del codigo: ("") , en caso de \" no se detecta la comilla porque esta escapada */

                //Mientras el codigo no sea String
                while ( comp->codigo != STRING){
                    c = siguiente_caracter();
                    //Si es otra barra se acaba el string
                    if( c == '"'){
                        comp->codigo = STRING;
                        //Si es una '\' se puede poner '\"' sin que acabe al escaparse
                    }else if(c == '\\'){
                        c = siguiente_caracter();
                        //Aqui esta el '\"'
                        if( c == '"'){
                            c = siguiente_caracter();
                        }
                    }
                }


                //Formamos el lexema
                _formarLexema(comp,0);

                //Aceptamos el lexema
                accept = 1;

                break;

            case 6:
                //FLOATS
                /*
                 * 2.645_751
                 * 6.022140857E+23
                 * 6_022.140857E+20
                 * 6_022_.140_857E+20_
                 */
                /* De nuevo, haremos hincapie en los que aparecen en el codigo regression.d */
                while(isdigit(c) || c == '_'){
                    c = siguiente_caracter();
                }
                //Exponente (
                if(c == 'E' || c == 'e'){
                    c = siguiente_caracter();
                    //Caso: 4e+1
                    if(isdigit(c) || c == '-' || c == '+' || c == '_'){
                        c = siguiente_caracter();
                    }else{
                        errorD(4);
                    }
                    while(isdigit(c) || c == '_'){
                        c = siguiente_caracter();
                    }
                //Caso: 4.0
                }else if (c == '.'){
                    c = siguiente_caracter();
                    while(isdigit(c) || c == '_'){
                        c = siguiente_caracter();
                    }
                    //Caso: 4.0e+03
                    if(c == 'e' || c == 'E'){
                        c = siguiente_caracter();
                        if(c == '+' || isdigit(c) || c == '-' || c == '_'){
                            c = siguiente_caracter();
                            while(isdigit(c) || c == '_'){
                                c = siguiente_caracter();
                            }
                        }

                    }
                }

                //Asigno el codigo de float
                comp->codigo = FLOATPOINT;

                //Formo el lexema
                _formarLexema(comp,1);

                //Acepto el lexema
                accept = 1;

                break;

        }

    }

    /* Devolvemos al sintactico el código. (Comp se da por referencia por lo que se modifica durante la ejecucion de la funcion) */
    return comp->codigo;
}
