
#include "SistemaEntrada.h"
#include "gestionerrores.h"
//Aqui deberia incluirse la gestion de errores
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 64			//Tamaño de cada centinela, multiplo de la unidad de asignacion (4096)
#define A 0		//Position indicator on the first sentinel
#define B 1		//Position indicator on the second sentinel

typedef struct {
    char centA[N + 1];	//Primer centinela
    char centB[N + 1];	//Segundo centinela
    int inicio;			//Puntero que indica el comienzo de un lexema
    int delantero;		//Puntero delantero que
    int bloque;	//Variable that allows knowing in which sentinel the reading is being performed
} centinela;

//Estas son las variables globales

FILE *fichero;
int pos_lectura = 0;
centinela cent;
int retroceder = 0; //Marcando que no es necesario el retroceso
int avanzar = 0; //Todavia no se para que será esto

/*
 * Inicializo el centfer
 */
void inicializarCentinelas() {
    cent.inicio = 0;
    cent.delantero = 0;
    cent.bloque = A;
    cent.centA[N] = EOF;
    cent.centB[N] = EOF;
}

/*
 * Function in charge of opening the file, initializing the centfer and reading the first N characters stored in the first sentinel.
 * */
void abrir_archivo(char *archivo) {
    //Opening the file with error checking
    if ((fichero = fopen(archivo, "r")) != NULL) {
        //inicializo las centinelas
        inicializarCentinelas();
        // Leo N caracteres del archivo, y los almaceno en el centinela A
        int leidos = fread(cent.centA, sizeof(char),N, fichero);
        // Comprobamos errores durante apertura de archivo esto a lo mejor se puede ir a tomar por culo
        if(!ferror(fichero)){
            //Me aseguro de que todos los datos se han leido correctamente
            fflush(fichero);
            //Posicion actual de puntero de lectura
            pos_lectura = N;

        } else {
            //errorArchivo(2);
            printf("Error durante apertura");
        }

    } else
        //errorArchivo(1);
        printf("Error durante fopen");
}

/*Cerramos el archivo*/

void cerrar_archivo() {
    fclose(fichero);
}

void cargar_bloque(){
    int n_leidos = 0;
    //Comprobamos el bloque que es
    if (cent.bloque ==A){
        fseek(fichero, pos_lectura, SEEK_SET);

    }

}

char siguiente_caracter(){
    char leido;
    //Comprobamos en que bloque se encuentran los punteros y por lo tanto de quien es el turno, si es de A
    if(cent.bloque == A){
        //asignamos el caracter en el que se encuentra el puntero delantero a el char leido
        leido = cent.centA[cent.delantero];
        //Comprobamos si el caracter leido se corresponde con el EOF del centinela A, en caso de serlo se operara de la siguiente manera
        if(leido == EOF){
            //Una vez que sabemos que es EOF, comprobamos si es o no el del archivo con feof que nos permite conocer si la ultima pos del fichero ha sido leida
            if(!feof(fichero)){
                //En caso de no serlo, quiere decir que llegamos al final del bloque y por lo tanto deberemos pasar al bloque B
                cent.bloque = B;
                //Ahoira compruebo si tengo o no que carga r el bloque por medio de la flag de retroceso, esto quiere decir, que en caos de que el lexema acabe justo al final del BloqueA,
                //Es decir que el separador esta en la primera posicon del bloqueB, tendre que encargarme de que el programa no cargue un bloque nuevo entero dentro del bloqueB, de esta
                //forma, no se perdera el bloqueB.  | |:|=|EOFBloqueA| |l|e|x|e|m|a|EOFbloqueB -> aqui se le := y en cuanto se llega al espacio en blanco, sabemos que tenemos que
                //coger := . Por lo tanto, para recoger := llegas a EOFBloqueA con el puntero delantero, cargas un bloque nuevo en B, y encuentras el espacio. Esto te señala que tienes
                //que recoger el lexema := y por lo tanto delantero se queda en EOFbloqueA. Una vez ahi, Inicio y delnatero tendrán la misma posicon, por lo que delantero volvera a
                //pasar por el EOFBloqueA y en vez e volver a cargar en el bloque Btodo de nuevo, se anula la funcion de cargar bloque con el FLAG.
                if(!retroceder){
                    //Si no hace falta retroceder, se carga en el centinela el siguiente bloque
                    //cargar_bloque();
                }else{
                    //En caso de que si que sea necesario retroceder, no se ocupa el centinela, y por lo tanto, no se carga el siguiente bloque en el centinelaB
                    retroceder=0;
                }
                //Recordemos que se entra aqui solo cuando el caracyer leido es EOF. Es por esto, que una vez arreglado el problema, y movido el puntero
                //delantero a una posicion legible, se volvera a invocar a esta función, de manera que deberia simplmente pasar de los ifs y devolver el siguiente caracter.
                leido = siguiente_caracter();

            }else{
                //Si es EOF del fichero, simplemente se adelanta una posicon el puntero delantero, y se termina.
                cent.delantero++;
            }

        }else{
            //Si no es EOF, simplemente se adelanta el puntero delantero, recordemos que el caracter ya estaba cargado en la variable leido, por lo que aqui
            //deberemos actualizar el puntero, para que en la proxima invocacions se devuleva el siguiente caracter
            cent.delantero++;
        }

    }else if (cent.bloque == B){
        //LO mismo
    }
}

