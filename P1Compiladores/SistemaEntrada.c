
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

/*
 * Inicializo el buffer
 */
void inicializarCentinelas() {
	cent.inicio = 0;
	cent.delantero = 0;
	cent.bloque = A;
}

/*
 * Function in charge of opening the file, initializing the buffer and reading the first N characters stored in the first sentinel.
 * */
void abrir_archivo(char *archivo) {
	//Opening the file with error checking
	if ((fichero = fopen(archivo, "r")) != NULL) {
        //inicializo las centinelas
        inicializarCentinelas();
        // Leo N caracteres del archivo, y los almaceno en el centinela A
        int leidos = fread(cent.centA, sizeof(char),N, fichero);



		} else {
			//errorArchivo(2);
		}

	} else
		//errorArchivo(1);
}

/*File closure*/
void cerrar_archivo() {
	fclose(archivoEntrada);
}

/*
 * Private function in charge of the occupation of the next sentinel
 * Using the number of characters read, the EOF is placed in the last position.
 * */
void _ocupar_centinela() {
	int caracteres_lidos = 0;
	if (buf.turno == PRIMERO) {
		fseek(archivoEntrada, pos_lectura, SEEK_SET);
		caracteres_lidos = fread(buf.centA, sizeof(char), N, archivoEntrada);
		buf.centA[caracteres_lidos] = EOF;
		if (!ferror(archivoEntrada)) {
			fflush(archivoEntrada);
			pos_lectura = ftell(archivoEntrada);
			buf.delantero = 0;
			if (sobrepasa)//If the lexeme is too long, we place the start pointer in the next block
				buf.inicio = N;
		} else {
			errorArchivo(2);
		}

	} else if (buf.turno == SEGUNDO) {
		fseek(archivoEntrada, pos_lectura, SEEK_SET);
		caracteres_lidos = fread(buf.centB, sizeof(char), N, archivoEntrada);
		buf.centB[caracteres_lidos] = EOF;
		if (!ferror(archivoEntrada)) {
			fflush(archivoEntrada);
			pos_lectura = ftell(archivoEntrada);
			if (sobrepasa)
				buf.inicio = 0;
		} else {
			errorArchivo(2);
		}
	}
}

/*
 * Function in charge of returning the next character stored in the corresponding sentinel. If this character is EOF,
 * it should be checked whether it is end of file or end of sentinel.
 *
 * return caracter: sentinel current character
 * */
char seguinte_caracter() {
	char caracter_leido;
	if (buf.turno == PRIMERO) {
		//Check if the read character is the end of file or another type of character
		if ((caracter_leido = buf.centA[buf.delantero]) == EOF) {

			//In case of finding the EOF symbol, it is checked if it is the end of file or block
			if (!feof(archivoEntrada)) {
				//In case of finishing the block, the following must be filled in
				buf.turno = SEGUNDO;
				//It is checked that the next block can be loaded, since it was possible to go back to the first one or accept a lexeme
				if (retroceso) {
					//If start is in the next block, it means that the lexeme exceeds the allowed size
					if (buf.inicio > N) {
						sobrepasa = 1;
					}
					_ocupar_centinela();
				} else {
					retroceso = 0;
				}
				caracter_leido = seguinte_caracter();//The function is called again to return the character

			} else {
				buf.delantero++;
			}
		} else {
			buf.delantero++;
		}
	} else if (buf.turno == SEGUNDO) {

		if ((caracter_leido = buf.centB[buf.delantero - N]) == EOF) {

			if (!feof(archivoEntrada)) {

				buf.turno = PRIMERO;

				if (retroceso) {
					if (buf.inicio <= N) {
						sobrepasa = 1;
					}
					_ocupar_centinela();
				} else {
					retroceso = 0;
				}
				caracter_leido = seguinte_caracter();
			} else
				buf.delantero++;
		} else {
			buf.delantero++;
		}
	}

	return caracter_leido;
}

/*
 * Calculation to know the size of the lexeme
 * */
int _tam_lexema() {
	if (buf.inicio < N && buf.delantero > N) {
		return buf.delantero - buf.inicio;
	} else if (buf.inicio > buf.delantero){
		return (N * 2) - buf.inicio + buf.delantero;
	}
	return buf.delantero - buf.inicio;
}

/*
 * Deshace el avance del puntero delantero, cuando se encuentra el carácter que determina el final del lexema y
 * permite asi que se recupere el lexema al completo.
 * */
void devolver_caracter() {
	if (buf.turno == PRIMERO) {
		//Si el puntero delantero se encuentra en la primera posicion del bloque A, se cambia el turno
		// y se coloca el puntero delantero en la ultima posicion del bloque B
		if (buf.delantero == 0) {
			buf.turno = SEGUNDO;
			buf.delantero = N * 2 - 1;
			retroceso = 0;
		} else { //Si no esta en esa posicion(que es comprometida), simplemente se retrocede una posicion
			buf.delantero--;
		}
	} else if (buf.turno == SEGUNDO) {
		//Es lo mismo que en el bloque A, pero estando en la primera posicion del bloque B. Tienes que volver a la ultima posicion del bloque A
		// para poder recuperar el lexema completo.
		if (buf.delantero == N) {
			buf.turno = PRIMERO;
			buf.delantero = N - 1;
			retroceso = 0;
		} else {
			buf.delantero--;
		}
	}
}

/*
 * Functions that retrieve the lexeme and return it
 *
 * In case the size exceeds the allowed one, the last part of the lexeme will be returned
 *
 * Si avanza es 0, se devuelve el lexema completo
 * */
//Obtener lexema
void _recuperar(char **lexema, int tam, int avanza) {

	if (!avanza) {
		*lexema = (char*) malloc(tam + 1 * sizeof(char));//reservo memoria para el lexema
		if (buf.inicio < N && buf.delantero >= N) {	//Si el puntero inicio esta en el centinela A y el puntero delantero en el centinela B
			strncpy(*lexema, buf.centA + buf.inicio, N - buf.inicio); //Se copia en lexema desde el inicio hasta el final del centinela A, los caracteres desde el puntero inicio, hasta el final del centinela A
			strncpy(*lexema + N - buf.inicio, buf.centB, buf.delantero - N); //Se apunte al ultimo caracter copiado en el anterior strncpy y se copia desde el inicio del centinela B hasta el puntero delantero
		} else if (buf.inicio > buf.delantero) {//Si el puntero inicio esta en el centinela B y el puntero delantero en el centinela A
			strncpy(*lexema, buf.centB + buf.inicio - N, (N * 2) - buf.inicio);//Se copia desde el puntero incio hasta el final de B
			strncpy(*lexema + (N * 2) - buf.inicio, buf.centA, buf.delantero); //Se apunte al ultimo caracter copiado en el anterior strncpy y se copia desde el inicio del centinela A hasta el puntero delantero

		} else {
			if (buf.turno == PRIMERO) {
				strncpy(*lexema, buf.centA + buf.inicio,
						buf.delantero - buf.inicio);

			} else if (buf.turno == SEGUNDO) {
				strncpy(*lexema, buf.centB + buf.inicio - N,
						buf.delantero - buf.inicio);
			}
		}
		*(*lexema + tam) = '\0';
	} else {//In this case, the length of the lexeme can only be N characters, which is the maximum allowed, returning the last N characters
		*lexema = (char*) malloc(N + 1 * sizeof(char));
		int avanzar = tam - N;
		if (buf.inicio < N && buf.delantero >= N) {
			strncpy(*lexema, buf.centA + buf.inicio + avanzar,
			N - buf.inicio - avanzar);
			strncpy(*lexema + N - buf.inicio - avanzar, buf.centB,
					buf.delantero - N);
		} else if (buf.inicio > buf.delantero) {
			strncpy(*lexema, buf.centB + buf.inicio - N + avanzar,
					(N * 2) - buf.inicio - avanzar);
			strncpy(*lexema + (N * 2) - buf.inicio - avanzar, buf.centA,
					buf.delantero);

		}
		*(*lexema + N) = '\0';
	}

}


void recuperar_lexema(char **lexema) {

	int tam = _tam_lexema();
	if (!sobrepasa) {

		if (tam <= N) {
			_recuperar(lexema, tam, 0);
		} else {
			erroLexico(1, "");
			_recuperar(lexema, tam, 1);
		}
	} else {
		erroLexico(1, "");
		_recuperar(lexema, tam, 0);
	}
}

/*Función que colocará o punteiro do inicio de lexema na posición correspondente o seguinte lexema*/
void aceptar_lexema() {
	buf.inicio = buf.delantero;
	sobrepasa = 0;	//Desactivamos o flag de lexema moi grande
}