#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "definiciones.h"

/*Se trata del arbol que almacenara la tabla de simbolos*/
abb tabla;

/* Estructura de almacenamiento para la tabla de simbolos*/
typedef struct{
    char *id;
    int codigo;
}keywords;


/* Almaceno las primeras Keywords */

keywords initial[] = {{"identificador", ID},{"import", IMPORT},
                    {"string", STRING}, {"int", INT}, {"float", FLOAT}, {"while", WHILE},
                    {"foreach", FOREACH}, {"return", RETURN}, {"void", VOID}};

/*
 *
 * Funcion auxiliar para insertar en la tabla de simbolos, las keywords iniciales
 *
 */
void insertSimbol(keywords keys){

    //Creamos una instancia de la estrucutra tipoelem
    tipoelem simb;
    //Reservamos la memoria justa con el tamaño del id
    simb.lexema = (char*)malloc(sizeof(char)*(strlen(keys.id)+1));
    //Copiamos en simbolo el id y el código
    strcpy(simb.lexema,keys.id);
    simb.codigo = keys.codigo;
    //Insertamos el elemento en la tabla de simbolos
    insertar(&tabla,simb);



}

/*
 * Funcion encargada de inicializar la tabla de simbolos
 */
void initTabla() {

    crear(&tabla);
    for (int i = 0; i < 9; i++)
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

    //Si se encuentra en la tabla
    if(es_miembro(tabla, *simb)){
        tipoelem s;
        //Se busca el nodo por medio del lexema almacenandolo en s
        buscar_nodo(tabla,simb->lexema,&s);
        simb->codigo = s.codigo;

    }else{ //En caso de no existir

        //Creo una keyword y lo inserto
        keywords key;
        key.id = simb->lexema;
        key.codigo = simb->codigo;
        insertSimbol(key);


    }

}



/*
 * Funcion que imprime la tabla, invocando a la recursiva anterior
 */
void printTabla(){
    printf("\n*************TABLA DE SIMBOLOS*************");
    _printTabla(&tabla);
    printf("\n*******************************************");
}