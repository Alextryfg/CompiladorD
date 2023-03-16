#ifndef MANEXOENTRADA_SISTEMAENTRADA_H_
#include "abb.h"

#define N 64			//Tamaño de cada centinela
#define A 0		//Indicador de Bloque B
#define B 1		//Indicador de Bloque A

void init(char *archivo);//Se abre el archivo y se leen los primeros 64 caracteres
void cerrarD(); // Cerramos el archivo

/*LEXEMA FUNCTIONS*/
char siguiente_caracter();

void retroceder_puntero(); //Comprobamos la pos de delantero y la retrasamos una pos (Devolver_puntero)

void avanzar_delantero(char lect);

void getLexema(tipoelem *lexema); //recupera lexema

void aceptar_lexema(); //aceptar

void igualarPunteros();



#endif /* MANEXOENTRADA_SISTEMAENTRADA_H_ */