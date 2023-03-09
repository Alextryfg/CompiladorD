
#include "gestionerrores.h"
#include <stdio.h>

//File erros
void errorArchivo(int codigo){

	switch(codigo){
	case 1:
		/*File cannot be opened*/
		printf("No se puede abrir el archivo\n");
		break;
	case 2:
		/*Error reading file*/
		printf("Error en la lectura de archivo\n");
		break;
	}
}

//Lexeme erros
void errorLexico(int codigo, char *lexema) {
	switch (codigo) {
	case 1:
		/*Lexeme too large*/
		printf("El lexema sobrepasa el tamaño máximo\n");
		break;
	case 2:
		/*Misformed lexema*/
		printf("Lexema '%s' mal formado\n", lexema);
		break;
	}
}