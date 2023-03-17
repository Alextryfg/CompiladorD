#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "definiciones.h"
#include "abb.h"

/*
 * Funcion que imprime el contenido del lexema dependiendo del caso
 */
void _printComp(tipoelem comp){
    //En caso de ser caracteres especiales
    if(comp.lexema[0] == '\r'){
        //printf("\n<%d,\\r>", comp.codigo);
    }else if(comp.lexema[0] == '\t'){
        //printf("\n<%d,\\t>", comp.codigo);
    }else if(comp.lexema[0] == '\n' || comp.lexema[1] == '\n'){
        //printf("\n<%d,\\n>", comp.codigo);
    }else if(comp.lexema[0] == ' '){
        //No se imprimen
    }else{
        printf("\n<%d,%s>", comp.codigo, comp.lexema);
    }

}
int a = 0;

/*
 * Funcion en cargada de ir pidiendo componentes lexicos al analizador lexico, liberando memoria e imprimiendo por pantalla el resultado
 */
void compilarD(){

    /* Inicializo el componenteLexico que le enviare a la funcion del lexico.c */
    tipoelem comp;
    comp.lexema= NULL;
    comp.codigo=-1;

    /*Invoco a siguiente_componente lexico hasta -1 que indica que llego el EOF */
    while(siguiente_componente_lexico(&comp) != -1){
        _printComp(comp);

        /* Se libera la memoria del lexema */
        if(comp.lexema!=NULL){
            free(comp.lexema);
            comp.lexema=NULL;
        }

        /* Termina */

    }











}