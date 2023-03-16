#ifndef MANEXOENTRADA_SISTEMAENTRADA_H_
#include "abb.h"

#define N 64			//Tamaño de cada centinela
#define A 0		//Indicador de Bloque B
#define B 1		//Indicador de Bloque A

/*
 * Inicializacion del sistema de entrada
 */
void init(char *input);

/*
 * Cerramos el archivo
*/
void cerrarD();

/*
 * Funcion utilizada para cargar los bloques en los distintos centinelas.
*/
void cargar_bloque();

/*
 * Funcion que ira devolviendo caracteres al analizador lexico dependiendo del caso
 */
char siguiente_caracter();

/*
 * Funcion encargada de retroceder el puntero delantero en los centinelas, para que se cumpla que la estructura sea cíclica
 */
void retroceder_puntero();

/*
 * Funcion privada para Igualar punteros
 */
void igualarPunteros();

/*
 * Funcion para recoger el lexema completo dependiendo de los distintos casos
 */
void getLexema(tipoelem *lexema);





#endif /* MANEXOENTRADA_SISTEMAENTRADA_H_ */