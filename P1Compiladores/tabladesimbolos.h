#ifndef TABLADESIMBOLOS_H
#define TABLADESIMBOLOS_H
#include "abb.h"

/*
 * Funcion auxiliar para insertar en la tabla de simbolos, las keywords iniciales
 */
void insertSimbol(tipoelem keys);

/*
 * Funcion encargada de inicializar la tabla de simbolos
 */
void initTabla();

/*
 * Funcion encargada de la destruccion del arbol
 */
void destruirTabla();

/*
 * Funcion encargada de encontrar un lexema dentro de el arbol. Si el elemento no esta en la tabla, se introducirá
 */

void findCodigo(tipoelem *simb);

/*
 * Funcion que imprime la tabla, invocando a la recursiva de abb.h
 */
void printTabla();


#endif	// TABLADESIMBOLOS_H