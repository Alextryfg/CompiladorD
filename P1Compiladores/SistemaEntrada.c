
#include "SistemaEntrada.h"
//Aqui deberia incluirse la gestion de errores
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 64			//Tamaño de cada centinela, multiplo de la unidad de asignacion (4096)
#define PRIMEIRO 0		//Position indicator on the first sentinel
#define SEGUNDO 1		//Position indicator on the second sentinel

typedef struct {
	char centA[N + 1];	//Primer centinela
	char centB[N + 1];	//Segundo centinela
	int inicio;			//Puntero que indica el comienzo de un lexema
	int delantero;		//Puntero delantero que
	int turno;	//Variable that allows knowing in which sentinel the reading is being performed
} buffer;

/*GLOBAL VARIABLES*/

FILE *archivoEntrada;		//input file
int pos_lectura = 0;		//Current position in the file
buffer buf;
int sobrepasa = 0;			//Flag to know if the lexeme is larger than allowed
int retroceso = 1;			//Flag to know if I can load the next block

/*
 * Buffer initialization
 * */
void crearBuffer() {
	buf.inicio = 0;
	buf.delantero = 0;
	buf.turno = PRIMEIRO;
}

/*
 * Function in charge of opening the file, initializing the buffer and reading the first N characters stored in the first sentinel.
 * */
void abrir_arquivo(char *arquivo) {
	//Opening the file with error checking
	if ((archivoEntrada = fopen(arquivo, "r")) != NULL) {
		crearBuffer();
		fseek(archivoEntrada, pos_lectura, SEEK_SET);
		int caracteres_lidos = fread(buf.centA, sizeof(char), N,
				archivoEntrada); //The first sentinel is filled
		buf.centA[caracteres_lidos] = EOF; //EOF is inserted in the last position
		if (!ferror(archivoEntrada)) { //Error checking
			fflush(archivoEntrada);
			pos_lectura = ftell(archivoEntrada);//Storing the reading position in the file
		} else {
			erroArquivo(2);
		}

	} else
		erroArquivo(1);
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
	if (buf.turno == PRIMEIRO) {
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
			erroArquivo(2);
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
			erroArquivo(2);
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
	if (buf.turno == PRIMEIRO) {
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

				buf.turno = PRIMEIRO;

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
 * Function that will make the forward pointer go back to the corresponding position when a character is found that determines the end of the lexeme.
 * Take into account those cases in which the recoil causes a change of sentinel
 * */
void devolver_caracter() {
	if (buf.turno == PRIMEIRO) {
		if (buf.delantero == 0) {
			buf.turno = SEGUNDO;
			buf.delantero = N * 2 - 1;
			retroceso = 0;
		} else {
			buf.delantero--;
		}
	} else if (buf.turno == SEGUNDO) {
		if (buf.delantero == N) {
			buf.turno = PRIMEIRO;
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
 * */

void _recuperar(char **lexema, int tam, int avanza) {

	if (!avanza) {
		*lexema = (char*) malloc(tam + 1 * sizeof(char));//memory reserve for the lexeme
		if (buf.inicio < N && buf.delantero >= N) {	//Differents sentinels
			strncpy(*lexema, buf.centA + buf.inicio, N - buf.inicio);
			strncpy(*lexema + N - buf.inicio, buf.centB, buf.delantero - N);
		} else if (buf.inicio > buf.delantero) {//Initial in second sentinel and forward in first sentinel.
			strncpy(*lexema, buf.centB + buf.inicio - N, (N * 2) - buf.inicio);
			strncpy(*lexema + (N * 2) - buf.inicio, buf.centA, buf.delantero);

		} else {
			if (buf.turno == PRIMEIRO) {
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