#include <stdio.h>
#include "abb.h"
#include "definiciones.h"

/*Se trata del arbol que almacenara la tabla de simbolos*/
abb tabla;


/* Almaceno las Keywords del programa */

tipoelem initial[] = {{"identificador", ID},{"import", IMPORT},
                    {"string", STRING}, {"int", INT}, {"float", FLOAT}, {"while", WHILE},
                    {"foreach", FOREACH}, {"return", RETURN}, {"void", VOID}, {"cast", CAST}, {"double", DOUBLE} };

/*
 * Funcion auxiliar para insertar en la tabla de simbolos, las keywords iniciales
 */
void insertSimbol(tipoelem keys){

    //Insertamos el elemento en la tabla de simbolos
    insertar(&tabla,keys);

}

/*
 * Funcion encargada de inicializar la tabla de simbolos
 */
void initTabla() {

    /* Creo la tabla */
    crear(&tabla);

    /* Inserto sus elementos */
    for (int i = 0; i < 11; i++)
        insertSimbol(initial[i]);

}

/*
 * Funcion encargada de la destruccion del arbol
 */
void destruirTabla(){
    destruir(&tabla);
}

/*
 * Funcion encargada de encontrar un lexema dentro de el arbol. Si el elemento no esta en la tabla, se introducirá
 */
void findCodigo(tipoelem *simb){

        /* Si es miembro simplemente se busca su codigo y se devuelve a _formarLexema en el lexico.c */
        if(es_miembro(tabla,*simb)){
            tipoelem s;
            //Se busca el nodo por medio del lexema almacenandolo en s
            buscar_nodo(tabla, simb->lexema, &s);
            simb->codigo = s.codigo;
        /* En caso de no ser miembro, lo insertamos como un nuevo ID */
        }else{
            simb->codigo= ID;
            insertSimbol(*simb);
        }

}

/*
 * Funcion que imprime la tabla, invocando a la recursiva de abb.h
 */
void printTabla(){
    printf("\n*************TABLA DE SIMBOLOS*************");
    _printTabla(&tabla);
    printf("\n*******************************************\n");
}