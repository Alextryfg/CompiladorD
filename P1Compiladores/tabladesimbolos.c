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

//Palbaras reservadas iniciales
palabras_reservadas estructuras[] = { { "ID", ID },
		{ "import", IMPORT }, { "STRING", STRING }, { "INT", INT },
		{ "FLOAT", FLOAT }, { "WHILE", WHILE }, { "FOREACH", FOREACH }, { "RETURN", RETURN }, { "VOID", VOID }};

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

//Imprime la tabla
void imprimirTabla() {
	imprimir_tabla(&arbol);
}

//Busca el codigo del lexema en la tabla de símbolos
int buscar(char *lexema) {
	return buscar_lexema(&arbol, lexema);
}