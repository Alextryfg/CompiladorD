#include "sistemaEntrada.h"
#include "errores.h"
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

    /* Abro el archivo */

    file = fopen(input, "r");

    /* Error en caso de fallo en fopen */

    if (file == NULL){
        errorD(1);
    }

    /* Incializo punteros, bloque actual y centinelas */
    cent.inicio = 0;
    cent.delantero = 0;
    cent.bloque = A;
    cent.centA[N] = EOF;
    cent.centB[N] = EOF;

    /* Cargo el primer bloque */

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

    /* Comprobamos el bloque en el que nos encontramos */

    if (cent.bloque == A) {

        /* Leemos los caracteres del file regression.d para cargarlos en A */

        p_lecture = fread(cent.centA, sizeof(char), N, file);

        /* En de leer menos de 64 caracteres quiere decir que encontro el EOF, y por lo tanto se asigna al centinela en la pos correspondiente */

        if(p_lecture < 64){
            cent.centA[p_lecture] = EOF;
        }

    } else if (cent.bloque == B) {

        /* Leemos los caracteres del file regression.d para cargarlos en A */

        p_lecture = fread(cent.centB, sizeof(char), N, file);

        /* En de leer menos de 64 caracteres quiere decir que encontro el EOF, y por lo tanto se asigna al centinela en la pos correspondiente */

        if(p_lecture < 64){
            cent.centB[p_lecture] = EOF;
        }

    }else{

        errorD(3);

    }

    /* p_lecture almacena la posicon a leer del fichero */

}

/*
 * Funcion que ira devolviendo caracteres al analizador lexico dependiendo del caso
 */
char siguiente_caracter(){

    char lect;

    /* Comprobamos en que bloque se encuentran los punteros y por lo tanto de quien es el turno, si es de A */

    if(cent.bloque == A){

        /* asignamos el caracter en el que se encuentra el puntero delantero */
        lect = cent.centA[cent.delantero];

        /* Es EOF? */

        if(lect == EOF){

            /* Si lo es preguntamos si es el EOF del archivo */

            if(!feof(file)){ /* Es el EOF del bloque */

                /* Puntero delantero -> Bloque B */

                cent.bloque = B;

                /* Delantero pasa a la primera pos del bloque B */

                cent.delantero = N+1;

                /* Se carga el bloque en B para poder seguir leyendo */

                cargar_bloque();

                /* Nos saltamos el EOF del bloque e invocamos al siguiente caracter del bloque B */

                lect = siguiente_caracter();


            }else{ /* En caso de ser EOF del fichero, si se devuelve para terminar */

                /* Adelanto el puntero delantero */

                cent.delantero++;

                /*Devuelve un EOF*/

                return EOF;
            }

        }else{ /* En caso de no ser EOF avanza delantero */

            /* Se adelanta p_delantero una pos */
            cent.delantero++;
        }

    }else if (cent.bloque == B){

        /* Se realiza el mismo proceso que para el bloque A */

        lect = cent.centB[cent.delantero-(N+1)];

        if(lect == EOF){

            if(!feof(file)){

                /* Cambio al bloque A */

                cent.bloque = A;

                /* En este caso Puntero delantero a primera pos de A que es 0 */

                cent.delantero= 0;

                /* Cargo un nuevo bloque */

                cargar_bloque();

                /* Nos saltamos el EOF del bloque e invocamos al siguiente caracter del bloque A */

                lect = siguiente_caracter();

            }else{

                /* Adelanto el puntero delantero */

                cent.delantero++;

                /*Devuelve un EOF*/

                return EOF;
            }
        }else{  /* En caso de no ser EOF avanza delantero */

            /* Se adelanta p_delantero una pos */

            cent.delantero++;
        }
    }

    /* Devolvemos el caracter a siguiente_comp_lexico */

    return lect;
}

/*
 * Funcion encargada de retroceder el puntero delantero en los centinelas, para que se cumpla que la estructura sea cíclica
 */
void retroceder_puntero(){

    /* Casos especiales */

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
 * Funcion privada para Igualar punteros
 */
void igualarPunteros(){
    cent.inicio = cent.delantero;
}


/*
 * Funcion para recoger el lexema completo dependiendo de los distintos casos
 */
void getLexema(tipoelem *lexema){

    int tam; //tamaño del lexema
    int cont = 0; //Contador


    if(cent.inicio > cent.delantero){
        // l|a| |-| |h|o
        tam = (2 * N - cent.inicio + cent.delantero)+1;

    }else{
        // |h|o|l|a|-| | |
        tam = cent.delantero - cent.inicio;
    }

    //Si el lexema tiene un tamaño mayor o igual al bloque
    //|h|o|l|o|-|c|a|u|s|t|o|
    if(tam >=N){
        errorD(2);
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

            //leo |h|o|
            for(int i = cent.inicio; i < ((N+1)*2)-1; i++){
                lexema->lexema[cont] = cent.centA[i];
                cont++;
            }

            //Paso cent.inicio al inicio del bloque A
            cent.inicio = 0;

            //leo |l|a|
            for(int i = cent.inicio; i < cent.delantero; i++){

                lexema->lexema[cont] = cent.centA[i];
                cont++;
            }

        }else{

            //En caso de estar los dos en el mismo bloque
            // |h|o|l|a|-| | |

            /* Si esta en el bloque A */
            if(cent.bloque == A){

                for(int i = cent.inicio; i < cent.delantero; i++){
                    lexema->lexema[cont] = cent.centA[i];
                    cont++;
                }
            /* Si esta en el bloque B */
            }else{
                if(cent.centB[cent.inicio] == EOF){
                    for(int i = cent.inicio+1; i < cent.delantero; i++){
                        lexema->lexema[cont] = cent.centB[i-(N+1)]; //VAlor absoluto? o cambiar N+1 por N en la resta?
                        cont++;
                    }
                }else{
                    for(int i = cent.inicio; i < cent.delantero; i++){
                        lexema->lexema[cont] = cent.centB[i-(N+1)]; //VAlor absoluto? o cambiar N+1 por N en la resta?
                        cont++;
                    }
                }


            }


        }
        /* Le añadimos el \0 al final del lexema */
        lexema->lexema[cont] = '\0';

        /* Igualamos los punteros Inical y final */
        igualarPunteros();
    }

}



