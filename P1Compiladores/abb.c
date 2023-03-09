#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"

/*			STRUCTURES			*/

/*Elements of the tree
 *
 * char *lexema: Keywords or ID names
 * int codigo: Keywords or ID codes
 * */
typedef struct {
	char *lexema;
	int codigo;
} elemento;

/*Cells that make up the tree
 *
 * elemento info: lexeme
 * struct celda *izq, *der: pointer to child nodes
 * */
struct celda {
	elemento info;
	struct celda *izq, *der;
};

/*Tree Initialization*/
void crear(abb *A) {
	*A = NULL;
}

/*Tree destruction*/
void destruir(abb *A) {
	if (*A != NULL) {
		destruir(&(*A)->izq);	//destruction of the children of the left branch
		destruir(&(*A)->der);	//destruction of the children of the right branch
		if (&(*A)->info.lexema != NULL) {//lexeme memory release
			free((*A)->info.lexema);
			(*A)->info.lexema = NULL;
		}
		free(*A);				//tree memoria release
		*A = NULL;
	}
}

/*INFORMATION OPERATIONS*/

//Determine if the tree is empty
unsigned es_vacio(abb A) {
	return A == NULL;
}

//Return of the left subtree
abb izq(abb A) {
	return A->izq;
}

//Return of the right subtree
abb der(abb A) {
	return A->der;
}

/*MODIFICATION OPERATIONS*/

/*
 * This function allows the insertion of a lexeme and code in the tree.
 * First of all we check if the cell is empty, if this is the case,
 * the lexeme and the code are introduced and the pointers are initialized to NULL.
 * Otherwise, the lexemes are compared to know if it should go to a left or right branch,
 * recursively calling the function until insertion.
 * */
void insertar(abb *A, char *lexema, int codigo) {
	if (es_vacio(*A)) {
		*A = (abb) malloc(sizeof(struct celda));
		(*A)->info.lexema = (char*) malloc(strlen(lexema) + 1 * sizeof(char));
		strcpy((*A)->info.lexema, lexema);
		(*A)->info.lexema[strlen(lexema)] = '\0';
		(*A)->info.codigo = codigo;
		(*A)->izq = NULL;
		(*A)->der = NULL;
		return;
	}

	int comparacion = strcmp(lexema, (*A)->info.lexema);

	if (comparacion > 0) {
		insertar(&(*A)->der, lexema, codigo);
	} else {
		insertar(&(*A)->izq, lexema, codigo);
	}

}

/*
 * Function that performs the printing of the tree, it works as follows:
 * Check if the node is not empty, if there is a left child it will be called recursively until the end is reached and print the name and value of the lexeme.
 * In the same way, the process is carried out for the children of the right branches
 * */
void imprimir_tabla(abb *A) {
	if (!es_vacio(*A)) {
		if (&(*A)->izq != NULL) {
			imprimir_tabla(&(*A)->izq);
			printf("Lexema: %10s", (*A)->info.lexema);
		}

		printf("%10s: %-5d\n", "Valor", (*A)->info.codigo);

		if (&(*A)->der != NULL) {
			imprimir_tabla(&(*A)->der);
		}
	}
}

/*
 * Function in charge of searching for a lexeme, comes from the alphanumeric automaton.
 * If no lexeme with the same name is found, an insert is made into the child corresponding to the previous compared node.
 * The insert implies that it is an identifier, so this is returned, otherwise the corresponding code is returned.
 *
 * Return:
 * 		int: lexeme code
 * */
int buscar_lexema(abb *A, char *lexema) {
	if (es_vacio(*A)) {
		insertar(A, lexema, ID);
		return ID;
	}

	int comp = strcmp(lexema, (*A)->info.lexema);
	if (comp == 0) {
		return (*A)->info.codigo;
	} else if (comp < 0) {
		return (buscar_lexema(&(*A)->izq, lexema));
	} else {
		return (buscar_lexema(&(*A)->der, lexema));
	}

}