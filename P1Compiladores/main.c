#include "tabladesimbolos.h"
#include "sintactico.h"
#include <stdio.h>
#include <stdlib.h>
#include "sistemaEntrada.h"

int main(int argc, char **argv) {

		apertura_arquivo("regression.d");
		inicializarTaboa();
		printf("======== TABLA SÍMBOLOS =========\n");
		imprimirTaboa();
		printf("=================================\n\n");
		inicializar();
		peche_arquivo();
		printf("\n\n======== TABLA SÍMBOLOS =========\n");
		imprimirTaboa();
		printf("=================================\n");
		destruirTaboa();

}