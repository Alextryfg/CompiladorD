#include "lexico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//El Sintactico, siguiendo el patron productor-consumidor, lee el siguiente componente lexico
//mediante un while. Este seguira leyendo hasta llegar al EOF del archivo. Cuando no queda nada
//se libera la memoria (ESTO ES MUY IMPORTANTE) y se termina el programa. Imprimiendo la tabla de 
//simbolos al final. (Nuestra tabla de símbolos es un árbol binario de búsqueda)

void inicializar() {
	/*Creation and initialization of the token*/
	compLexico *lex;
	lex = (compLexico*) malloc(sizeof(compLexico));
	lex->nombre_lexema = NULL;
	lex->codigo = -1;

	//Impression of the different lexical components
	while (seguinte_comp_lexico(lex)) {
		printf("<%d, %s>\n", lex->codigo, lex->nombre_lexema);
	}

	//memory release
	if (lex->nombre_lexema != NULL) {
		free(lex->nombre_lexema);
		lex->nombre_lexema = NULL;
	}
	free(lex);
	lex = NULL;
}