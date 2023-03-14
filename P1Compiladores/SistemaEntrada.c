
#include "SistemaEntrada.h"
#include "gestionerrores.h"
#include "abb.h"
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

/*
 * Funcion que ira devolviendo caracteres al analizador lexico
 */
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
                cargar_bloque(); //Carga un bloque nuevo
                printf(cent.centB); //Para ver si se carga el bloque nuevo
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
    //Devolvemos el caracter leido
    return lect;
}

/*
 * Funcion encargada de retroceder el puntero delantero en los centinelas, para que se cumpla que la estructura sea cíclica
 */
void retroceder_puntero(){

    if(cent.bloque == A && cent.delantero == 0){//En caso de estar en la primera pos del primer bloque

            //Se retrocede el puntero una posicon
            cent.bloque = B;
            //Por lo que se encontrara en la posicion ultima de B - 1, ya que 2 * N = EOFBloque B
            cent.delantero = N * 2 - 1;

    }else if(cent.bloque == B && cent.delantero == N){ //Ultima posicion del bloque A
            //Cambia de bloque
            cent.bloque = A;
            //Retrocede una posicion
            cent.delantero--;

    }else{

        cent.delantero--;

    }
}

/*
 * Funcion para recoger el lexema completo dependiendo de los distintos casos
 */
void getLexema(tipoelem *lexema){

    int tam = 0; //tamaño del lexema
    int cont = 0; //

    if(cent.inicio > cent.delantero){
        tam = (cent.delantero + 2 * N) - cent.inicio;
    }else{
        tam = cent.delantero - cent.inicio;
    }

    if(tam >N){
        perror("El tamaño del lexema es demasiado grande");
        cent.inicio = cent.delantero;
    }else{

        //Reservo memoria para el lexema
        lexema->lexema = (char*) malloc (sizeof(char)*(tam+1));


        //Caso de inicio en A y delantero en B copio la primera parte en del lexema
        if (cent.inicio < N && cent.delantero >= N ){
            for(int i = cent.inicio; i < N; i++){
                lexema->lexema[cont] = cent.centA[i];
                cont++;

            }
            //Paso inicio al siguiente bloque
            cent.inicio = N+1;
            //Copio el resto del lexema
            for(int i = cent.inicio; i < cent.delantero; i++ ){
                lexema->lexema[cont] = cent.centA[i];
                cont++;
            }
        //Caso de inicio en B y delantero en A
        }else if (cent.inicio > cent.delantero){
            for(int i = cent.inicio; i < (N*2)+1; i++){
                lexema->lexema[cont] = cent.centA[i];
                cont++;
            }
            //Paso cent.inicio al inicio del bloque A
            cent.inicio = 0;
            for(int i = cent.inicio; i < cent.delantero; i++){
                lexema->lexema[cont] = cent.centA[i];
                cont++;
            }

        }else{ //En caso de estar los dos en el mismo bloque
            for(int i = cent.inicio; i < cent.delantero; i++){
                lexema->lexema[cont] = cent.centA[i];
                cont++;
            }

        }
        //Le añadimos el \0 al final del lexema y el codigo
        lexema->lexema[cont] = '\0';
        cent.inicio = cent.delantero;

        if(cent.inicio == 63){
            printf("\ninicio => %d - delantero => %d\n", cent.inicio, cent.delantero);
            printf(cent.centB); //No carga el siguiente bloque
        }






        //TODO:Se supone que falta el caso de la flag avanza, que todavia no se muy bien que es, asi que lo dejare asi.






    }










}



