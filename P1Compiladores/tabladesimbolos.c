#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "definiciones.h"

/*Representation of the symbol table*/
abb arbol;

/*Keywords structure*/
typedef struct {
	char *lexema;
	int registro;
} palabras_reservadas;

//Initial keywords
palabras_reservadas estructuras[] = { { "package", PACKAGE },
		{ "import", IMPORT }, { "func", FUNC }, { "chan", CHAN },
		{ "var", VAR }, { "range", RANGE }, { "go", GO }, { "for", FOR } };

//Creating table and inserting keywords in the table
void inicializarTabla() {
	int nElems = sizeof(estructuras) / sizeof(estructuras[0]);
	crear(&arbol);

	for (int i = 0; i < nElems; i++) {
		insertar(&arbol, estructuras[i].lexema, estructuras[i].registro);

	}
}

//Destruction of the table
void destruirTabla() {
	destruir(&arbol);
}

//Print table
void imprimirTabla() {
	imprimir_tabla(&arbol);
}

//Search and insertion of lexeme
int buscar(char *lexema) {
	return buscar_lexema(&arbol, lexema);
}