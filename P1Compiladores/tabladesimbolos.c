#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "definiciones.h"

/**/
abb tabla;

/* Estructura de almacenamiento para la tabla de simbolos*/
typedef struct{
    char *id;
    int codigo;
}keywords;


/* Almaceno las primeras Keywords */
keywords initial = {{"import", IMPORT}, {"id", ID},
                    {"string", STRING}, {"int", INT}, {"float", FLOAT}, {"while", WHILE},
                    {"foreach", FOREACH}, {"return", RETURN}, {"void", VOID}};

void insertSimbol(char *id, int codigo){
    tipoelem simb;
    simb.lexema

}

/*
 * Funcion encargada de inicializar la tabla de simbolos
 */
void initTabla(){

    crear(&tabla);
    for(int i = 0; i < 9; i++ ){

    }
}