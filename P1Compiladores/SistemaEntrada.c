
#include "SistemaEntrada.h"
#include "gestionerrores.h"
//Aqui deberia incluirse la gestion de errores
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 64			//Tamaño de cada centinela
#define A 0		//Indicador de Bloque B
#define B 1		//Indicador de Bloque A

/*
 * centA: Primer centinela
 * centB: Segundo centinela
 * inicio: Puntero que indica el comienzo del lexema
 * delantero: Puntero delantero que indica el final del bloque
 * bloque: Bloque de lectura del lexema actual
 */

typedef struct {
    char centA[N + 1];	//Primer centinela
    char centB[N + 1];	//Segundo centinela
    int inicio;			//Puntero que indica el comienzo de un lexema
    int delantero;		//Puntero delantero que
    int bloque;
} centinela;

//Estas son las variables globales

FILE *fichero;
int p_lecture = 0;
centinela cent;
int retroceder = 0; //Marcando que no es necesario el retroceso
int avanzar = 0; //Todavia no se para que será esto

/*
 * Inicializo el centenila para posicionar el puntero delantero e inicio en la posicion 0. Ademas de colocar
 * el bloque actual de centinela en A y los EOF de los dos centinelas.
 */
void initCentinelas() {
    cent.inicio = 0;
    cent.delantero = 0;
    cent.bloque = A;
    cent.centA[N] = EOF;
    cent.centB[N] = EOF;
}

/*
 * Abro el archivo y cargo el primer bloque en el centinela A
 */
void abrirD(char *archivo) {
    //Abrimos el archivo en el file fichero
    if ((fichero = fopen(archivo, "r")) != NULL) {
        //Inicializo los centinelas
        initCentinelas();
        // Leo 2N caracteres del archivo, y los almaceno en el centinela A y B
        p_lecture=fread(cent.centA, sizeof(char),N, fichero);
        //Posicion actual de puntero de lectura
        p_lecture = ftell(fichero);
        printf(cent.centA);

    } else{
        //errorArchivo(1);
        perror("Error durante fopen");
        exit(0);
    }

}

/*
 *
 * Cerramos el archivo
 *
 * */

void cerrarD() {
    fclose(fichero);
}

/*
 *
 * Funcion utilizada para cargar los bloques en los distintos centinelas.
 *
*/
void cargar_bloque() {

    //Posicionamos el puntero de lectura en p_lecture
    fseek(fichero, p_lecture, SEEK_SET);

    //Comprobamos el bloque en el que nos encontramos
    if (cent.bloque == A) {
        //Leemos los caracteres del fichero regression.d
        p_lecture += fread(cent.centA, sizeof(char), N, fichero);
        //TODO: Comprobar si lexema sobrepasa al bloque para caso A y B

    } else if (cent.bloque == B) {
        //Leemos los caracteres del fichero regression.d
        p_lecture += fread(cent.centB, sizeof(char), N, fichero);
    }
}

char siguiente_caracter(){

    char lect;
    //Comprobamos en que bloque se encuentran los punteros y por lo tanto de quien es el turno, si es de A
    if(cent.bloque == A){
        //asignamos el caracter en el que se encuentra el puntero delantero a el char lect
        lect = cent.centA[cent.delantero];
        //EOF?
        if(lect == EOF){
            //Final archivo?
            if(!feof(fichero)){ //No
                //Puntero delantero -> Bloque B
                cent.bloque = B;
                //TODO: Comprobar caso de retroceder

                //Nos devuelve el siguiente caracter que no es EOF
                lect = siguiente_caracter();

            }else{ //Si

                // Adelanta p_delantero una pos
                cent.delantero++;
            }

        }else{ //No es EOF

            //Se adelanta p_delantero una pos
            cent.delantero++;
        }

    }else if (cent.bloque == B){

        lect = cent.centB[cent.delantero];
        if(lect == EOF){
            if(!feof(fichero)){
                cent.bloque = A;
                //TODO: Comprobar caso de retroceder

                lect = siguiente_caracter();

            }else{
                cent.delantero++;
            }
        }else{
            cent.delantero++;
        }
    }
}

