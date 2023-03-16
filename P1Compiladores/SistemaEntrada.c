
#include "SistemaEntrada.h"
#include "gestionerrores.h"
#include "abb.h"
#include <stdio.h>
#include <stdlib.h>

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

/* Variables Globales */

FILE *file;
int p_lecture = 0;
centinela cent;

/* Definicion de cargar bloque */

void cargar_bloque();

/*
 * Inicializacion del sistema de entrada
 */
void init(char *input){

    file = fopen(input, "r");

    if (file == NULL){
        perror("Error en fopen");
    }

    cent.inicio = 0;
    cent.delantero = 0;
    cent.bloque = A;
    cent.centA[N] = EOF;
    cent.centB[N] = EOF;

    cargar_bloque();

}

/*
 * Cerramos el archivo
*/
void cerrarD() {
    fclose(file);
}

/*
 * Funcion utilizada para cargar los bloques en los distintos centinelas.
*/
void cargar_bloque() {

    //Posicionamos el puntero de lectura en p_lecture
    //fseek(file, p_lecture, SEEK_SET);

    //Comprobamos el bloque en el que nos encontramos
    if (cent.bloque == A) {
        //Leemos los caracteres del file regression.d
        p_lecture += fread(cent.centA, sizeof(char), N, file);
        //TODO: Comprobar si lexema sobrepasa al bloque para caso A y B

    } else if (cent.bloque == B) {
        //Leemos los caracteres del file regression.d para cargarlo en B
        p_lecture += fread(cent.centB, sizeof(char), N, file);
    }

}

int a=0;

/*
 * Funcion que ira devolviendo caracteres al analizador lexico
 */
char siguiente_caracter(){

    if(cent.delantero == 123){
        a++;
        if(a==2){
            printf("e");
        }

    }

    char lect;
    //Comprobamos en que bloque se encuentran los punteros y por lo tanto de quien es el turno, si es de A
    if(cent.bloque == A){

        //asignamos el caracter en el que se encuentra el puntero delantero a el char lect
        lect = cent.centA[cent.delantero];

        //EOF?
        if(lect == EOF){

            //Final archivo?
            if(!feof(file)){ //No

                //Puntero delantero -> Bloque B
                cent.bloque = B;
                cent.delantero = N+1; //Primera posicion del bloque B
                //TODO: Comprobar caso de retroceder
                //Para ver si se carga el bloque nuevo
                cargar_bloque();
                //Nos devuelve el siguiente caracter que no es EOF
                lect = siguiente_caracter();


            }else{ //Si

                // Adelanta p_delantero una pos
                printf("EOF fichero..");
                exit(0);
                cent.delantero++;
            }

        }else{ //No es EOF

            //Se adelanta p_delantero una pos
            cent.delantero++;
        }

    }else if (cent.bloque == B){

        lect = cent.centB[cent.delantero-(N+1)];

        if(lect == EOF){

            if(!feof(file)){

                //Cambio al bloque A
                cent.bloque = A;
                //Puntero delantero a primera pos de A
                cent.delantero= 0;
                //Cargo un nuevo bloque
                cargar_bloque();

                //TODO: Comprobar caso de retroceder
                lect = siguiente_caracter();

            }else{
                printf("EOF fichero..");
                exit(0);
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

            //Bloque actual B
            cent.bloque = B;

            //Delantero a final del centinela B sin tocar EOF
            cent.delantero = N * 2 - 1;


    }else if(cent.bloque == B && cent.delantero == N+1){ //Primera posicion del bloque B
            //Cambia de bloque
            cent.bloque = A;
            //Retrocede una posicion
            cent.delantero = N - 1;

    }else{

        cent.delantero--;

    }
}


/*
 * Funcion para recoger el lexema completo dependiendo de los distintos casos
 */
void getLexema(tipoelem *lexema){

    int tam; //tamaño del lexema
    int cont = 0; //Contador


    if(cent.inicio > cent.delantero){
        // l|a| |-| |h|o
        tam = 2 * N - cent.inicio + cent.delantero;

    }else{
        // |h|o|l|a|-| | |
        tam = cent.delantero - cent.inicio;
    }

    //Si el lexema tiene un tamaño mayor o igual al bloque
    //|h|o|l|o|-|c|a|u|s|t|o|
    if(tam >=N){
        //TODO:GESTION DE ERRORES LEXEMA TAM
        perror("El tamaño del lexema es demasiado grande");
        igualarPunteros();
    }
    //Si el lexema tiene un tamaño menor
    //|o|l|a| |-| | | | | | |
    else{

        //Reservo memoria para el lexema
        lexema->lexema = (char*) malloc (sizeof(char)*(tam+1));


        //Caso de inicio en A y delantero en B
        //| | | |h|o|-|l|a| | |
        if (cent.inicio < N && cent.delantero > N ){

            //Copio la primera parte en del lexema
            for(int i = cent.inicio; i < N; i++){
                lexema->lexema[cont] = cent.centA[i];
                cont++;

            }

            //Paso inicio al siguiente bloque
            cent.inicio = N+1;

            //Copio el resto del lexema
            for(int i = N+1; i < cent.delantero; i++ ){
                lexema->lexema[cont] = cent.centB[i-(N+1)];
                cont++;
            }
        //Caso de inicio en B y delantero en A
        // l|a| |-| |h|o
        }else if (cent.inicio > cent.delantero){

            for(int i = cent.inicio; i < ((N+1)*2)-1; i++){
                lexema->lexema[cont] = cent.centA[i];
                printf("\nIndice -> %d, Inicio -> %d, Delantero-> %d = %c", i, cent.inicio, cent.delantero, cent.centA[i] );
                cont++;
            }
            //Paso cent.inicio al inicio del bloque A
            cent.inicio = 0;

            for(int i = cent.inicio; i < cent.delantero; i++){

                lexema->lexema[cont] = cent.centA[i];
                printf("\nIndice -> %d, Inicio -> %d, Delantero-> %d = %c", i, cent.inicio, cent.delantero, cent.centA[i] );
                cont++;
            }

        }else{

            //En caso de estar los dos en el mismo bloque
            // |h|o|l|a|-| | |

            if(cent.bloque == A){

                for(int i = cent.inicio; i < cent.delantero; i++){
                    lexema->lexema[cont] = cent.centA[i];
                    cont++;
                }

            }else{

                for(int i = cent.inicio; i < cent.delantero; i++){
                    lexema->lexema[cont] = cent.centB[i-(N+1)];
                    cont++;
                }

            }


        }
        //Le añadimos el \0 al final del lexema
        lexema->lexema[cont] = '\0';

        igualarPunteros();

        //TODO:Se supone que falta el caso de la flag avanza, que todavia no se muy bien que es, asi que lo dejare asi.
    }

}

/*
 * Funcion privada para Igualar punteros
 */
void igualarPunteros(){
    cent.inicio = cent.delantero;
}



